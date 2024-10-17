// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MotionWarpingComponent.h"
#include "InputMappingContext.h" // 追加
#include "InputAction.h" // 追加
#include "InputActionValue.h" // 追加
#include "TimerManager.h"
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

	UPROPERTY(VisibleAnywhere, Category = "Default Property")
	float DefaultYaw = -90.0f;

	// For EnhancedInput
	// BluePrint化した時に作成したInputActionとInputMappingContextを登録できるようにする
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* MoveInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* SprintInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* DashInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* JumpInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* DiveInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* CameraMoveInputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* CameraSetForwardInputAction;

	UPROPERTY(VisibleAnywhere, Category = "Movement Property")
	FVector2D DirectionalVector2D;// Dash,Jump,Diveの方向指定に利用する
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float NormalSpeed = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float SprintSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float DashDistance = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float DashFlame = 0.30f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	bool bIsDashing;

	UPROPERTY(VisibleAnywhere, Category = "Movement Property")
	int32 JumpCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	int32 MaxJumpCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float JumpImpulse = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float JumpForwardImpulse = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float AirJumpForwardImpulse = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float DiveImpulse = -500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float DiveForwardImpulse = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	float DiveGravity = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement Property")
	bool bIsDiveing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Warping")
	UAnimInstance* AnimInstanceRef;// MotionWarp用変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Warping")
	UAnimMontage* DashMontage;// MotionWarp用変数
	UPROPERTY(VisibleAnywhere)
	bool bCheck;
	UPROPERTY(VisibleAnywhere)
	bool bCheckOnGround;
	UPROPERTY(VisibleAnywhere)
	float CheckVelocityZ;

	void Move(const FInputActionValue& Value);
	void ResetDirection();
	void BeginSprint();
	void EndSprint();
	void Dash();
	void DashEnd();
	void Jump();
	void Dive();
	virtual void Landed(const FHitResult& Hit) override;
	void CameraMove(const FInputActionValue& Value);
	void CameraSetForward();

private:
	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetIsDashing();

};
