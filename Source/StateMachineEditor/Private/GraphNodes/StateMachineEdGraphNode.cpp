// Fill out your copyright notice in the Description page of Project Settings.


#include "GraphNodes/StateMachineEdGraphNode.h"

#include "StateMachineEditor.h"
#include "StateMachineGraphSchema.h"

void UStateMachineEdGraphNode::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (!FromPin)
		return;
	
	UEdGraphPin* OutputPin = GetOutputPin();
	if (!OutputPin)
	{
		UE_LOG(LogStateMachineEditor, Warning, TEXT("This does not have an Output Pin. AutoWire can't work."))
		return;
	}

	UEdGraphPin* InputPin = GetInputPin();
    	if (!InputPin)
    	{
    		UE_LOG(LogStateMachineEditor, Warning, TEXT("This does not have an Input Pin. AutoWire can't work."))
    		return;
    	}

	if (GetSchema()->TryCreateConnection(FromPin, InputPin))
	{
		FromPin->GetOwningNode()->NodeConnectionListChanged();
	}
	else if (OutputPin != nullptr && GetSchema()->TryCreateConnection(OutputPin, FromPin))
	{
		NodeConnectionListChanged();
	}
}

UEdGraphPin* UStateMachineEdGraphNode::GetOutputPin()
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->PinType.PinCategory == UStateMachineGraphSchema::PC_Exec && Pin->Direction == EGPD_Output)
			return Pin;
	}

	return nullptr;
}

UEdGraphPin* UStateMachineEdGraphNode::GetInputPin()
{
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->PinType.PinCategory == UStateMachineGraphSchema::PC_Exec && Pin->Direction == EGPD_Input)
			return Pin;
	}

	return nullptr;
}
