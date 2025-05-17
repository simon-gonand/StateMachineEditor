// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIGraphTypes.h"
#include "EdGraph/EdGraphNode.h"
#include "StateMachineEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineEdGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

public:
	UPROPERTY()
	struct FGraphNodeClassData ClassData;

protected:
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;

private:
	UEdGraphPin* GetOutputPin();
	UEdGraphPin* GetInputPin();
};
