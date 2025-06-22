// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachineFactory.h"

#include "StateMachine.h"

UStateMachineFactory::UStateMachineFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UStateMachine::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* UStateMachineFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                                UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UStateMachine>(InParent, InName, Flags); 
}
