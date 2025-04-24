#include "StateMachineEditorApp.h"

#include "StateMachineEditorMode.h"

FStateMachineEditorApp::FStateMachineEditorApp()
{
}

void FStateMachineEditorApp::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(InTabManager);
}

void FStateMachineEditorApp::InitEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<class IToolkitHost>& InitToolkitHost, UStateMachine* Object)
{
	TArray<UObject*> Objects;
	Objects.Add(Object);
	InitAssetEditor(Mode, InitToolkitHost, TEXT("StateMachineEditor"), FTabManager::FLayout::NullLayout,
		true, true, Objects);

	const FName DefaultMode = TEXT("StateMachineEditorMode");
	AddApplicationMode(DefaultMode, MakeShareable(new FStateMachineEditorMode(SharedThis(this))));
	SetCurrentMode(DefaultMode);
}
