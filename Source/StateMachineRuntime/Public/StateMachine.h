// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Tasks/StateMachineState.h"
#include "UObject/Object.h"
#include "StateMachine.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateMachineEvent, UStateMachineComponent*, OwnerComp, UStateMachineState*, State);

/**
 * 
 */
UCLASS()
class STATEMACHINERUNTIME_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FStateMachineEvent OnNewStateEntered;
	UPROPERTY(BlueprintAssignable)
	FStateMachineEvent OnStateExited;
	
#if WITH_EDITOR
	UEdGraph* GetSourceGraph() const;
	void SetSourceGraph(UEdGraph* InSourceGraph);
	void SetEntryTask(UStateMachineState* Task);
#endif

	UStateMachineState* GetEntryTask() const;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UEdGraph* SourceGraph;
#endif
	
	UPROPERTY()
	UStateMachineState* EntryTask;
};
