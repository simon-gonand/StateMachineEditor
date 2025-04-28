// Copyright Epic Games, Inc. All Rights Reserved.

#include "StateMachineEditor.h"

#include "StateMachineGraphSchema.h"

#define LOCTEXT_NAMESPACE "FStateMachineEditorModule"

void FStateMachineEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	StateMachineGraphNodeFactory = MakeShareable(new FStateMachineNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(StateMachineGraphNodeFactory);

	StateMachineGraphPinFactory = MakeShareable(new FStateMachinePinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(StateMachineGraphPinFactory);
}

void FStateMachineEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEdGraphUtilities::UnregisterVisualNodeFactory(StateMachineGraphNodeFactory);
	FEdGraphUtilities::UnregisterVisualPinFactory(StateMachineGraphPinFactory);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStateMachineEditorModule, StateMachineEditor)