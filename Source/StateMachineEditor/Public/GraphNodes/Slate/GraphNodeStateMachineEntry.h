#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"

class UStateMachineEntryEdGraphNode;

class SGraphNodeStateMachineEntry: public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeStateMachineEntry){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UStateMachineEntryEdGraphNode* InNode);

	// Begin SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// End SGraphNode interface

private:
	FSlateColor GetBackgroundColor() const;
};
