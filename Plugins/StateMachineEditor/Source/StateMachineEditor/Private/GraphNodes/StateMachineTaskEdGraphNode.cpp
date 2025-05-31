// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineTaskEdGraphNode.h"

#include "StateMachine.h"
#include "StateMachineGraphSchema.h"
#include "GraphNodes/StateMachineEntryEdGraphNode.h"
#include "GraphNodes/StateMachineTransitionEdGraphNode.h"

UStateMachineTask* UStateMachineTaskEdGraphNode::GetNodeInstance() const
{
	return NodeInstance;
}

void UStateMachineTaskEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Input = CreatePin(EGPD_Input, UStateMachineGraphSchema::PC_Exec, TEXT("In"));
	UEdGraphPin* Output = CreatePin(EGPD_Output, UStateMachineGraphSchema::PC_Exec, TEXT("Out"));
}

void UStateMachineTaskEdGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
	class UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

void UStateMachineTaskEdGraphNode::PostPlacedNewNode()
{
	UClass* NodeClass = ClassData.GetClass(true);
	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* Graph = GetGraph();
		UObject* GraphOwner = Graph ? Graph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			NodeInstance = NewObject<UStateMachineTask>(Graph, NodeClass);
			NodeInstance->SetFlags(RF_Transactional);
		}
	}
}

void UStateMachineTaskEdGraphNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	CheckInputLinks(); // If input is linked to entry then set entry task in the StateMachine object
	FillOutputLinks(); // Fill the linked tasks to the ouput pin
	Modify();
}

void UStateMachineTaskEdGraphNode::CheckInputLinks()
{
	UEdGraphPin* InputPin = GetInputPin();
	if (!InputPin)
		return;

	if (!InputPin->LinkedTo.IsEmpty())
	{
		// No need to check the owning node since GetOwningNode function already does a check
		if (InputPin->LinkedTo[0] &&
			InputPin->LinkedTo[0]->GetOwningNode()->IsA(UStateMachineEntryEdGraphNode::StaticClass()))
		{
			if (UEdGraph* Graph = GetGraph())
			{
				if (UStateMachine* StateMachine = Cast<UStateMachine>(Graph->GetOuter()))
				{
					StateMachine->SetEntryTask(NodeInstance);
				}
			}
		}
	}
	else
	{
		if (UEdGraph* Graph = GetGraph())
		{
			if (UStateMachine* StateMachine = Cast<UStateMachine>(Graph->GetOuter()))
			{
				StateMachine->SetEntryTask(nullptr);
			}
		}
	}
}


void UStateMachineTaskEdGraphNode::FillOutputLinks()
{
	UEdGraphPin* OutputPin = GetOutputPin();
	if (!OutputPin)
		return;

	NodeInstance->EmptyLinkedTransitions();

	for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
	{
		if (!LinkedPin)
			continue;

		if (UStateMachineTransitionEdGraphNode* TaskEdGraph = Cast<UStateMachineTransitionEdGraphNode>(LinkedPin->GetOwningNode()))
		{
			NodeInstance->AddLinkedTransition(TaskEdGraph->GetNodeInstance());
		}
	}
}
