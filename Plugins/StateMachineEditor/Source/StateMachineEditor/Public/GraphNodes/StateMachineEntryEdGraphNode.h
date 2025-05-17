// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineEdGraphNode.h"
#include "StateMachineEntryEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineEntryEdGraphNode : public UStateMachineEdGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
};
