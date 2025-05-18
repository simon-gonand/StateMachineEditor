#include "StateMachineEditorApp.h"

#include "StateMachineEditor.h"
#include "StateMachineEditorMode.h"
#include "StateMachineEditorTabFactories.h"
#include "StateMachineGraphSchema.h"
#include "GraphNodes/StateMachineEdGraphNode.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "HAL/PlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "StateMachineEditorApp"

FStateMachineEditorApp::FStateMachineEditorApp()
{
}

void FStateMachineEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	DocumentManager->SetTabManager(InTabManager);
	
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FStateMachineEditorApp::InitEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UStateMachine* Object)
{
	if (!Object)
	{
		UE_LOG(LogStateMachineEditor, Error, TEXT("StateMachineEditorApp::InitEditor: Object is null"));
		return;
	}

	if (Object->GetSourceGraph())
	{
		GraphEditor = Object->GetSourceGraph();
	}
	else
	{
		GraphEditor = FBlueprintEditorUtils::CreateNewGraph(Object, FName("StateMachineGraph"), UEdGraph::StaticClass(), UStateMachineGraphSchema::StaticClass());
		GraphEditor->GetSchema()->CreateDefaultNodesForGraph(*GraphEditor);
		Object->SetSourceGraph(GraphEditor);
	}

	
	TSharedPtr<FStateMachineEditorApp> ThisPtr(SharedThis(this));
	if(!DocumentManager.IsValid())
	{
		DocumentManager = MakeShareable(new FDocumentTracker);
		DocumentManager->Initialize(ThisPtr);

		// Register the document factories
		{
			TSharedRef<FDocumentTabFactory> GraphEditorFactory = MakeShareable(new FSMGraphEditorSummoner(ThisPtr,
				FSMGraphEditorSummoner::FOnCreateGraphEditorWidget::CreateSP(this, &FStateMachineEditorApp::CreateGraphEditorWidget)
				));
			
			DocumentManager->RegisterDocumentFactory(GraphEditorFactory);
		}
	}
	
	TArray<UObject*> Objects;
	Objects.Add(Object);
	InitAssetEditor(Mode, InitToolkitHost, TEXT("StateMachineEditor"), FTabManager::FLayout::NullLayout,
		true, true, Objects);

	const FName DefaultMode = TEXT("StateMachineEditorMode");
	AddApplicationMode(DefaultMode, MakeShareable(new FStateMachineEditorMode(SharedThis(this))));
	SetCurrentMode(DefaultMode);

	RegenerateMenusAndToolbars();
}

void FStateMachineEditorApp::RestoreStateMachine()
{
	TSharedRef<FTabPayload_UObject> Payload = FTabPayload_UObject::Make(GraphEditor);
	TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(Payload, FDocumentTracker::OpenNewDocument);
}

void FStateMachineEditorApp::CreateCommandList()
{
	if (GraphEditorCommands.IsValid())
		return;

	GraphEditorCommands = MakeShareable(new FUICommandList());

	GraphEditorCommands->MapAction(FGenericCommands::Get().Delete,
		FExecuteAction::CreateRaw(this, &FStateMachineEditorApp::DeleteSelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CanDeleteSelectedNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Copy,
		FExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CopySelectedNodes),
		FCanExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CanCopySelectedNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Paste,
    		FExecuteAction::CreateRaw(this, &FStateMachineEditorApp::PasteNodes),
    		FCanExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CanPasteNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Duplicate,
			FExecuteAction::CreateRaw(this, &FStateMachineEditorApp::DuplicateNodes),
			FCanExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CanCopySelectedNodes));

	GraphEditorCommands->MapAction(FGenericCommands::Get().Cut,
			FExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CutNodes),
			FCanExecuteAction::CreateRaw(this, &FStateMachineEditorApp::CanCutNodes));
}

TSharedRef<SGraphEditor> FStateMachineEditorApp::CreateGraphEditorWidget(UEdGraph* InGraph)
{
	CreateCommandList();
	
	check(InGraph != nullptr);

	// Create the title bar widget
	TSharedRef<SWidget> TitleBarWidget = 
		SNew(SBorder)
		.BorderImage( FAppStyle::GetBrush( TEXT("Graph.TitleBackground") ) )
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.FillWidth(1.f)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("StateMachineGraphLabel", "Document"))
				.TextStyle( FAppStyle::Get(), TEXT("GraphBreadcrumbButtonText") )
			]
		];

	FGraphAppearanceInfo AppearanceInfo;
	AppearanceInfo.CornerText = FText::FromString("State Machine");
	
	return SAssignNew(StateMachineGraphEditor, SGraphEditor)
		.AdditionalCommands(GraphEditorCommands)
		//.IsEditable(this, &FBehaviorTreeEditor::InEditingMode, bGraphIsEditable)
		.Appearance(AppearanceInfo)
		.TitleBar(TitleBarWidget)
		.GraphToEdit(InGraph)
		.AutoExpandActionMenu(true);
}

bool FStateMachineEditorApp::CanDeleteSelectedNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return false;
	}

	const FGraphPanelSelectionSet SelectedNodes = StateMachineGraphEditor->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*SelectedIter);
		if (Node && Node->CanUserDeleteNode())
		{
			return true;
		}
	}
	return false;
}

