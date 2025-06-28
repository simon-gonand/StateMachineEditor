// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/Slate/GraphNodeStateMachineTask.h"

void SGraphNodeStateMachineTask::Construct(const FArguments& InArgs, UStateMachineTaskEdGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

FReply SGraphNodeStateMachineTask::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry,
	const FPointerEvent& InMouseEvent)
{
	FReply Reply = SGraphNode::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
	if (UStateMachineTaskEdGraphNode* TaskGraphNode = Cast<UStateMachineTaskEdGraphNode>(GraphNode))
	{
		TaskGraphNode->OpenNodeInstanceEditor();
		return FReply::Handled();
	}
	return Reply;
}
