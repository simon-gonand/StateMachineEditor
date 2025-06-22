// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineNode.h"
#include "StateMachineTransition.generated.h"

class UStateMachineComponent;
class UStateMachineState;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew, Abstract)
class STATEMACHINERUNTIME_API UStateMachineTransition : public UStateMachineNode
{
	GENERATED_BODY()

public:
	void SetLinkedTask(UStateMachineState* TaskToLink);
	UStateMachineState* GetLinkedTasks() const;

	UFUNCTION(BlueprintNativeEvent, Category = "StateMachine")
	bool CanGoThrough(UStateMachineComponent* OwnerComp);
	
private:
	UPROPERTY()
	UStateMachineState* LinkedState;
};
