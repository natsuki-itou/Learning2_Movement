// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �N���X�R���|�[�l���g���C���X�^���X������
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmcomp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	// ���[�V�������[�v�����������̂ł�������C���X�^���X������
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// SpringArm�̐ݒ� �f�t�H���g�̈ʒu���ƃJ������������Ă��܂��̂ŁASetRelativeLocation()�Œ�������
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SpringArmComp->SetupAttachment(GetMesh());
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::MoveForward(float AxisValue)

{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// �ǂ��炪�O�����������܂�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// �O���x�N�^�[�������܂�
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// �ǂ��炪�E���������t���܂�
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// ���̕����Ɉړ���ǉ����܂�
		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void APlayerCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void APlayerCharacter::Jump()
{
	if (JumpCount < MaxJumpCount) {
		LaunchCharacter(FVector(0.0f, 0.0f, 500.0f), false, true);
		JumpCount++;
	}
}

void APlayerCharacter::Dive()
{
	if (GetCharacterMovement()->IsFalling()) {
		//GetCharacterMovement()->GravityScale = 100.0f;
		FVector ForwardDirection = GetActorForwardVector();

		// �O���x�N�g����LaunchPower���|���đ��x�x�N�g�����쐬
		FVector LaunchVelocity = ForwardDirection * 1000.0f;
		LaunchVelocity.Z = -5000.0f;
		// �L�����N�^�[���w�肵�������ɑł��グ��
		LaunchCharacter(LaunchVelocity, true, true);
		if (GEngine)
		{
			FString RotationString = ForwardDirection.ToString();  // FRotator�𕶎���ɕϊ�
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotation: %s"), *RotationString));
			FString RotationStrings = LaunchVelocity.ToString();  // FRotator�𕶎���ɕϊ�
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotation: %s"), *RotationStrings));
		}
		//LaunchCharacter(FVector(0.0f, 0.0f, -500.0f), false, true);
		//LaunchCharacter(FVector(1000.0f, 0.0f, -500.0f), false, true);
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	GetCharacterMovement()->GravityScale = 1.0f;
	JumpCount = 0;
}


// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprint);
	PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &APlayerCharacter::Dive);
}

