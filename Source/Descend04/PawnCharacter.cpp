// Fill out your copyright notice in the Description page of Project Settings.

#include "Descend04.h"
#include "PawnCharacter.h"
#include "PawnCharacterMovementComponent.h"


// Sets default values
APawnCharacter::APawnCharacter()
{

	// Stats
	moveSpeed = 300.0f;
	dodgeSpeed = 800.0f;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Our root component will be a sphere that reacts to physics
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create and position a mesh component so we can see where our sphere is
	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->AttachTo(RootComponent);
	SpringArm->RelativeRotation = FRotator(-75.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->AttachTo(SpringArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of our movement component, and tell it to update the root.
	OurMovementComponent = CreateDefaultSubobject<UPawnCharacterMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
	OurMovementComponent->setMoveSpeed(moveSpeed);

}

// Called when the game starts or when spawned
void APawnCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void APawnCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("Attack", IE_Pressed, this, &APawnCharacter::Attack);
	InputComponent->BindAction("Dodge", IE_Pressed, this, &APawnCharacter::Dodge);

	InputComponent->BindAxis("MoveForward", this, &APawnCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnCharacter::MoveRight);
}

UPawnMovementComponent* APawnCharacter::GetMovementComponent() const
{
	return OurMovementComponent;
}

void APawnCharacter::MoveForward(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);		
	}
}

void APawnCharacter::MoveRight(float AxisValue)
{
	if (OurMovementComponent && (OurMovementComponent->UpdatedComponent == RootComponent))
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void APawnCharacter::Attack()
{

}

void APawnCharacter::Dodge()
{
	OurMovementComponent->setMoveSpeed(dodgeSpeed);
	dodgeTime = 3;
	GetWorldTimerManager().SetTimer(DodgeTimerHandle, this, &APawnCharacter::AdvanceDodgeTimer, 0.1f, true);
}

void APawnCharacter::AdvanceDodgeTimer()
{
	--dodgeTime;
	if (dodgeTime < 1)
	{
		GetWorldTimerManager().ClearTimer(DodgeTimerHandle);
		DodgeOver();
	}
}

void APawnCharacter::DodgeOver()
{
	OurMovementComponent->setMoveSpeed(moveSpeed);
}