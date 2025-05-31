// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineNode.h"
#include "StateMachineTransition.generated.h"

class UStateMachineTask;
/**
 * 
 */
UCLASS()
class STATEMACHINERUNTIME_API UStateMachineTransition : public UStateMachineNode
{
	GENERATED_BODY()

public:
	void SetLinkedTask(UStateMachineTask* TaskToLink);
	UStateMachineTask* GetLinkedTasks() const;
	
private:
	UPROPERTY()
	UStateMachineTask* LinkedTask;
};
