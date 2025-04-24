#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditorModes.h"

class FStateMachineEditorMode: public FApplicationMode
{
public:
	FStateMachineEditorMode(TSharedPtr<class FStateMachineEditorApp> InEditorApp);

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PreDeactivateMode() override;
	virtual void PostActivateMode() override;

private:
	TWeakPtr<class FStateMachineEditorApp> EditorApp;
	FWorkflowAllowedTabSet Tabs;
};
