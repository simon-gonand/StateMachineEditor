// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "GraphNodes/StateMachineTaskEdGraphNode.h"
#include "UObject/Object.h"

/**
 * 
 */
class STATEMACHINEEDITOR_API SGraphNodeStateMachineTask : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeStateMachineTask){}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UStateMachineTaskEdGraphNode* InNode);
};
