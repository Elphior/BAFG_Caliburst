// Copyright Epic Games, Inc. All Rights Reserved.

#include "BAFG_CaliburstCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BAFG_CaliburstGameMode.h"

ABAFG_CaliburstCharacter::ABAFG_CaliburstCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	//CameraBoom->bDoCollisionTest = false;
	//CameraBoom->TargetArmLength = 500.f;
	//CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	//CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	//// Create a camera and attach to boom
	//SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	//SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	playerHealth = 1.f;
	attack1Used = false;
	attack2Used = false;
	wasAttacked = false;

	hurtbox = nullptr;
	directionalInput = EDirectionalInput::VE_Default;
	wasMediumAttackUsed = false;
	wasHeavyAttackUsed = false;
	hasAttackConnected = false;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABAFG_CaliburstCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	if (auto gameMode = Cast<ABAFG_CaliburstGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == this)
		{
			PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
			PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
			PlayerInputComponent->BindAxis("MoveRight", this, &ABAFG_CaliburstCharacter::MoveRight);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ABAFG_CaliburstCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ABAFG_CaliburstCharacter::TouchStopped);
	
			PlayerInputComponent->BindAction("Attack1P1", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack1);
			PlayerInputComponent->BindAction("Attack2P1", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack2);
			PlayerInputComponent->BindAction("Attack3P1", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack3);
			PlayerInputComponent->BindAction("Attack4P1", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack4);
		}
		else
		{
			PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ABAFG_CaliburstCharacter::P2Jump);
			PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ABAFG_CaliburstCharacter::P2StopJump);
			PlayerInputComponent->BindAxis("MoveRightP2", this, &ABAFG_CaliburstCharacter::MoveRight);

			PlayerInputComponent->BindTouch(IE_Pressed, this, &ABAFG_CaliburstCharacter::TouchStarted);
			PlayerInputComponent->BindTouch(IE_Released, this, &ABAFG_CaliburstCharacter::TouchStopped);

			PlayerInputComponent->BindAction("Attack1P2", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack1);
			PlayerInputComponent->BindAction("Attack2P2", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack2);
			PlayerInputComponent->BindAction("Attack3P2", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack3);
			PlayerInputComponent->BindAction("Attack4P2", IE_Pressed, this, &ABAFG_CaliburstCharacter::StartAttack4);
		}
	}
}

void ABAFG_CaliburstCharacter::TakeDamage(float damageAmount)
{
	UE_LOG(LogTemp, Warning, TEXT("Took %f damage"), damageAmount);
	playerHealth -= damageAmount;
	if (playerHealth <= 0.f)
		playerHealth = 0.f;
	if (otherPlayer)
		otherPlayer->hasAttackConnected = true;
}

void ABAFG_CaliburstCharacter::P2Attack1()
{
	StartAttack1();
}

void ABAFG_CaliburstCharacter::P2Attack2()
{
	StartAttack2();
}

void ABAFG_CaliburstCharacter::P2Attack3()
{
	StartAttack3();
}

void ABAFG_CaliburstCharacter::P2Attack4()
{
	StartAttack4();
}

void ABAFG_CaliburstCharacter::P2Jump()
{
	Jump();
}

void ABAFG_CaliburstCharacter::P2StopJump()
{
	StopJumping();
}

void ABAFG_CaliburstCharacter::P2MoveRight(float val)
{
	MoveRight(val);
}

void ABAFG_CaliburstCharacter::StartAttack1()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack1"));
	attack1Used = true;
}

void ABAFG_CaliburstCharacter::StartAttack2()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack2"));
	attack2Used = true;
	wasMediumAttackUsed = true;
}

void ABAFG_CaliburstCharacter::StartAttack3()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack3"));
}

void ABAFG_CaliburstCharacter::StartAttack4()
{
	UE_LOG(LogTemp, Warning, TEXT("Attack4"));
	TakeDamage(0.05f);
	wasAttacked = true;
}

void ABAFG_CaliburstCharacter::MoveRight(float Value)
{
	if (Value > 0.20f)
	{
		if (!isFlipped)
			directionalInput = EDirectionalInput::VE_MovingForward;
		else
			directionalInput = EDirectionalInput::VE_MovingBack;

	}
	else if (Value < -0.20f)
	{
		if (isFlipped)
			directionalInput = EDirectionalInput::VE_MovingForward;
		else
			directionalInput = EDirectionalInput::VE_MovingBack;
		
	}
	else
		directionalInput = EDirectionalInput::VE_Default;
	
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ABAFG_CaliburstCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ABAFG_CaliburstCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

