// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineEntryNode.h"

#include "StateMachineGraphSchema.h"

void UStateMachineEntryNode::AllocateDefaultPins()
{
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, UStateMachineGraphSchema::PC_Exec, TEXT("Entry"));
}
