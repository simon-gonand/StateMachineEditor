// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineGraphSchema.h"

#include "BlueprintActionDatabase.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphNodes/StateMachineEntryEdGraphNode.h"
#include "GraphNodes/StateMachineTaskEdGraphNode.h"
#include "GraphNodes/Slate/GraphNodeStateMachineEntry.h"
#include "KismetPins/SGraphPinExec.h"
#include "Nodes/StateMachineNode.h"
#include "Nodes/Tasks/StateMachineTask.h"


#define LOCTEXT_NAMESPACE "FStateMachineGraphSchema"

TSharedPtr<class SGraphNode> FStateMachineNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	if (UStateMachineEntryEdGraphNode* EntryNode = Cast<UStateMachineEntryEdGraphNode>(Node))
	{
		return SNew(SGraphNodeStateMachineEntry, EntryNode); 
	}
	return FGraphPanelNodeFactory::CreateNode(Node);
}

const FName UStateMachineGraphSchema::PC_Exec = FName("Exec");

TSharedPtr<class SGraphPin> FStateMachinePinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	if (Pin->GetSchema()->IsA(UStateMachineGraphSchema::StaticClass()))
	{
		if (Pin->PinType.PinCategory == UStateMachineGraphSchema::PC_Exec)
			return SNew(SGraphPinExec, Pin);
	}

	return nullptr;
}

void UStateMachineGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UStateMachineEntryEdGraphNode> NodeCreator(Graph);
	UStateMachineEntryEdGraphNode* EntryNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();

	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);
}

void UStateMachineGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FGraphNodeClassHelper& NodeClassHelper = GetClassCache();
	AddNodeOption(TEXT("Tasks"), ContextMenuBuilder, UStateMachineTask::StaticClass(), UStateMachineTaskEdGraphNode::StaticClass());
}

void UStateMachineGraphSchema::GetContextMenuActions(class UToolMenu* Menu,
	UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("StateMachineGraphSchemaNodeActions", LOCTEXT("StateMachineGraphActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		}
	}
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UStateMachineGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                           const UEdGraphPin* B) const
{
	if ((A->Direction != EGPD_Output || B->Direction != EGPD_Input) &&
			(A->Direction != EGPD_Input && B->Direction != EGPD_Output))
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Needs to connect an Output to an Input"));
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Pin Connection"));
}

TSharedPtr<FGraphNodeClassHelper> ClassCache;

FGraphNodeClassHelper& UStateMachineGraphSchema::GetClassCache() const
{
	if (!ClassCache.IsValid())
	{
		ClassCache = MakeShareable(new FGraphNodeClassHelper(UStateMachineNode::StaticClass()));
		FGraphNodeClassHelper::AddObservedBlueprintClasses(UStateMachineTask::StaticClass());
		ClassCache->UpdateAvailableBlueprintClasses();
	}

	return *ClassCache.Get();
}

void UStateMachineGraphSchema::AddNodeOption(const FString& CategoryName, FGraphContextMenuBuilder& ContextMenuBuilder,
	TSubclassOf<class UStateMachineNode> RuntimeNode, TSubclassOf<class UStateMachineEdGraphNode> EditorNode) const
{
	FCategorizedGraphActionListBuilder ListBuilder(CategoryName);
	
	TArray<FGraphNodeClassData> ClassData;
	GetClassCache().GatherClasses(RuntimeNode, ClassData);

	for (FGraphNodeClassData& NodeClassData : ClassData)
	{
		bool bIsAllowed = false;
		if (!NodeClassData.GetPackageName().IsEmpty())
		{
			bIsAllowed = FBlueprintActionDatabase::IsClassAllowed(FTopLevelAssetPath(FName(NodeClassData.GetPackageName()), FName(NodeClassData.GetClassName())), FBlueprintActionDatabase::EPermissionsContext::Node);
		}
		else
		{
			bIsAllowed = FBlueprintActionDatabase::IsClassAllowed(NodeClassData.GetClass(), FBlueprintActionDatabase::EPermissionsContext::Node);
		}

		if (!bIsAllowed)
			continue;

		const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClassData.ToString(), false));
		TSharedPtr<FEdGraphSchemaAction_NewNode> NewNodeAction = MakeShareable(
			new FEdGraphSchemaAction_NewNode(NodeClassData.GetCategory(), NodeTypeName, FText::GetEmpty(), 0));
		ListBuilder.AddAction(NewNodeAction);

		UStateMachineEdGraphNode* TaskNode = NewObject<UStateMachineEdGraphNode>(ContextMenuBuilder.OwnerOfTemporaries, EditorNode);
		TaskNode->ClassData = NodeClassData;
		NewNodeAction->NodeTemplate = TaskNode;
	}

	ContextMenuBuilder.Append(ListBuilder);
}