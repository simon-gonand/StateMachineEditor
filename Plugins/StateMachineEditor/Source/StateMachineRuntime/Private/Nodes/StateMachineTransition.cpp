// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/StateMachineTransition.h"

void UStateMachineTransition::SetLinkedTask(UStateMachineState* TaskToLink)
{
	LinkedState = TaskToLink;
}

UStateMachineState* UStateMachineTransition::GetLinkedTasks() const
{
	return LinkedState;
}

bool UStateMachineTransition::CanGoThrough_Implementation(UStateMachineComponent* OwnerComp)
{
	return true;
}
