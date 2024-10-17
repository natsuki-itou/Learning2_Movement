// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Learning2_Movement/Characters/EnhanceInputTestCharacter.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LEARNING2_MOVEMENT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//UCharacterAnimInstance(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	AEnhanceInputTestCharacter* OwnerCharacter;
	UCharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bShouldMove;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsFalling;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsJumping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	bool bIsDashing;
	

	void CalculateSpeed();
	void CalculateShouldMove();
	void CalsulateIsJumping();
	void CalculateIsFalling();
	void CalculateIsDashing();;
};
