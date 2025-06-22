#include "StateMachineEditorTabFactories.h"

#include "StateMachineEditorApp.h"

#define LOCTEXT_NAMESPACE "StateMachineEditorTabFactories"

const FName FStateMachineEditorTabs::GraphEditorID = FName("Document");
const FName FStateMachineEditorTabs::GraphDetailsID = FName("Details");

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

void FSMGraphEditorSummoner::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	check(StateMachineEditorPtr.IsValid());
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	StateMachineEditorPtr.Pin()->OnGraphEditorFocused(GraphEditor);
}

FSMDetailsSummoner::FSMDetailsSummoner(TSharedPtr<class FStateMachineEditorApp> InStateMachineEditorPtr)
	: FWorkflowTabFactory(FStateMachineEditorTabs::GraphDetailsID, InStateMachineEditorPtr),
	  StateMachineEditorPtr(InStateMachineEditorPtr)
{
	TabLabel = LOCTEXT("StateMachineDetailsLabel", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("StateMachineDetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("StateMachineDetailsView_ToolTip", "Show the details view");
}

TSharedRef<SWidget> FSMDetailsSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	check(StateMachineEditorPtr.IsValid());
	return StateMachineEditorPtr.Pin()->SpawnProperties();
}

FText FSMDetailsSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("StateMachineDetailsTabTooltip", "The state machine details tab allows editing of the properties of state machine nodes");
}
