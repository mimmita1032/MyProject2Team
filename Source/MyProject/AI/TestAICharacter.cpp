#include "MyProject/AI/TestAICharacter.h"

// Sets default values
ATestAICharacter::ATestAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 몸통 모듈
	BodyModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyModule"));
	if (BodyModule)
	{
		// 기본 메시 컴포넌트에 부착
		BodyModule->SetupAttachment(GetMesh()); 
        
		// 모듈이 기본 메시의 애니메이션을 따라 하도록 설정 
		BodyModule->SetMasterPoseComponent(GetMesh()); 
	}
    
	// 머리 모듈
	HairModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairModule"));
	if (HairModule)
	{
		HairModule->SetupAttachment(GetMesh());
		
		HairModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 백팩 모듈
	BackpackModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackpackModule"));
	if (BackpackModule)
	{
		BackpackModule->SetupAttachment(GetMesh()); 
		
		BackpackModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 수염 모듈
	BeardModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeardModule"));
	if (BeardModule)
	{
		BeardModule->SetupAttachment(GetMesh()); 
		
		BeardModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 신발 모듈
	BootModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BootModule"));
	if (BootModule)
	{
		BootModule->SetupAttachment(GetMesh()); 
		
		BootModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 눈썹 모듈
	BrowsModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BrowsModule"));
	if (BrowsModule)
	{
		BrowsModule->SetupAttachment(GetMesh()); 
		
		BrowsModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 눈 모듈
	EyeModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EyeModule"));
	if (EyeModule)
	{
		EyeModule->SetupAttachment(GetMesh()); 
		
		EyeModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 안경 모듈
	GlassModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlassModule"));
	if (GlassModule)
	{
		GlassModule->SetupAttachment(GetMesh()); 
		
		GlassModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 장갑 모듈
	GlovesModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesModule"));
	if (GlovesModule)
	{
		GlovesModule->SetupAttachment(GetMesh()); 
		
		GlovesModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 모자 모듈
	HatModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HatModule"));
	if (HatModule)
	{
		HatModule->SetupAttachment(GetMesh()); 
		
		HatModule->SetMasterPoseComponent(GetMesh()); 
	}

	// 바지 모듈
	PantsModule = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PantsModule"));
	if (PantsModule)
	{
		PantsModule->SetupAttachment(GetMesh()); 
		
		PantsModule->SetMasterPoseComponent(GetMesh()); 
	}
}

// Called when the game starts or when spawned
void ATestAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

