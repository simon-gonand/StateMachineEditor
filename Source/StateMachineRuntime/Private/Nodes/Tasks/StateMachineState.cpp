// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/Tasks/StateMachineState.h"

#include "StateMachine.h"
#include "StateMachineComponent.h"
#include "Nodes/StateMachineTransition.h"

void UStateMachineState::EmptyLinkedTransitions()
{
	if (!LinkedTransitions.IsEmpty())
		LinkedTransitions.Empty();
}

void UStateMachineState::AddLinkedTransition(UStateMachineTransition* TransitionToLink)
{
	if (TransitionToLink)
		LinkedTransitions.Add(TransitionToLink);
}

TArray<UStateMachineTransition*> UStateMachineState::GetLinkedTransitions() const
{
	return LinkedTransitions;
}

UStateMachineState* UStateMachineState::GetNextState(UStateMachineComponent* OwnerComp) const
{
	for (UStateMachineTransition* Transition : LinkedTransitions)
	{
		if (Transition && Transition->CanGoThrough(OwnerComp) && Transition->GetLinkedTasks())
			return Transition->GetLinkedTasks();
	}

	return nullptr;
}

FText UStateMachineState::GetTooltip_Implementation() const
{
	return FText::FromString("State of the state machine");
}

FLinearColor UStateMachineState::GetNodeTitleColor_Implementation() const
{
	return FLinearColor::Blue;
}

FText UStateMachineState::GetNodeTitle_Implementation() const
{
	return FText::FromString(GetClass()->GetName());
}

void UStateMachineState::EnteringState_Implementation(UStateMachineComponent* OwnerComp)
{
	if (OwnerComp && OwnerComp->GetStateMachine())
	{
		OwnerComp->GetStateMachine()->OnNewStateEntered.Broadcast(OwnerComp, this);
	}
}

void UStateMachineState::ExecuteTasks_Implementation(UStateMachineComponent* OwnerComp)
{
}

void UStateMachineState::ExitingState_Implementation(UStateMachineComponent* OwnerComp)
{
	if (OwnerComp && OwnerComp->GetStateMachine())
	{
		OwnerComp->GetStateMachine()->OnStateExited.Broadcast(OwnerComp, this);
	}
}