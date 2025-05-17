// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineTaskEdGraphNode.h"

#include "StateMachineGraphSchema.h"

void UStateMachineTaskEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Input = CreatePin(EGPD_Input, UStateMachineGraphSchema::PC_Exec, TEXT("In"));
	UEdGraphPin* Output = CreatePin(EGPD_Output, UStateMachineGraphSchema::PC_Exec, TEXT("Out"));
}