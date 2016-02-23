// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "PawnCharacter.generated.h"

UCLASS()
class DESCEND04_API APawnCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnCharacter();

	// How quick the character can move
	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float moveSpeed;

	UPROPERTY(Category = Gameplay, EditAnywhere, BlueprintReadWrite)
		float dodgeSpeed;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	

	class UPawnCharacterMovementComponent* OurMovementComponent;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	int32 dodgeTime;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Attack();
	void Dodge();
	void AdvanceDodgeTimer();
	void DodgeOver();
	FTimerHandle DodgeTimerHandle;

};
