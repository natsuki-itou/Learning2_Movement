// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "InputMappingContext.h" // í«â¡
#include "InputAction.h" // í«â¡
#include "InputActionValue.h" // í«â¡
#include "EnhanceInputTestCharacter.generated.h"

UCLASS()
class LEARNING2_MOVEMENT_API AEnhanceInputTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnhanceInputTestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UMotionWarpingComponent* MotionWarpingComponent;

	// For InputActions
	// BluePrintâªÇµÇΩéûÇ…çÏê¨ÇµÇΩInputActionÇ∆InputMappingContextÇìoò^Ç≈Ç´ÇÈÇÊÇ§Ç…Ç∑ÇÈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* AxisInput;

	void Move(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	//void SetupInput();
};
