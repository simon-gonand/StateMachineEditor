// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/StateMachineNode.h"
#include "StateMachineState.generated.h"

class UStateMachineComponent;
class UStateMachineTransition;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class STATEMACHINERUNTIME_API UStateMachineState : public UStateMachineNode
{
	GENERATED_BODY()

public:
	void EmptyLinkedTransitions();
	void AddLinkedTransition(UStateMachineTransition* TaskToLink);
	TArray<UStateMachineTransition*> GetLinkedTransitions() const;

	UStateMachineState* GetNextState(UStateMachineComponent* OwnerComp) const;

	UFUNCTION(BlueprintNativeEvent, Category = "StateMachine")
	void EnteringState(UStateMachineComponent* OwnerComp);
	
	UFUNCTION(BlueprintNativeEvent, Category = "StateMachine")
	void ExecuteTasks(UStateMachineComponent* OwnerComp);
	
	UFUNCTION(BlueprintNativeEvent, Category = "StateMachine")
	void ExitingState(UStateMachineComponent* OwnerComp);
	
private:
	UPROPERTY()
	TArray<UStateMachineTransition*> LinkedTransitions;
};
