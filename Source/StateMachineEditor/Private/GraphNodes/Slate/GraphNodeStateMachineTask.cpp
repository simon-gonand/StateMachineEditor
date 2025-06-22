// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/Slate/GraphNodeStateMachineTask.h"

void SGraphNodeStateMachineTask::Construct(const FArguments& InArgs, UStateMachineTaskEdGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}
