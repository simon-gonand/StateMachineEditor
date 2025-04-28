#include "GraphNodes/Slate/GraphNodeStateMachineEntry.h"

#include "SGraphPin.h"
#include "GraphNodes/StateMachineEntryNode.h"

void SGraphNodeStateMachineEntry::Construct(const FArguments& InArgs, UStateMachineEntryNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::CardinalCross);

	this->UpdateGraphNode();
}

void SGraphNodeStateMachineEntry::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();
	
	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind( this, &SGraphNode::GetContentScale );

	this->GetOrAddSlot( ENodeZone::Center )
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage( FAppStyle::GetBrush( "Graph.StateNode.Body" ) )
			.Padding(0)
			.BorderBackgroundColor( this, &SGraphNodeStateMachineEntry::GetBackgroundColor )
			[
				SNew(SOverlay)

				// PIN AREA
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(10.0f)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]
			]			
		];

	CreatePinWidgets();
}

void SGraphNodeStateMachineEntry::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner( SharedThis(this) );
	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];
	OutputPins.Add(PinToAdd);
}

FSlateColor SGraphNodeStateMachineEntry::GetBackgroundColor() const
{
	return FLinearColor(0.08f, 0.08f, 0.08f);
}