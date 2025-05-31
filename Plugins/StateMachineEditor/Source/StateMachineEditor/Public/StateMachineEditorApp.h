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
	TSharedRef<SWidget> SpawnProperties() const;
	
	// Begin FAssetEditorToolkit interface
	virtual FName GetToolkitFName() const override { return FName(TEXT("StateMachineEditor")); }
	virtual FText GetBaseToolkitName() const override { return FText::FromString("StateMachineEditor"); }
	virtual FString GetWorldCentricTabPrefix() const override { return TEXT("StateMachineEditor"); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FColor(29,182,229); }
	// End FAssetEditorToolkit interface

	void RestoreStateMachine();
	
	void OnGraphEditorFocused(const TSharedRef<SGraphEditor>& InGraphEditor);

private:
	TSharedPtr<FDocumentTracker> DocumentManager;

	UEdGraph* GraphEditor;

	TSharedPtr<FUICommandList> GraphEditorCommands;

	TSharedPtr<SGraphEditor> StateMachineGraphEditor;

	/** Property View */
	TSharedPtr<class IDetailsView> DetailsView;

	void InitGraph(UStateMachine* StateMachine);
	void InitDocumentManager();
	void CreateCommandList();
	TSharedRef<SGraphEditor> CreateGraphEditorWidget(UEdGraph* InGraph);
	void CreateInternalWidgets();

	bool IsInEditingMode(bool bGraphEditable) const;
	bool IsPropertyEditable() const;
	
	// Graph Editor Commands Action
	bool CanDeleteSelectedNodes();
	void DeleteSelectedNodes();
	bool CanCopySelectedNodes();
	void CopySelectedNodes();
	bool CanPasteNodes();
	void PasteNodes();
	void DuplicateNodes();
	bool CanCutNodes();
	void CutNodes();
	void DeleteDuplicatableNodes();
	bool CanSelectAllNodes();
	void SelectAllNodes();

	void OnSelectedNodesChanged(const TSet<class UObject*>& NewSelection);
};