// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineEdGraphNode.h"
#include "StateMachineTransitionEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineTransitionEdGraphNode : public UStateMachineEdGraphNode
{
	GENERATED_BODY()

public:
	void CreateConnections(UStateMachineEdGraphNode* NodeA, UStateMachineEdGraphNode* NodeB);

	UStateMachineEdGraphNode* GetInputConnectedNode();
	UStateMachineEdGraphNode* GetOutputConnectedNode();

protected:
	virtual void AllocateDefaultPins() override;

	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
};
