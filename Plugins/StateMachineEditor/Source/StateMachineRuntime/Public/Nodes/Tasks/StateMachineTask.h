// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/StateMachineNode.h"
#include "StateMachineTask.generated.h"

class UStateMachineTransition;
/**
 * 
 */
UCLASS(Blueprintable)
class STATEMACHINERUNTIME_API UStateMachineTask : public UStateMachineNode
{
	GENERATED_BODY()

public:
	void EmptyLinkedTransitions();
	void AddLinkedTransition(UStateMachineTransition* TaskToLink);
	TArray<UStateMachineTransition*> GetLinkedTransitions() const;
	
private:
	UPROPERTY()
	TArray<UStateMachineTransition*> LinkedTransitions;
};
