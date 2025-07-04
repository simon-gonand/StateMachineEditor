#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"
#include "Templates/SharedPointer.h"
#include "EdGraph/EdGraph.h"

struct FStateMachineEditorTabs
{
	static const FName GraphEditorID;
	static const FName GraphDetailsID;
};

struct FSMGraphEditorSummoner : public FDocumentTabFactoryForObjects<UEdGraph>
{
public:
	DECLARE_DELEGATE_RetVal_OneParam(TSharedRef<SGraphEditor>, FOnCreateGraphEditorWidget, UEdGraph*);
	
	FSMGraphEditorSummoner(TSharedPtr<class FStateMachineEditorApp> InStateMachineEditorPtr, FOnCreateGraphEditorWidget OnCreateGraphEditorWidgetCallback);

protected:
	virtual TAttribute<FText> ConstructTabNameForObject(UEdGraph* DocumentID) const override;
	virtual TSharedRef<SWidget> CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const override;
	virtual const FSlateBrush* GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const override;
	virtual void OnTabActivated(TSharedPtr<SDockTab> Tab) const override;

private:
	TWeakPtr<FStateMachineEditorApp> StateMachineEditorPtr;
	FOnCreateGraphEditorWidget OnCreateGraphEditorWidget;
};

struct FSMDetailsSummoner : public FWorkflowTabFactory
{
public:
	FSMDetailsSummoner(TSharedPtr<class FStateMachineEditorApp> InStateMachineEditorPtr);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

protected:
	TWeakPtr<class FStateMachineEditorApp> StateMachineEditorPtr;
};
