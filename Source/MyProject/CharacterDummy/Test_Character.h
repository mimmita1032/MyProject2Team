#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Test_Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateTestCharacter, Log, All);

UCLASS(config=Game)
class ATest_Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

public:
	ATest_Character();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeed = 600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float WalkSpeed = 300.f;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void OnInteract(const FInputActionValue& Value);
	
	void ToggleCrouch(const FInputActionValue& Value);
	
	void StartSprint(const FInputActionValue& Value);

	void StopSprint(const FInputActionValue& Value);


protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(EditDefaultsOnly, Category = "Interact")
	TObjectPtr<UAnimMontage> InteractMontage;

	UPROPERTY(Replicated)
	bool bIsInteracting = false; // 몽타주 재생 여부

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted); // 몽타주 종료 콜백

	// 입력 잠금 관리
	void SetInputLocked(bool bLocked);
	
private:
	
	FTimerHandle SpeedInterpTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SpeedInterpSpeed = 5.f;
	// Sprint -> Walk 보간
	void InterpSpeed();


#pragma region Replicated

public:
	// ReplicatedUsing을 추가하여 클라이언트에서 변경 감지
	UPROPERTY(ReplicatedUsing = OnRep_WantsToCrouch, VisibleDefaultsOnly, BlueprintReadOnly, Category = "Crouch")
	uint8 bWantsToCrouch : 1;	// 앉기 플래그

	UPROPERTY(ReplicatedUsing = OnRep_IsSprinting ,VisibleDefaultsOnly ,BlueprintReadOnly ,Category = "Sprint")
	uint8 bIsSprinting : 1;		// 달리기 플래그


	
protected:
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayMontage(UAnimMontage* Montage, float PlayRate = 1.f);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StopMontage(UAnimMontage* Montage, float BlendOutTime = 0.2f);

	UFUNCTION(Server, Reliable)
	void ServerRPC_OnInteract();
	
	UFUNCTION(Server, Reliable)
	void ServerRPC_ToggleCrouch();

	UFUNCTION(Server, Reliable)
	void ServerRPC_SetSprinting(bool bNewSprinting);

	// 복제 콜백 함수 추가
	UFUNCTION()
	void OnRep_WantsToCrouch();

	UFUNCTION()
	void OnRep_IsSprinting();

	// 실제 크라우치 로직을 처리하는 헬퍼 함수
	void HandleCrouchToggle();

	void UpdateMovementSpeed();	// 속도 변경
	
#pragma endregion
};