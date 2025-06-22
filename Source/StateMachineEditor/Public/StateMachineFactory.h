// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "StateMachineFactory.generated.h"

/**
 * 
 */
UCLASS()
class STATEMACHINEEDITOR_API UStateMachineFactory : public UFactory
{
	GENERATED_BODY()

public:
	UStateMachineFactory(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
