// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetDefinition_StateMachine.h"

#include "StateMachine.h"

#define LOCTEXT_NAMESPACE "AssetDefinition"

FText UAssetDefinition_StateMachine::GetAssetDisplayName() const
{
	return LOCTEXT("AssetDefinition_StateMachine", "State Machine"); // In case I add Localization
}

FLinearColor UAssetDefinition_StateMachine::GetAssetColor() const
{
	return FColor(29, 182, 229);
}

TSoftClassPtr<UObject> UAssetDefinition_StateMachine::GetAssetClass() const
{
	return UStateMachine::StaticClass();
}

TConstArrayView<FAssetCategoryPath> UAssetDefinition_StateMachine::GetAssetCategories() const
{
	static const FAssetCategoryPath Categories[] = { EAssetCategoryPaths::AI };
	return Categories;
}

EAssetCommandResult UAssetDefinition_StateMachine::OpenAssets(const FAssetOpenArgs& OpenArgs) const
{
	return Super::OpenAssets(OpenArgs);
}

EAssetCommandResult UAssetDefinition_StateMachine::PerformAssetDiff(const FAssetDiffArgs& DiffArgs) const
{
	return Super::PerformAssetDiff(DiffArgs);
}
