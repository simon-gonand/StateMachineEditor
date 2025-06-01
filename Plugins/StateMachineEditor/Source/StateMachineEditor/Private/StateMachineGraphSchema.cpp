// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineGraphSchema.h"

#include "AIGraphSchema.h"
#include "BlueprintActionDatabase.h"
#include "StateMachinesConnectionDrawingPolicy.h"
#include "Framework/Commands/GenericCommands.h"
#include "GraphNodes/StateMachineEntryEdGraphNode.h"
#include "GraphNodes/StateMachineTaskEdGraphNode.h"
#include "GraphNodes/StateMachineTransitionEdGraphNode.h"
#include "GraphNodes/Slate/GraphNodeStateMachineEntry.h"
#include "GraphNodes/Slate/GraphNodeStateMachineTask.h"
#include "GraphNodes/Slate/GraphNodeStateMachineTransition.h"
#include "KismetPins/SGraphPinExec.h"
#include "Nodes/StateMachineNode.h"
#include "Nodes/Tasks/StateMachineState.h"
#include "Settings/EditorStyleSettings.h"


#define LOCTEXT_NAMESPACE "FStateMachineGraphSchema"

TSharedPtr<class SGraphNode> FStateMachineNodeFactory::CreateNode(class UEdGraphNode* Node) const
{
	if (UStateMachineEntryEdGraphNode* EntryNode = Cast<UStateMachineEntryEdGraphNode>(Node))
	{
		return SNew(SGraphNodeStateMachineEntry, EntryNode); 
	}
	if (UStateMachineTaskEdGraphNode* TaskNode = Cast<UStateMachineTaskEdGraphNode>(Node))
	{
		return SNew(SGraphNodeStateMachineTask, TaskNode);
	}
	if (UStateMachineTransitionEdGraphNode* TransitionNode = Cast<UStateMachineTransitionEdGraphNode>(Node))
	{
		return SNew(SGraphNodeStateMachineTransition, TransitionNode);
	}
	return FGraphPanelNodeFactory::CreateNode(Node);
}

const FName UStateMachineGraphSchema::PC_Exec = FName("Exec");

TSharedPtr<class SGraphPin> FStateMachinePinFactory::CreatePin(class UEdGraphPin* Pin) const
{
	if (Pin->GetSchema()->IsA(UStateMachineGraphSchema::StaticClass()))
	{
		if (Pin->PinType.PinCategory == UStateMachineGraphSchema::PC_Exec)
			return SNew(SGraphPinExec, Pin);
	}

	return nullptr;
}

class FConnectionDrawingPolicy* FStateMachinePinConnectionFactory::CreateConnectionPolicy(
	const class UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor,
	const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements,
	class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UStateMachineGraphSchema::StaticClass()))
	{
		return new FStateMachinesConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements);
	}

	return nullptr;
}

namespace 
{
	// Maximum distance a drag can be off a node edge to require 'push off' from node
	const int32 NodeDistance = 60;
}

UEdGraphNode* FStateMachineSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

#if WITH_EDITOR
	// If there is a template, we actually use it
	if (NodeTemplate != nullptr)
	{
		const FScopedTransaction Transaction(LOCTEXT("AddNode", "Add Node"));
		ParentGraph->Modify();
		if (FromPin)
		{
			FromPin->Modify();
		}

		ResultNode = CreateNode(ParentGraph, FromPin, Location, NodeTemplate);
	}
#endif // WITH_EDITOR

	return ResultNode;
}

UEdGraphNode* FStateMachineSchemaAction_NewNode::PerformAction(class UEdGraph* ParentGraph,
	TArray<UEdGraphPin*>& FromPins, const FVector2D Location, bool bSelectNewNode)
{
	UEdGraphNode* ResultNode = nullptr;

#if WITH_EDITOR
	if (FromPins.Num() > 0)
	{
		ResultNode = PerformAction(ParentGraph, FromPins[0], Location, bSelectNewNode);

		// Try autowiring the rest of the pins
		for (int32 Index = 1; Index < FromPins.Num(); ++Index)
		{
			ResultNode->AutowireNewNode(FromPins[Index]);
		}
	}
	else
	{
		ResultNode = PerformAction(ParentGraph, nullptr, Location, bSelectNewNode);
	}
#endif // WITH_EDITOR

	return ResultNode;
}

