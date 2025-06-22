// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineEdGraphNode.h"
#include "Nodes/Tasks/StateMachineState.h"
#include "StateMachineTaskEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineTaskEdGraphNode : public UStateMachineEdGraphNode
{
	GENERATED_BODY()

public:
	UStateMachineState* GetNodeInstance() const;
	virtual void NodeConnectionListChanged() override;
	
protected:
	virtual void AllocateDefaultPins() override;

	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void PostPlacedNewNode() override;


	
private:
	UPROPERTY()
	TObjectPtr<UStateMachineState> NodeInstance;

	void CheckInputLinks();
	void FillOutputLinks();
};
