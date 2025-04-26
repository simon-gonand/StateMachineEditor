#include "StateMachineEditorMode.h"

#include "StateMachineEditorApp.h"
#include "StateMachineEditorTabFactories.h"

FStateMachineEditorMode::FStateMachineEditorMode(TSharedPtr<class FStateMachineEditorApp> InEditorApp)
	: FApplicationMode(TEXT("StateMachineEditorMode"))
{
	EditorApp = InEditorApp;

	// Register Tab Factories here	
	
	TabLayout = FTabManager::NewLayout("StateMachineEditorMode_Layout_V2")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->Split
			(
			FTabManager::NewStack()
				->SetSizeCoefficient(1.0f)
				->AddTab(FStateMachineEditorTabs::GraphEditorID, ETabState::ClosedTab)
			)
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
	check(EditorApp.IsValid());
	TSharedPtr<FStateMachineEditorApp> Editor = EditorApp.Pin();
	Editor->RestoreStateMachine();
	FApplicationMode::PostActivateMode();
}
