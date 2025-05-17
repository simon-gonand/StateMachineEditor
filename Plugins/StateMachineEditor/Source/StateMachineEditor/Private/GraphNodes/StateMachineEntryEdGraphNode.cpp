// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineEntryEdGraphNode.h"

#include "StateMachineGraphSchema.h"

void UStateMachineEntryEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, UStateMachineGraphSchema::PC_Exec, TEXT("Entry"));
}

bool UStateMachineEntryEdGraphNode::CanUserDeleteNode() const
{
	return false;
}
