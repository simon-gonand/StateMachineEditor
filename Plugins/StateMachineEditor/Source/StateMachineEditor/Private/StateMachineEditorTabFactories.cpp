#include "StateMachineEditorTabFactories.h"

#include "StateMachineEditorApp.h"

const FName FStateMachineEditorTabs::GraphEditorID = FName("Document");

FSMGraphEditorSummoner::FSMGraphEditorSummoner(TSharedPtr<FStateMachineEditorApp>  InStateMachineEditorPtr, FOnCreateGraphEditorWidget OnCreateGraphEditorWidgetCallback)
	: FDocumentTabFactoryForObjects<UEdGraph>(FStateMachineEditorTabs::GraphEditorID, InStateMachineEditorPtr),
	StateMachineEditorPtr(InStateMachineEditorPtr),
	OnCreateGraphEditorWidget(OnCreateGraphEditorWidgetCallback)
{
}

TAttribute<FText> FSMGraphEditorSummoner::ConstructTabNameForObject(UEdGraph* DocumentID) const
{
	return TAttribute<FText>( FText::FromString( DocumentID->GetName() ) );
}

TSharedRef<SWidget> FSMGraphEditorSummoner::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info,
	UEdGraph* DocumentID) const
{
	return OnCreateGraphEditorWidget.Execute(DocumentID);
}

const FSlateBrush* FSMGraphEditorSummoner::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info,
	UEdGraph* DocumentID) const
{
	return FAppStyle::GetBrush(TEXT("GraphEditor.Function_24x") );
}
