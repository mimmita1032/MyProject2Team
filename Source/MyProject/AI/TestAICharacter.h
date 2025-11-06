#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "TestAICharacter.generated.h"

UCLASS()
class MYPROJECT_API ATestAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATestAICharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
    
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
