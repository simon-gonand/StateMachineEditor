#include "StateMachineEditorMode.h"

#include "StateMachineEditorTabFactories.h"

FStateMachineEditorMode::FStateMachineEditorMode(TSharedPtr<class FStateMachineEditorApp> InEditorApp)
	: FApplicationMode(TEXT("StateMachineEditorMode"))
{
	EditorApp = InEditorApp;

	// Register Tab Factories here	
	
	TabLayout = FTabManager::NewLayout("StateMachineEditorMode_Layout_V1")
	->AddArea
	(
		FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
				->AddTab(FStateMachineEditorTabs::GraphEditorID, ETabState::OpenedTab)
		)
	);
}

void FStateMachineEditorMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	FApplicationMode::RegisterTabFactories(InTabManager);
}

void FStateMachineEditorMode::PreDeactivateMode()
{
	FApplicationMode::PreDeactivateMode();
}

void FStateMachineEditorMode::PostActivateMode()
{
	FApplicationMode::PostActivateMode();
}
