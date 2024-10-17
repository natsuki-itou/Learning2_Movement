// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhanceInputTestCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AEnhanceInputTestCharacter::AEnhanceInputTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// クラスコンポーネントをインスタンス化する モーションワープも試したいのでそちらもインスタンス化する
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmcomp"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));

	// 追加したコンポーネントの設定を行う
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->bUsePawnControlRotation = true;
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// CharacterクラスとCharacterMovementComponentの設定を行う
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, DefaultYaw, 0.0f)));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

}

// Called when the game starts or when spawned
void AEnhanceInputTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

// Called every frame
void AEnhanceInputTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	bCheck = GetCharacterMovement()->IsFalling();
	bCheckOnGround = GetCharacterMovement()->IsMovingOnGround();
	CheckVelocityZ = GetCharacterMovement()->Velocity.Z;
}

// Called to bind functionality to input
void AEnhanceInputTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AEnhanceInputTestCharacter::Move);
	EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Completed, this, &AEnhanceInputTestCharacter::ResetDirection);
	EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Triggered, this, &AEnhanceInputTestCharacter::BeginSprint);
	EnhancedInputComponent->BindAction(DashInputAction , ETriggerEvent::Completed, this, &AEnhanceInputTestCharacter::EndSprint);
	EnhancedInputComponent->BindAction(DashInputAction, ETriggerEvent::Started, this, &AEnhanceInputTestCharacter::Dash);
	EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Started, this, &AEnhanceInputTestCharacter::Jump);
	EnhancedInputComponent->BindAction(DiveInputAction, ETriggerEvent::Started, this, &AEnhanceInputTestCharacter::Dive);
	EnhancedInputComponent->BindAction(CameraMoveInputAction, ETriggerEvent::Triggered, this, &AEnhanceInputTestCharacter::CameraMove);
	EnhancedInputComponent->BindAction(CameraSetForwardInputAction, ETriggerEvent::Triggered, this, &AEnhanceInputTestCharacter::CameraSetForward);
}

bool AEnhanceInputTestCharacter::GetIsDashing()
{
	return bIsDashing;
}

void AEnhanceInputTestCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputVector2D = Value.Get<FVector2D>();
	DirectionalVector2D = Value.Get<FVector2D>();// ここで方向指定を取得しておく
	if ((Controller != nullptr) && GetCharacterMovement()->IsFalling() == false)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(RightDirection, InputVector2D.X);
		AddMovementInput(ForwardDirection, InputVector2D.Y);
	}		
}

void AEnhanceInputTestCharacter::ResetDirection()
{
	// 移動の入力がない時に方向指定を初期化することでニュートラルも実装する
	
	DirectionalVector2D = FVector2D(0, 0);
}

void AEnhanceInputTestCharacter::BeginSprint()
{
	//GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AEnhanceInputTestCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
}

void AEnhanceInputTestCharacter::Dash() 
{
	if (Controller != nullptr) {
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->Velocity = FVector::ZeroVector;
		GetCharacterMovement()->GravityScale = 0.0f;
		bIsDashing = true;
		float DashImpulse = DashDistance / DashFlame;
		FVector ForwardVector;
		FVector TargetDashImpulse;
		FVector Target;
		if (DirectionalVector2D.SizeSquared() > 0.0f) {
			FRotator FirstRotation = FRotator(0.0f, FMath::Atan2(DirectionalVector2D.X, DirectionalVector2D.Y) * 180.0f / PI, 0.0f);
			FRotator ControllerRotation = Controller->GetControlRotation();
			FRotator LastRotation = FRotator(0.0f, FirstRotation.Yaw + ControllerRotation.Yaw, 0.0f);
			SetActorRotation(LastRotation);	
		}
		ForwardVector = GetActorRotation().Vector();
		TargetDashImpulse = FVector(ForwardVector.X * DashImpulse, ForwardVector.Y * DashImpulse, ForwardVector.Z);
		Target = FVector(ForwardVector.X * DashDistance, ForwardVector.Y * DashDistance, ForwardVector.Z);
		Target += GetActorLocation();
		DrawDebugSphere(GetWorld(), Target, 10.0f, 12, FColor::Green, false, 2.0f, 0, 1.0f);
		LaunchCharacter(TargetDashImpulse, true, true);
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnhanceInputTestCharacter::DashEnd, DashFlame, false);
	}
}

void AEnhanceInputTestCharacter::DashEnd()
{
	GetCharacterMovement()->GravityScale = 1.0f;
	bIsDashing = false;
	//GetCharacterMovement()->Velocity.X /= 4;
	//GetCharacterMovement()->Velocity.Y /= 4;
}

