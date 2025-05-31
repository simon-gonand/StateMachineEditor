// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/StateMachineTransition.h"

void UStateMachineTransition::SetLinkedTask(UStateMachineTask* TaskToLink)
{
	LinkedTask = TaskToLink;
}

UStateMachineTask* UStateMachineTransition::GetLinkedTasks() const
{
	return LinkedTask;
}