void FStateMachineSchemaAction_NewNode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdGraphSchemaAction::AddReferencedObjects( Collector );

	// These don't get saved to disk, but we want to make sure the objects don't get GC'd while the action array is around
	Collector.AddReferencedObject( NodeTemplate );
}

UEdGraphNode* FStateMachineSchemaAction_NewNode::CreateNode(class UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, class UEdGraphNode* InNodeTemplate)
{
	// Duplicate template node to create new node

#if WITH_EDITOR
	InNodeTemplate->SetFlags(RF_Transactional);

	ParentGraph->AddNode(InNodeTemplate, true);

	InNodeTemplate->CreateNewGuid();
	InNodeTemplate->PostPlacedNewNode();
	InNodeTemplate->AllocateDefaultPins();
	InNodeTemplate->AutowireNewNode(FromPin);

	// For input pins, new node will generally overlap node being dragged off
	// Work out if we want to visually push away from connected node
	int32 XLocation = Location.X;
	if (FromPin && FromPin->Direction == EGPD_Input)
	{
		UEdGraphNode* PinNode = FromPin->GetOwningNode();
		const float XDelta = FMath::Abs(PinNode->NodePosX - Location.X);

		if (XDelta < NodeDistance)
		{
			// Set location to edge of current node minus the max move distance
			// to force node to push off from connect node enough to give selection handle
			XLocation = PinNode->NodePosX - NodeDistance;
		}
	}

	InNodeTemplate->NodePosX = XLocation;
	InNodeTemplate->NodePosY = Location.Y;
	InNodeTemplate->SnapToGrid(GetDefault<UEditorStyleSettings>()->GridSnapSize);
#endif // WITH_EDITOR

	return InNodeTemplate;
}

void UStateMachineGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UStateMachineEntryEdGraphNode> NodeCreator(Graph);
	UStateMachineEntryEdGraphNode* EntryNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();

	SetNodeMetaData(EntryNode, FNodeMetadata::DefaultGraphNode);
}

void UStateMachineGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	FGraphNodeClassHelper& NodeClassHelper = GetClassCache();
	AddNodeOption(TEXT("Tasks"), ContextMenuBuilder, UStateMachineState::StaticClass(), UStateMachineTaskEdGraphNode::StaticClass());
}

void UStateMachineGraphSchema::GetContextMenuActions(class UToolMenu* Menu,
	UGraphNodeContextMenuContext* Context) const
{
	if (Context->Node)
	{
		{
			FToolMenuSection& Section = Menu->AddSection("StateMachineGraphSchemaNodeActions", LOCTEXT("StateMachineGraphActionsMenuHeader", "Node Actions"));
			Section.AddMenuEntry(FGenericCommands::Get().Delete);
			Section.AddMenuEntry(FGenericCommands::Get().Cut);
			Section.AddMenuEntry(FGenericCommands::Get().Copy);
			Section.AddMenuEntry(FGenericCommands::Get().Duplicate);
		}
	}
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UStateMachineGraphSchema::CanCreateConnection(const UEdGraphPin* A,
                                                                           const UEdGraphPin* B) const
{
	if (A->GetOwningNode() == B->GetOwningNode())
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Can't connect two pins from the same node"));
	
	if ((A->Direction != EGPD_Output || B->Direction != EGPD_Input) &&
			(A->Direction != EGPD_Input || B->Direction != EGPD_Output))
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Needs to connect an Output to an Input"));

	if (A->GetOwningNode()->IsA(UStateMachineTaskEdGraphNode::StaticClass()) &&
		B->GetOwningNode()->IsA(UStateMachineTaskEdGraphNode::StaticClass()))
		return FPinConnectionResponse(CONNECT_RESPONSE_MAKE_WITH_CONVERSION_NODE, TEXT("Transition Connection"));
	
	return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT("Pin Connection"));
}