void FStateMachineEditorApp::DeleteSelectedNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Delete->GetDescription());
	StateMachineGraphEditor->GetCurrentGraph()->Modify();

	const FGraphPanelSelectionSet SelectedNodes = StateMachineGraphEditor->GetSelectedNodes();
	StateMachineGraphEditor->ClearSelectionSet();

	for (FGraphPanelSelectionSet::TConstIterator NodeIt(SelectedNodes); NodeIt; ++NodeIt)
	{
		if (UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}

bool FStateMachineEditorApp::CanCopySelectedNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return false;
	}

	const FGraphPanelSelectionSet SelectedNodes = StateMachineGraphEditor->GetSelectedNodes();
	for (FGraphPanelSelectionSet::TConstIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*SelectedIter);
		if (Node && Node->CanDuplicateNode())
		{
			return true;
		}
	}
	return false;
}

void FStateMachineEditorApp::CopySelectedNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return;
	}
	
	FGraphPanelSelectionSet SelectedNodes = StateMachineGraphEditor->GetSelectedNodes();

	FString ExportedText;

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*SelectedIter);
		if (!Node || !Node->CanDuplicateNode())
		{
			SelectedIter.RemoveCurrent();
			continue;
		}

		Node->PrepareForCopying();
	}

	FEdGraphUtilities::ExportNodesToText(SelectedNodes, ExportedText);
	FPlatformApplicationMisc::ClipboardCopy(*ExportedText);

	// Not needed for the moment but can be useful later
	/*for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*SelectedIter);
		if (Node)
		{
			Node->PostCopyNode();
		}
	}*/
}

bool FStateMachineEditorApp::CanPasteNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return false;
	}

	FString ClipboardContent;
	FPlatformApplicationMisc::ClipboardPaste(ClipboardContent);

	return FEdGraphUtilities::CanImportNodesFromText(StateMachineGraphEditor->GetCurrentGraph(), ClipboardContent);
}

void FStateMachineEditorApp::PasteNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Paste->GetDescription());
	UEdGraph* Graph = StateMachineGraphEditor->GetCurrentGraph();
	Graph->Modify();

	// Clear the selection set (newly pasted stuff will be selected)
	StateMachineGraphEditor->ClearSelectionSet();

	// Grab the text to paste from the clipboard.
	FString TextToImport;
	FPlatformApplicationMisc::ClipboardPaste(TextToImport);

	// Import the nodes
	TSet<UEdGraphNode*> PastedNodes;
	FEdGraphUtilities::ImportNodesFromText(Graph, TextToImport, /*out*/ PastedNodes);

	// Get current Paste Graph Location
	FVector2D PasteLocation = StateMachineGraphEditor->GetPasteLocation();

	// Average position of nodes so we can move them while still maintaining relative distances to each other
	FVector2D AvgNodePosition(0.0f, 0.0f);
	
	// Number of nodes used to calculate AvgNodePosition
	int32 AvgCount = 0;

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UStateMachineEdGraphNode* Node = Cast<UStateMachineEdGraphNode>(*It);
		if (!Node)
		{
			AvgNodePosition.X += Node->NodePosX;
			AvgNodePosition.Y += Node->NodePosY;
			++AvgCount;
		}
	}

	if (AvgCount > 0)
	{
		float InvNumNodes = 1.0f / float(AvgCount);
		AvgNodePosition.X *= InvNumNodes;
		AvgNodePosition.Y *= InvNumNodes;
	}

	for (TSet<UEdGraphNode*>::TIterator It(PastedNodes); It; ++It)
	{
		UStateMachineEdGraphNode* PastedNode = Cast<UStateMachineEdGraphNode>(*It);
		if (!PastedNode)
			continue;

		StateMachineGraphEditor->SetNodeSelection(PastedNode, true);

		const FVector::FReal NodePosX = (PastedNode->NodePosX - AvgNodePosition.X) + PasteLocation.X;
		const FVector::FReal NodePosY = (PastedNode->NodePosY - AvgNodePosition.Y) + PasteLocation.Y;

		PastedNode->NodePosX = static_cast<int32>(NodePosX);
		PastedNode->NodePosY = static_cast<int32>(NodePosY);

		PastedNode->SnapToGrid(16);

		// Give new node a different Guid from the old one
		PastedNode->CreateNewGuid();
	}

	// Update UI
	StateMachineGraphEditor->NotifyGraphChanged();

	UObject* GraphOwner = Graph->GetOuter();
	if (GraphOwner)
	{
		GraphOwner->PostEditChange();
		GraphOwner->MarkPackageDirty();
	}
}

void FStateMachineEditorApp::DuplicateNodes()
{
	CopySelectedNodes();
	PasteNodes();
}

bool FStateMachineEditorApp::CanCutNodes()
{
	return CanCopySelectedNodes() && CanDeleteSelectedNodes();
}

void FStateMachineEditorApp::CutNodes()
{
	CopySelectedNodes();
	DeleteDuplicatableNodes();
}

void FStateMachineEditorApp::DeleteDuplicatableNodes()
{
	if (!StateMachineGraphEditor.IsValid())
	{
		return;
	}

	const FScopedTransaction Transaction(FGenericCommands::Get().Duplicate->GetDescription());
	FGraphPanelSelectionSet SelectedNodes = StateMachineGraphEditor->GetSelectedNodes();

	for (FGraphPanelSelectionSet::TIterator SelectedIter(SelectedNodes); SelectedIter; ++SelectedIter)
	{
		UStateMachineEdGraphNode* SelectedNode = Cast<UStateMachineEdGraphNode>(*SelectedIter);
		if (!SelectedNode || !SelectedNode->CanDuplicateNode() || !SelectedNode->CanUserDeleteNode())
			continue;

		SelectedNode->Modify();
		SelectedNode->DestroyNode();
	}
}
