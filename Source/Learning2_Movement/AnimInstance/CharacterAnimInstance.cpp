// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"

void UCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AEnhanceInputTestCharacter>(GetOwningActor());
	if (OwnerCharacter != nullptr) {
		CharacterMovementComp = OwnerCharacter->GetCharacterMovement();
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	//SuperÇÃåƒÇ—èoÇµ
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter != nullptr) {
		CalculateShouldMove();
		CalculateSpeed();
		CalsulateIsJumping();
		CalculateIsFalling();
		CalculateIsDashing();
	}
	else {
		OwnerCharacter = Cast<AEnhanceInputTestCharacter>(GetOwningActor());
		if (OwnerCharacter != nullptr) {
			CharacterMovementComp = OwnerCharacter->GetCharacterMovement();
		}
	}
	
}

void UCharacterAnimInstance::CalculateSpeed()
{
	Speed = CharacterMovementComp->Velocity.Size2D();
}

void UCharacterAnimInstance::CalculateShouldMove()
{
	if (Speed > 3.0f) {
		bShouldMove = true;
	}
	else {
		bShouldMove = false;
	}
}

void UCharacterAnimInstance::CalsulateIsJumping()
{
	if (CharacterMovementComp->IsFalling() && CharacterMovementComp->Velocity.Z > 100.0f) {
		bIsJumping = true;
	}
	else {
		bIsJumping = false;
	}
}

void UCharacterAnimInstance::CalculateIsFalling()
{
	bIsFalling = CharacterMovementComp->IsFalling();
	if (CharacterMovementComp->Velocity.Z == 0.0f) {
		bIsFalling = false;
	}
}

void UCharacterAnimInstance::CalculateIsDashing()
{
	bIsDashing = OwnerCharacter->GetIsDashing();
}
