// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/Tasks/StateMachineTask.h"
#include "UObject/Object.h"
#include "StateMachine.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINERUNTIME_API UStateMachine : public UObject
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	UEdGraph* GetSourceGraph() const;
	void SetSourceGraph(UEdGraph* InSourceGraph);
	void SetEntryTask(UStateMachineTask* Task);
#endif

	UStateMachineTask* GetEntryTask() const;

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	UEdGraph* SourceGraph;
#endif
	
	UPROPERTY()
	UStateMachineTask* EntryTask;
};
