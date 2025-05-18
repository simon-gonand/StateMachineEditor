// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineTransitionEdGraphNode.h"

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
