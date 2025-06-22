#include "StateMachineEditorMode.h"

#include "StateMachineEditorApp.h"
#include "StateMachineEditorTabFactories.h"

FStateMachineEditorMode::FStateMachineEditorMode(TSharedPtr<class FStateMachineEditorApp> InEditorApp)
	: FApplicationMode(TEXT("StateMachineEditorMode"))
{
	EditorApp = InEditorApp;

	// Register Tab Factories here
	Tabs.RegisterFactory(MakeShareable(new FSMDetailsSummoner(InEditorApp)));
	
	TabLayout = FTabManager::NewLayout("StateMachineEditorMode_Layout_V3")
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
				->SetSizeCoefficient(0.7f)
				->AddTab(FStateMachineEditorTabs::GraphEditorID, ETabState::ClosedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.3f)
				->AddTab(FStateMachineEditorTabs::GraphDetailsID, ETabState::OpenedTab)
			)
		)
	);
}

void FStateMachineEditorMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	check(EditorApp.IsValid());

	TSharedPtr<FStateMachineEditorApp> StateMachineEditorPtr = EditorApp.Pin();

	StateMachineEditorPtr->PushTabFactories(Tabs);
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
