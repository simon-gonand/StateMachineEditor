// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/StateMachineNode.h"
#include "StateMachineTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class STATEMACHINERUNTIME_API UStateMachineTask : public UStateMachineNode
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<UStateMachineTask*> LinkedTasks;
};
