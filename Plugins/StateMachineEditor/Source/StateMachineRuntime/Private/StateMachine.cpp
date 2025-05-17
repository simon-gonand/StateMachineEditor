// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine.h"

#if WITH_EDITOR
void UStateMachine::SetEntryTask(UStateMachineTask* Task)
{
	EntryTask = Task;
}

UEdGraph* UStateMachine::GetSourceGraph() const
{
	return SourceGraph;
}

void UStateMachine::SetSourceGraph(UEdGraph* InSourceGraph)
{
	SourceGraph = InSourceGraph;
}
#endif
