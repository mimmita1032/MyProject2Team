// Copyright Epic Games, Inc. All Rights Reserved.

#include "Test_Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateTestCharacter);

//////////////////////////////////////////////////////////////////////////
// ATest_Character

ATest_Character::ATest_Character()
{
	bReplicates = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->SetIsReplicated(true);
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 크라우치 설정
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	//GetCharacterMovement()->CrouchedHalfHeight = 48.0f;  // 앉았을 때 캡슐 높이 (기본 96의 절반)
	//GetCharacterMovement()->MaxWalkSpeedCrouched = 250.f;  // 앉았을 때 이동 속도

	// Create a camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// bWantsToCrouch는 헤더에서 비트필드로 선언되어 기본값 0 (false)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATest_Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATest_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATest_Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATest_Character::Look);

		// Crouching - Started 이벤트로 토글
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ThisClass::ToggleCrouch);
	}
	else
	{
		UE_LOG(LogTemplateTestCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void ATest_Character::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// bWantsToCrouch를 모든 클라이언트에 복제
	DOREPLIFETIME(ThisClass, bWantsToCrouch);
}

//////////////////////////////////////////////////////////////////////////
// Movement Functions

void ATest_Character::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATest_Character::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATest_Character::OnInteract(const FInputActionValue& Value)
{
	// 상호작용 로직 구현
}

//////////////////////////////////////////////////////////////////////////
// Crouch Functions

void ATest_Character::ToggleCrouch(const FInputActionValue& Value)
{
	// 클라이언트인 경우 서버에 요청
	if (!HasAuthority())
	{
		Server_ToggleCrouch();
		
		// 로컬 예측을 위해 즉시 실행 (선택사항)
		// 네트워크 지연을 줄이고 반응성을 높임
		bWantsToCrouch = !bWantsToCrouch;
		HandleCrouchToggle();
	}
	else
	{
		// 서버 또는 리슨 서버: 직접 처리
		bWantsToCrouch = !bWantsToCrouch;
		HandleCrouchToggle();
	}
}

void ATest_Character::Server_ToggleCrouch_Implementation()
{
	// 서버에서 유효성 검증
	if (!IsValid(this) || !GetCharacterMovement())
	{
		UE_LOG(LogTemp, Error, TEXT("[Server] Invalid character or movement component"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Server] ToggleCrouch - Player: %s, Current bWantsToCrouch: %d"), 
		*GetName(), bWantsToCrouch);
	
	// 서버에서 상태 토글
	bWantsToCrouch = !bWantsToCrouch;
	
	// 서버에서 크라우치 실행
	HandleCrouchToggle();
	
	// bWantsToCrouch가 Replicated이므로 자동으로 모든 클라이언트에 전파됨
	// 그러면 각 클라이언트에서 OnRep_WantsToCrouch()가 호출됨
}

void ATest_Character::OnRep_WantsToCrouch()
{
	// 클라이언트에서 복제된 값이 변경되었을 때 호출
	UE_LOG(LogTemp, Log, TEXT("[Client] OnRep_WantsToCrouch - Player: %s, bWantsToCrouch: %d, IsCrouching: %d"), 
		*GetName(), bWantsToCrouch, GetCharacterMovement()->IsCrouching());
	
	// 복제된 상태에 따라 크라우치 실행
	HandleCrouchToggle();
}

void ATest_Character::HandleCrouchToggle()
{
	if (!GetCharacterMovement())
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] CharacterMovement is null - Player: %s"), 
			HasAuthority() ? TEXT("Server") : TEXT("Client"), *GetName());
		return;
	}

	const bool bIsCrouching = GetCharacterMovement()->IsCrouching();
	
	if (bWantsToCrouch)
	{
		// 앉기 시도
		if (!bIsCrouching)
		{
			Crouch();
			UE_LOG(LogTemp, Log, TEXT("[%s] Crouch() called - Player: %s"), 
				HasAuthority() ? TEXT("Server") : TEXT("Client"), *GetName());
		}
	}
	else
	{
		// 일어서기 시도
		if (bIsCrouching)
		{
			UnCrouch();
			UE_LOG(LogTemp, Log, TEXT("[%s] UnCrouch() called - Player: %s"), 
				HasAuthority() ? TEXT("Server") : TEXT("Client"), *GetName());
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation Functions

void ATest_Character::Multicast_PlayMontage_Implementation(UAnimMontage* Montage, float PlayRate)
{
	// 몽타주 재생 로직 구현
	if (Montage && GetMesh() && GetMesh()->GetAnimInstance())
	{
		GetMesh()->GetAnimInstance()->Montage_Play(Montage, PlayRate);
	}
}