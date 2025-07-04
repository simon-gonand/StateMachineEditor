// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineTransitionEdGraphNode.h"

#include "GraphNodes/StateMachineTaskEdGraphNode.h"
#include "Nodes/StateMachineTransition.h"

void UStateMachineTransitionEdGraphNode::CreateConnections(UStateMachineEdGraphNode* NodeA,
                                                           UStateMachineEdGraphNode* NodeB)
{
	// Previous to this
	Pins[0]->Modify();
	Pins[0]->LinkedTo.Empty();

	NodeA->GetOutputPin()->Modify();
	Pins[0]->MakeLinkTo(NodeA->GetOutputPin());

	// This to next
	Pins[1]->Modify();
	Pins[1]->LinkedTo.Empty();

	NodeB->GetInputPin()->Modify();
	Pins[1]->MakeLinkTo(NodeB->GetInputPin());

	// Node Instance needs to have a default value
	if (NodeInstance)
	{
		NodeInstance->SetLinkedTask(Cast<UStateMachineTaskEdGraphNode>(NodeB)->GetNodeInstance());
	}
}

UStateMachineEdGraphNode* UStateMachineTransitionEdGraphNode::GetInputConnectedNode()
{
	if (Pins.IsEmpty() || !(Pins[0]) || Pins[0]->LinkedTo.IsEmpty() || !(Pins[0]->LinkedTo[0]))
		return nullptr;
	
	return Cast<UStateMachineEdGraphNode>(Pins[0]->LinkedTo[0]->GetOwningNode());
}

UStateMachineEdGraphNode* UStateMachineTransitionEdGraphNode::GetOutputConnectedNode()
{
	if (Pins.IsEmpty() || !(Pins[1]) || Pins[1]->LinkedTo.IsEmpty() || !(Pins[1]->LinkedTo[0]))
		return nullptr;
	
	return Cast<UStateMachineEdGraphNode>(Pins[1]->LinkedTo[0]->GetOwningNode());
}

UStateMachineTransition* UStateMachineTransitionEdGraphNode::GetNodeInstance() const
{
	return NodeInstance;
}

void UStateMachineTransitionEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT("In"));
	Inputs->bHidden = true;
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT("Out"));
	Outputs->bHidden = true;
}

void UStateMachineTransitionEdGraphNode::PinConnectionListChanged(UEdGraphPin* Pin)
{
	if (Pin->LinkedTo.IsEmpty())
	{
		// Commit suicide; transitions must always have an input and output connection
		Modify();

		// Our parent graph will have our graph in SubGraphs so needs to be modified to record that.
		if(UEdGraph* ParentGraph = GetGraph())
		{
			ParentGraph->Modify();
		}

		DestroyNode();
	}
}

void UStateMachineTransitionEdGraphNode::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UStateMachineTransitionEdGraphNode, NodeInstance))
	{
		if (NodeInstance)
		{
			if (UStateMachineTaskEdGraphNode* InputLinkedTaskGraphNode = Cast<UStateMachineTaskEdGraphNode>(GetInputConnectedNode()))
			{
				InputLinkedTaskGraphNode->NodeConnectionListChanged();
			}
			if (UStateMachineTaskEdGraphNode* OutputLinkedTaskGraphNode = Cast<UStateMachineTaskEdGraphNode>(GetOutputConnectedNode()))
			{
				NodeInstance->SetLinkedTask(OutputLinkedTaskGraphNode->GetNodeInstance());
			}

			NodeInstance->Modify();
			Modify();
		}
	}
}