bool UStateMachineGraphSchema::CreateAutomaticConversionNodeAndConnections(UEdGraphPin* A, UEdGraphPin* B) const
{
	UStateMachineTaskEdGraphNode* NodeA = Cast<UStateMachineTaskEdGraphNode>(A->GetOwningNode());
	UStateMachineTaskEdGraphNode* NodeB = Cast<UStateMachineTaskEdGraphNode>(B->GetOwningNode());
	UEdGraph* Graph = NodeA->GetGraph();
	
	if ((NodeA != NULL) && (NodeB != NULL) 
		&& (NodeA->GetInputPin() != NULL) && (NodeA->GetOutputPin() != NULL)
		&& (NodeB->GetInputPin() != NULL) && (NodeB->GetOutputPin() != NULL))
	{
		FVector2D Location = (FVector2D(NodeA->NodePosX, NodeA->NodePosY) + FVector2D(NodeB->NodePosX, NodeB->NodePosY)) * 0.5f;
		UStateMachineTransitionEdGraphNode* TransitionNode =
			FStateMachineSchemaAction_NewNode::SpawnNodeFromTemplate<UStateMachineTransitionEdGraphNode>(NodeA->GetGraph(),
				NewObject<UStateMachineTransitionEdGraphNode>(Graph), Location, false);
		
		if (A->Direction == EGPD_Output)
		{
			TransitionNode->CreateConnections(NodeA, NodeB);
		}
		else
		{
			TransitionNode->CreateConnections(NodeB, NodeA);
		}
		return true;
	}
	
	return Super::CreateAutomaticConversionNodeAndConnections(A, B);
}

FLinearColor UStateMachineGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor::White;
}

TSharedPtr<FGraphNodeClassHelper> ClassCache;

FGraphNodeClassHelper& UStateMachineGraphSchema::GetClassCache() const
{
	if (!ClassCache.IsValid())
	{
		ClassCache = MakeShareable(new FGraphNodeClassHelper(UStateMachineNode::StaticClass()));
		FGraphNodeClassHelper::AddObservedBlueprintClasses(UStateMachineState::StaticClass());
		ClassCache->UpdateAvailableBlueprintClasses();
	}

	return *ClassCache.Get();
}

void UStateMachineGraphSchema::AddNodeOption(const FString& CategoryName, FGraphContextMenuBuilder& ContextMenuBuilder,
	TSubclassOf<class UStateMachineNode> RuntimeNode, TSubclassOf<class UStateMachineEdGraphNode> EditorNode) const
{
	FCategorizedGraphActionListBuilder ListBuilder(CategoryName);
	
	UEdGraph* Graph = (UEdGraph*)ContextMenuBuilder.CurrentGraph;
	
	TArray<FGraphNodeClassData> ClassData;
	GetClassCache().GatherClasses(RuntimeNode, ClassData);

	for (FGraphNodeClassData& NodeClassData : ClassData)
	{
		bool bIsAllowed = false;
		if (!NodeClassData.GetPackageName().IsEmpty())
		{
			bIsAllowed = FBlueprintActionDatabase::IsClassAllowed(FTopLevelAssetPath(FName(NodeClassData.GetPackageName()), FName(NodeClassData.GetClassName())), FBlueprintActionDatabase::EPermissionsContext::Node);
		}
		else
		{
			bIsAllowed = FBlueprintActionDatabase::IsClassAllowed(NodeClassData.GetClass(), FBlueprintActionDatabase::EPermissionsContext::Node);
		}

		if (!bIsAllowed)
			continue;

		const FText NodeTypeName = FText::FromString(FName::NameToDisplayString(NodeClassData.ToString(), false));
		TSharedPtr<FStateMachineSchemaAction_NewNode> NewNodeAction = MakeShareable(
			new FStateMachineSchemaAction_NewNode(NodeClassData.GetCategory(), NodeTypeName, FText::GetEmpty(), 0));
		ListBuilder.AddAction(NewNodeAction);

		UStateMachineEdGraphNode* TaskNode = NewObject<UStateMachineEdGraphNode>(Graph, EditorNode);
		TaskNode->ClassData = NodeClassData;
		NewNodeAction->NodeTemplate = TaskNode;
	}

	ContextMenuBuilder.Append(ListBuilder);
}