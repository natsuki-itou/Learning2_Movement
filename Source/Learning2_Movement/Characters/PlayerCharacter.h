// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LEARNING2_MOVEMENT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UMotionWarpingComponent* MotionWarpingComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 JumpCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxJumpCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpImpulse = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DiveImpulse = -500.0f;

	// ì¸óÕÇ…ÇÊÇ¡ÇƒåƒÇ—èoÇ≥ÇÍÇÈä÷êî
	void MoveForward(float InputAxis);
	void MoveRight(float InputAxis);
	void BeginSprint();
	void EndSprint();
	virtual void Jump() override;
	void Dive();

	virtual void Landed(const FHitResult& Hit) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
