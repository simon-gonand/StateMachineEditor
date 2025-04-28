// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "StateMachineEntryNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineEntryNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	virtual void AllocateDefaultPins() override;
};
