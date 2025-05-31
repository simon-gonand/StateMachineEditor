// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateMachineEdGraphNode.h"
#include "Nodes/Tasks/StateMachineTask.h"
#include "StateMachineTaskEdGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineTaskEdGraphNode : public UStateMachineEdGraphNode
{
	GENERATED_BODY()

public:
	UStateMachineTask* GetNodeInstance() const;
	virtual void NodeConnectionListChanged() override;
	
protected:
	virtual void AllocateDefaultPins() override;

	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;

	virtual void PostPlacedNewNode() override;


	
private:
	UPROPERTY()
	TObjectPtr<UStateMachineTask> NodeInstance;

	void CheckInputLinks();
	void FillOutputLinks();
};
