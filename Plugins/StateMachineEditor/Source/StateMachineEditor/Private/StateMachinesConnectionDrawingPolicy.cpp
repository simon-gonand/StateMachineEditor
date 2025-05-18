// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachinesConnectionDrawingPolicy.h"

#include "GraphNodes/StateMachineTransitionEdGraphNode.h"

FStateMachinesConnectionDrawingPolicy::FStateMachinesConnectionDrawingPolicy(int32 InBackLayerID, int32 InFrontLayerID,
                                                                             float ZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements)
		:FConnectionDrawingPolicy(InBackLayerID, InBackLayerID, ZoomFactor, InClippingRect, InDrawElements)
{
}

void FStateMachinesConnectionDrawingPolicy::Draw(TMap<TSharedRef<SWidget>, FArrangedWidget>& InPinGeometries,
	FArrangedChildren& ArrangedNodes)
{
	// Build an acceleration structure to quickly find geometry for the nodes
	NodeIndexMap.Empty();
	for (int32 NodeIndex = 0; NodeIndex < ArrangedNodes.Num(); ++NodeIndex)
	{
		FArrangedWidget& CurWidget = ArrangedNodes[NodeIndex];
		TSharedRef<SGraphNode> ChildNode = StaticCastSharedRef<SGraphNode>(CurWidget.Widget);
		NodeIndexMap.Add(ChildNode->GetNodeObj(), NodeIndex);
	}
	
	FConnectionDrawingPolicy::Draw(InPinGeometries, ArrangedNodes);
}

void FStateMachinesConnectionDrawingPolicy::DetermineLinkGeometry(FArrangedChildren& ArrangedNodes,
                                                                  TSharedRef<SWidget>& OutputPinWidget, UEdGraphPin* OutputPin, UEdGraphPin* InputPin,
                                                                  FArrangedWidget*& StartWidgetGeometry, FArrangedWidget*& EndWidgetGeometry)
{
	if (UStateMachineTransitionEdGraphNode* TransNode = Cast<UStateMachineTransitionEdGraphNode>(InputPin->GetOwningNode()))
    {
    	UStateMachineEdGraphNode* InputConnectedNode = TransNode->GetInputConnectedNode();
    	UStateMachineEdGraphNode* OutputConnectedNode = TransNode->GetOutputConnectedNode();
    	if (InputConnectedNode && OutputConnectedNode)
    	{
    		int32* InputConnectedNodeIndex = NodeIndexMap.Find(InputConnectedNode);
    		int32* OutputConnectedNodeIndex = NodeIndexMap.Find(OutputConnectedNode);
    		if (InputConnectedNodeIndex && OutputConnectedNodeIndex)
    		{
    			StartWidgetGeometry = &(ArrangedNodes[*InputConnectedNodeIndex]);
    			EndWidgetGeometry = &(ArrangedNodes[*OutputConnectedNodeIndex]);
    		}
    	}
    }
	else
	{
		FConnectionDrawingPolicy::DetermineLinkGeometry(ArrangedNodes, OutputPinWidget, OutputPin, InputPin,
														StartWidgetGeometry, EndWidgetGeometry);
	}
}
