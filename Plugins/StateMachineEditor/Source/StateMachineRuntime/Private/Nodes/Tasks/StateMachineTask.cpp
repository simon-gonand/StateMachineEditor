// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/Tasks/StateMachineTask.h"

void UStateMachineTask::EmptyLinkedTransitions()
{
	if (!LinkedTransitions.IsEmpty())
		LinkedTransitions.Empty();
}

void UStateMachineTask::AddLinkedTransition(UStateMachineTransition* TransitionToLink)
{
	if (TransitionToLink)
		LinkedTransitions.Add(TransitionToLink);
}

TArray<UStateMachineTransition*> UStateMachineTask::GetLinkedTransitions() const
{
	return LinkedTransitions;
}