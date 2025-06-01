// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineComponent.h"

#include "StateMachine.h"
#include "StateMachineRuntime.h"

UStateMachine* UStateMachineComponent::GetStateMachine() const
{
	return StateMachine;
}

UStateMachineState* UStateMachineComponent::GetCurrentState() const
{
	return CurrentState;
}

void UStateMachineComponent::EvaluateNextState()
{
	if (!StateMachine)
	{
		UE_LOG(LogStateMachineRuntime, Error, TEXT("No State Machine is linked to StateMachineComponent %s"), GetOwner() ? *GetOwner()->GetName() : *GetName());
		return;
	}
		
	if (!CurrentState)
	{
		CurrentState = StateMachine->GetEntryTask();
	}
	else
	{
		UStateMachineState* NextState = CurrentState->GetNextState(this);
		if (NextState && NextState != CurrentState)
		{
			StateMachine->OnStateExited.AddDynamic(this, &UStateMachineComponent::OnStateExited);
			CurrentState->ExitingState(this);
			CurrentState = NextState;
		}
	}
}

void UStateMachineComponent::ExecuteTasksForCurrentState()
{
	if (!CurrentState)
	{
		EvaluateNextState();
		if (!CurrentState)
		{
			UE_LOG(LogStateMachineRuntime, Warning, TEXT("No State has been found or linked to the state machine"));
			return;
		}
	}

	CurrentState->ExecuteTasks(this);
}

void UStateMachineComponent::OnStateExited(UStateMachineComponent* OwnerComp, UStateMachineState* StateToBeExited)
{
	StateMachine->OnStateExited.RemoveAll(this);
	CurrentState->EnteringState(OwnerComp);
}
