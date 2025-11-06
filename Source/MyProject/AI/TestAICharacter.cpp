#include "MyProject/AI/TestAICharacter.h"
#include "GameFramework/PlayerController.h"
#include "MyProject/CharacterDummy/Test_Character.h"

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

	// 1. InteractionRange 컴포넌트 생성 및 설정
	InteractionRange = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionRange"));
	if (InteractionRange)
	{
		// 2. 기본 캐릭터의 RootComponent에 부착 (캐릭터 중앙에 위치)
		InteractionRange->SetupAttachment(RootComponent); 
        
		// 3. 충돌 크기 설정 (예: 반경 200cm)
		InteractionRange->SetSphereRadius(200.0f);
        
		// 4. 충돌 설정: 쿼리 전용, 모든 채널 무시, Pawn 채널만 Overlap
		InteractionRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		InteractionRange->SetCollisionResponseToAllChannels(ECR_Ignore); 
		InteractionRange->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 
        
		// 5. Overlap 이벤트 함수 연결 (이벤트 바인딩)
		InteractionRange->OnComponentBeginOverlap.AddDynamic(this, &ATestAICharacter::OnOverlapBegin);
		InteractionRange->OnComponentEndOverlap.AddDynamic(this, &ATestAICharacter::OnOverlapEnd);
	}
}

// Called when the game starts or when spawned
void ATestAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// 오버랩 시작 시 호출
void ATestAICharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 겹친 액터가 플레이어 캐릭터인지 확인
	if (OtherActor && OtherActor != this && Cast<APlayerController>(OtherActor->GetInstigatorController()))
	{
		bIsPlayerNearby = true;
        
		// 상호작용 시작 확인
		UE_LOG(LogTemp, Warning, TEXT("AI: 플레이어 근처에 진입. 상호작용 준비 완료."));
        
		// UI에 "E키를 눌러 대화" 메시지를 표시하는 로직 추가 (추후 블루프린트에서 구현)
	}
}

// 오버랩 종료 시 호출
void ATestAICharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 겹침이 끝난 액터가 플레이어 캐릭터인지 확인
	if (OtherActor && OtherActor != this && Cast<APlayerController>(OtherActor->GetInstigatorController()))
	{
		bIsPlayerNearby = false;

		// 상호작용 종료 확인
		UE_LOG(LogTemp, Warning, TEXT("AI: 플레이어 영역 이탈. 상호작용 불가능."));
        
		// UI의 "E키를 눌러 대화" 메시지를 숨기는 로직 추가 (추후 블루프린트에서 구현)
	}
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

