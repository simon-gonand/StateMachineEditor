// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "GraphNodes/StateMachineTransitionEdGraphNode.h"
#include "UObject/Object.h"

/**
 * 
 */
class STATEMACHINEEDITOR_API SGraphNodeStateMachineTransition : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeStateMachineTransition){}
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs, UStateMachineTransitionEdGraphNode* InNode);

protected:
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	virtual void UpdateGraphNode() override;
	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& InNodeToWidgetLookup) const override;

private:
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom) const;
};
