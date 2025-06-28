// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineTaskEdGraphNode.h"

#include "StateMachine.h"
#include "StateMachineGraphSchema.h"
#include "GraphNodes/StateMachineEntryEdGraphNode.h"
#include "GraphNodes/StateMachineTransitionEdGraphNode.h"

UStateMachineState* UStateMachineTaskEdGraphNode::GetNodeInstance() const
{
	return NodeInstance;
}

void UStateMachineTaskEdGraphNode::AllocateDefaultPins()
{
	UEdGraphPin* Input = CreatePin(EGPD_Input, UStateMachineGraphSchema::PC_Exec, TEXT("In"));
	UEdGraphPin* Output = CreatePin(EGPD_Output, UStateMachineGraphSchema::PC_Exec, TEXT("Out"));
}

FText UStateMachineTaskEdGraphNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	if (NodeInstance)
		return NodeInstance->GetNodeTitle();
	return Super::GetNodeTitle(TitleType);
}

FLinearColor UStateMachineTaskEdGraphNode::GetNodeTitleColor() const
{
	if (NodeInstance)
		return NodeInstance->GetNodeTitleColor();
	return Super::GetNodeTitleColor();
}

FText UStateMachineTaskEdGraphNode::GetTooltipText() const
{
	if (NodeInstance)
		return NodeInstance->GetTooltip();
	return Super::GetTooltipText();
}

void UStateMachineTaskEdGraphNode::GetNodeContextMenuActions(class UToolMenu* Menu,
                                                             class UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);
}

void UStateMachineTaskEdGraphNode::PostPlacedNewNode()
{
	UClass* NodeClass = ClassData.GetClass(true);
	if (NodeClass && (NodeInstance == nullptr))
	{
		UEdGraph* Graph = GetGraph();
		UObject* GraphOwner = Graph ? Graph->GetOuter() : nullptr;
		if (GraphOwner)
		{
			NodeInstance = NewObject<UStateMachineState>(Graph, NodeClass);
			NodeInstance->SetFlags(RF_Transactional);
		}
	}
}

void UStateMachineTaskEdGraphNode::NodeConnectionListChanged()
{
	Super::NodeConnectionListChanged();
	CheckInputLinks(); // If input is linked to entry then set entry task in the StateMachine object
	FillOutputLinks(); // Fill the linked tasks to the ouput pin
	Modify();
}

void UStateMachineTaskEdGraphNode::OpenNodeInstanceEditor()
{
	if (NodeInstance && NodeInstance->GetClass()->IsInBlueprint())
	{
		UClass* NodeClass = NodeInstance->GetClass();
		UPackage* Pkg = NodeClass->GetOuterUPackage();
		FString ClassName = NodeClass->GetName().LeftChop(2);
		UBlueprint* BlueprintOb = FindObject<UBlueprint>(Pkg, *ClassName);
		if(UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			AssetEditorSubsystem->OpenEditorForAsset(BlueprintOb);
		}
	}
}

void UStateMachineTaskEdGraphNode::CheckInputLinks()
{
	UEdGraphPin* InputPin = GetInputPin();
	if (!InputPin)
		return;

	if (!InputPin->LinkedTo.IsEmpty())
	{
		// No need to check the owning node since GetOwningNode function already does a check
		if (InputPin->LinkedTo[0] &&
			InputPin->LinkedTo[0]->GetOwningNode()->IsA(UStateMachineEntryEdGraphNode::StaticClass()))
		{
			if (UEdGraph* Graph = GetGraph())
			{
				if (UStateMachine* StateMachine = Cast<UStateMachine>(Graph->GetOuter()))
				{
					StateMachine->SetEntryTask(NodeInstance);
				}
			}
		}
	}
	else
	{
		if (UEdGraph* Graph = GetGraph())
		{
			if (UStateMachine* StateMachine = Cast<UStateMachine>(Graph->GetOuter()))
			{
				StateMachine->SetEntryTask(nullptr);
			}
		}
	}
}


void UStateMachineTaskEdGraphNode::FillOutputLinks()
{
	UEdGraphPin* OutputPin = GetOutputPin();
	if (!OutputPin || !NodeInstance)
		return;

	NodeInstance->EmptyLinkedTransitions();

	for (UEdGraphPin* LinkedPin : OutputPin->LinkedTo)
	{
		if (!LinkedPin)
			continue;

		if (UStateMachineTransitionEdGraphNode* TaskEdGraph = Cast<UStateMachineTransitionEdGraphNode>(LinkedPin->GetOwningNode()))
		{
			NodeInstance->AddLinkedTransition(TaskEdGraph->GetNodeInstance());
		}
	}
}
