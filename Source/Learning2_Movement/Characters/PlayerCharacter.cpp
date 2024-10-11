// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// クラスコンポーネントをインスタンス化する
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmcomp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	// モーションワープも試したいのでそちらもインスタンス化する
	MotionWarpingComp = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComp"));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// SpringArmの設定 デフォルトの位置だとカメラが埋もれてしまうので、SetRelativeLocation()で調整する
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
		// どちらが前方かを見つけます
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 前方ベクターを見つけます
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		// どちらが右側かを見付けます
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// その方向に移動を追加します
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

		// 前方ベクトルにLaunchPowerを掛けて速度ベクトルを作成
		FVector LaunchVelocity = ForwardDirection * 1000.0f;
		LaunchVelocity.Z = -5000.0f;
		// キャラクターを指定した方向に打ち上げる
		LaunchCharacter(LaunchVelocity, true, true);
		if (GEngine)
		{
			FString RotationString = ForwardDirection.ToString();  // FRotatorを文字列に変換
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Rotation: %s"), *RotationString));
			FString RotationStrings = LaunchVelocity.ToString();  // FRotatorを文字列に変換
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

