// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

struct FStateMachinePinFactory;
struct FStateMachineNodeFactory;

DECLARE_LOG_CATEGORY_CLASS(LogStateMachineEditor, Log, All);

class FStateMachineEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	TSharedPtr<FStateMachineNodeFactory> StateMachineGraphNodeFactory;
	TSharedPtr<FStateMachinePinFactory> StateMachineGraphPinFactory;
};
