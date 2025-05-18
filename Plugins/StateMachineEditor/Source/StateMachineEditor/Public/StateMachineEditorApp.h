#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditor.h"
#include "StateMachine.h"
#include "Framework/Commands/GenericCommands.h"

class FStateMachineEditorApp : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook
{
public:
	FStateMachineEditorApp();
	
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	void InitEditor(const EToolkitMode::Type Mode,  const TSharedPtr<class IToolkitHost>& InitToolkitHost, UStateMachine* Object);
	
	// Begin FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("StateMachineEditor")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("StateMachineEditor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("StateMachineEditor"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FColor(29,182,229); }
	// End FAssetEditorToolkit interface

	void RestoreStateMachine();

private:
	TSharedPtr<FDocumentTracker> DocumentManager;

	UEdGraph* GraphEditor;

	TSharedPtr<FUICommandList> GraphEditorCommands;

	TSharedPtr<SGraphEditor> StateMachineGraphEditor;

	void CreateCommandList();
	TSharedRef<SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);
	
	// Graph Editor Commands Action
	bool CanDeleteSelectedNodes();
	void DeleteSelectedNodes();
	bool CanCopySelectedNodes();
	void CopySelectedNodes();
	bool CanPasteNodes();
	void PasteNodes();
};