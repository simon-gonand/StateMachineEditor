// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateMachineComponent.generated.h"


class UStateMachineState;
class UStateMachine;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STATEMACHINERUNTIME_API UStateMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintGetter)
	UStateMachine* GetStateMachine() const;
	UFUNCTION(BlueprintGetter)
	UStateMachineState* GetCurrentState() const;

	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void EvaluateNextState();
	UFUNCTION(BlueprintCallable, Category = "StateMachine")
	void ExecuteTasksForCurrentState();

private:
	UPROPERTY(EditAnywhere, BlueprintGetter=GetStateMachine, Category = "StateMachine")
	UStateMachine* StateMachine;

	UPROPERTY(Transient, BlueprintGetter=GetCurrentState)
	UStateMachineState* CurrentState;

	UFUNCTION()
	void OnStateExited(UStateMachineComponent* OwnerComp, UStateMachineState* StateToBeExited);
};
