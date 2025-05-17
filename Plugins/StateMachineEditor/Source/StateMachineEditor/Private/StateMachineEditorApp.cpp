#include "StateMachineEditorApp.h"

#include "StateMachineEditor.h"
#include "StateMachineEditorMode.h"
#include "StateMachineEditorTabFactories.h"
#include "StateMachineGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"

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
		UEdGraphNode* Node = Cast<UEdGraphNode>(*SelectedIter);
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
		if (UEdGraphNode* Node = Cast<UEdGraphNode>(*NodeIt))
		{
			if (Node->CanUserDeleteNode())
			{
				Node->Modify();
				Node->DestroyNode();
			}
		}
	}
}