void AEnhanceInputTestCharacter::Jump()
{
	if ((Controller != nullptr) && bIsDiveing == false) {
		if (bIsDashing) {
			GetCharacterMovement()->Velocity.X /= 3;
			GetCharacterMovement()->Velocity.Y /= 3;
			bIsDashing = false;
		}
		if (JumpCount < 1) {
			if (DirectionalVector2D.SizeSquared() > 0.0f) {
				FRotator FirstRotation = FRotator(0.0f, FMath::Atan2(DirectionalVector2D.X, DirectionalVector2D.Y) * 180.0f / PI, 0.0f);
				FRotator ControllerRotation = Controller->GetControlRotation();
				FRotator LastRotation = FRotator(0.0f, FirstRotation.Yaw + ControllerRotation.Yaw, 0.0f);
				SetActorRotation(LastRotation);
				FVector ForwardVector = LastRotation.Vector();
				FVector JumpImpulseVector(ForwardVector.X * JumpForwardImpulse, ForwardVector.Y * JumpForwardImpulse, JumpImpulse);
				LaunchCharacter(JumpImpulseVector, false, true);
			}
			else {
				LaunchCharacter(FVector(0.0f, 0.0f, JumpImpulse), false, true);
			}
			JumpCount++;
		}
		else if (JumpCount < MaxJumpCount && GetCharacterMovement()->IsFalling() == true) {
			if (DirectionalVector2D.SizeSquared() > 0.0f) {
				FRotator FirstRotation = FRotator(0.0f, FMath::Atan2(DirectionalVector2D.X, DirectionalVector2D.Y) * 180.0f / PI, 0.0f);
				FRotator ControllerRotation = Controller->GetControlRotation();
				FRotator LastRotation = FRotator(0.0f, FirstRotation.Yaw + ControllerRotation.Yaw, 0.0f);
				SetActorRotation(LastRotation);
				FVector ForwardVector = LastRotation.Vector();
				FVector JumpImpulseVector(ForwardVector.X * AirJumpForwardImpulse, ForwardVector.Y * AirJumpForwardImpulse, JumpImpulse);
				LaunchCharacter(JumpImpulseVector, false, true);
			}
			else {
				LaunchCharacter(FVector(0.0f, 0.0f, JumpImpulse), false, true);
			}
			JumpCount++;
		}
	}
}

void AEnhanceInputTestCharacter::Dive()
{
		if ((Controller != nullptr) && GetCharacterMovement()->IsFalling() == true && bIsDiveing == false) {
			GetCharacterMovement()->GravityScale = DiveGravity;
			bIsDiveing = true;
			if (DirectionalVector2D.SizeSquared() > 0.0f) {
				FRotator FirstRotation = FRotator(0.0f, FMath::Atan2(DirectionalVector2D.X, DirectionalVector2D.Y) * 180.0f / PI, 0.0f);
				FRotator ControllerRotation = Controller->GetControlRotation();
				FRotator LastRotation = FRotator(0.0f, FirstRotation.Yaw + ControllerRotation.Yaw, 0.0f);
				SetActorRotation(LastRotation);
			}
				FVector ForwardVector = GetActorRotation().Vector();
				float DashBonus;
				if (bIsDashing) {
					DashBonus = 50.0f;
					bIsDashing = false;
				}
				else {
					DashBonus = 1.0f;
				}
				FVector DiveImpulseVector(ForwardVector.X * DiveForwardImpulse * DashBonus, ForwardVector.Y * DiveForwardImpulse * DashBonus, DiveImpulse);
				LaunchCharacter(FVector(DiveImpulseVector), true, true);
		}
}

void AEnhanceInputTestCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCharacterMovement()->GravityScale = 1.0f;
	if (bIsDiveing) {
		GetCharacterMovement()->Velocity.X /= 5;
		GetCharacterMovement()->Velocity.Y /= 5;
		bIsDiveing = false;
	}
	JumpCount = 0;
}

void AEnhanceInputTestCharacter::CameraMove(const FInputActionValue& Value)
{
	FVector2D InputVector2D = Value.Get<FVector2D>();
	if ((Controller != nullptr)) {
		AddControllerYawInput(InputVector2D.X);
		AddControllerPitchInput(InputVector2D.Y);
	}
}

void AEnhanceInputTestCharacter::CameraSetForward()
{
	const FRotator CharactorRotation = GetActorRotation();
	if ((Controller != nullptr)) {
		Controller->SetControlRotation(CharactorRotation);
	}
}
