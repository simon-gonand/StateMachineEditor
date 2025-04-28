// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineGraphSchema.h"

#include "GraphNodes/StateMachineEntryNode.h"
#include "GraphNodes/Slate/GraphNodeStateMachineEntry.h"
#include "KismetPins/SGraphPinExec.h"

TSharedPtr<class SGraphNode> FStateMachineNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	if (UStateMachineEntryNode* EntryNode = Cast<UStateMachineEntryNode>(Node))
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
	FGraphNodeCreator<UStateMachineEntryNode> NodeCreator(Graph);
	UStateMachineEntryNode* EntryNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();

	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);
}

void UStateMachineGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	Super::GetGraphContextActions(ContextMenuBuilder);
}
