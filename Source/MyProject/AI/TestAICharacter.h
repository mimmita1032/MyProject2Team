#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "TestAICharacter.generated.h"

UCLASS()
class MYPROJECT_API ATestAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestAICharacter();

	// 상효작용 가능한 상태인지 저장
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	bool bIsPlayerNearby = false;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 상호작용 감지 영역 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractionRange;

protected:
	// 충돌 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// 상체(Upper, Suit) 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BodyModule;

	// 머리 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HairModule;

	// 백팩 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BackpackModule;

	// 수염 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BeardModule;

	// 신발 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BootModule;

	// 눈썹 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BrowsModule;

	// 눈 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> EyeModule;

	// 안경 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GlassModule;

	// 장갑 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GlovesModule;

	// 모자 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> HatModule;

	// 바지 모듈
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Modular", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PantsModule;

	
};
