// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "PawnCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class DESCEND04_API UPawnCharacterMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	class UPawnCharacter* owner;

	float moveSpeed;

	void setMoveSpeed(float moveSpd);
};
