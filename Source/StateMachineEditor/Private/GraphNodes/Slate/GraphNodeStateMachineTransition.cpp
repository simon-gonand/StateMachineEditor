// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/Slate/GraphNodeStateMachineTransition.h"

#include "ConnectionDrawingPolicy.h"
#include "GraphNodes/StateMachineTaskEdGraphNode.h"

void SGraphNodeStateMachineTransition::Construct(const FArguments& InArgs, UStateMachineTransitionEdGraphNode* InNode)
{
	this->GraphNode = InNode;

	this->SetCursor(EMouseCursor::Hand);

	this->UpdateGraphNode();
}

void SGraphNodeStateMachineTransition::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty)
{
	// Can't be moved by the user
}

void SGraphNodeStateMachineTransition::UpdateGraphNode()
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
			SNew(SOverlay)
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image( FAppStyle::GetBrush("Graph.TransitionNode.ColorSpill") )
				.ColorAndOpacity( FLinearColor(0.9f, 0.9f, 0.9f, 1.0f) )
			]
			+SOverlay::Slot()
			[
				SNew(SImage)
				.Image( FAppStyle::GetBrush("Graph.TransitionNode.Icon"))
			]
		];
}

bool SGraphNodeStateMachineTransition::RequiresSecondPassLayout() const
{
	return true;
}

void SGraphNodeStateMachineTransition::PerformSecondPassLayout(
	const TMap<UObject*, TSharedRef<SNode>>& InNodeToWidgetLookup) const
{
	if (!GraphNode)
		return;

	UStateMachineTransitionEdGraphNode* TransitionEdGraphNode = Cast<UStateMachineTransitionEdGraphNode>(GraphNode);
	
	// Find the geometry of the state nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	UStateMachineTaskEdGraphNode* InputConnectedNode = Cast<UStateMachineTaskEdGraphNode>(TransitionEdGraphNode->GetInputConnectedNode());
	UStateMachineTaskEdGraphNode* OutputConnectedNode = Cast<UStateMachineTaskEdGraphNode>(TransitionEdGraphNode->GetOutputConnectedNode());
	if (InputConnectedNode && OutputConnectedNode)
	{
		const TSharedRef<SNode>* pInputConnectedNodeWidget = InNodeToWidgetLookup.Find(InputConnectedNode);
		const TSharedRef<SNode>* pOutputConnectedNodeWidget = InNodeToWidgetLookup.Find(OutputConnectedNode);
		if (pInputConnectedNodeWidget && pOutputConnectedNodeWidget)
		{
			const TSharedRef<SNode>& InputConnectedNodeWidget = *pInputConnectedNodeWidget;
			const TSharedRef<SNode>& OutputConnectedNodeWidget = *pOutputConnectedNodeWidget;

			StartGeom = FGeometry(FVector2D(InputConnectedNode->NodePosX, InputConnectedNode->NodePosY), FVector2D::ZeroVector, InputConnectedNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(OutputConnectedNode->NodePosX, OutputConnectedNode->NodePosY), FVector2D::ZeroVector, OutputConnectedNodeWidget->GetDesiredSize(), 1.0f);
		}
	}

	//Position Node
	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom);
}

void SGraphNodeStateMachineTransition::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom,
	const FGeometry& EndGeom) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * DesiredNodeSize.Size());

	GraphNode->NodePosX = static_cast<int32>(NewCorner.X);
	GraphNode->NodePosY = static_cast<int32>(NewCorner.Y);
}
