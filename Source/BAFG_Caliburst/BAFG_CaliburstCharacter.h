// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BAFG_CaliburstCharacter.generated.h"

UCLASS(config=Game)
class ABAFG_CaliburstCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	void StartAttack1();
	void StartAttack2();
	void StartAttack3();
	void StartAttack4();
	
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damageAmount);


protected:

	/** Called for side to side input */
	void MoveRight(float Val);

	/** Handle touch inputs. */
	void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Handle touch stop event. */
	void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//ECharacterClass characterClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
	AActor* hurtbox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attack1Used;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool attack2Used;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damages")
	bool wasAttacked;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float playerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ABAFG_CaliburstCharacter* otherPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;


public:
	ABAFG_CaliburstCharacter();

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
