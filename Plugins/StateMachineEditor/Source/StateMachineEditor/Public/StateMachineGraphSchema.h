// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "EdGraph/EdGraphSchema.h"
#include "StateMachineGraphSchema.generated.h"

struct STATEMACHINEEDITOR_API FStateMachineNodeFactory : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(class UEdGraphNode* Node) const override;
};

struct STATEMACHINEEDITOR_API FStateMachinePinFactory : public FGraphPanelPinFactory
{
	virtual TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* Pin) const override;
};

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineGraphSchema : public UEdGraphSchema
{
	GENERATED_BODY()

public:
	static const FName PC_Exec;
	
	// Begin UEdGraphSchema Interface
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const override;
	// End UEdGraphSchema Interface
};
