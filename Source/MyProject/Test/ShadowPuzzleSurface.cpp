#include "ShadowPuzzleSurface.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PointLightComponent.h"


AShadowPuzzleSurface::AShadowPuzzleSurface()
{
	PrimaryActorTick.bCanEverTick = true;

	// 받는 면 메시 컴포넌트 생성 및 루트 설정
	SurfaceMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SurfaceMesh"));
	SetRootComponent(SurfaceMesh);

	// 기본 충돌 정책:
	// 퍼즐 판정은 라인트레이스 기반으로 구현 예정이므로,
	// 이 메시가 트레이스를 막지 않게 하려면 추후 Trace 채널 구성 시
	// SurfaceMesh 프로파일을 별도로 두거나, 필요한 경우 채널 무시에 포함한다.
}

void AShadowPuzzleSurface::BeginPlay()
{
	Super::BeginPlay();

	
	UE_LOG(LogTemp, Warning, TEXT("[ShadowPuzzleSurface] BeginPlay: %s"), *GetName());

	// 초기화 로직은 다음 단계에서 추가(샘플링 영역 계산 등)
}

void AShadowPuzzleSurface::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Verbose, TEXT("[ShadowPuzzleSurface] Tick dt=%.3f"), DeltaSeconds);

	UpdateCoverage(DeltaSeconds);

	// 다음 단계에서 UpdateCoverage(샘플링/트레이스) 구현 예정
	// 여기서는 골격만 유지
}
void AShadowPuzzleSurface::GetSamplingArea(FVector& Origin, FVector& AxisX, FVector& AxisY, FVector& Normal, float& SizeX, float& SizeY) const
{
	if (!SurfaceMesh)
	{
		Origin = FVector::ZeroVector;
		AxisX = FVector::RightVector;
		AxisY = FVector::ForwardVector;
		Normal = FVector::UpVector;
		SizeX = SizeY = 0.f;
		return;
	}

	const FTransform WT = SurfaceMesh->GetComponentTransform();
	const FBoxSphereBounds Bounds = SurfaceMesh->Bounds;

	AxisX = WT.GetUnitAxis(EAxis::X);
	AxisY = WT.GetUnitAxis(EAxis::Y);
	Normal = WT.GetUnitAxis(EAxis::Z);

	const FVector Extent = Bounds.BoxExtent; // 월드 기준
	SizeX = Extent.X * 2.0f;
	SizeY = Extent.Y * 2.0f;

	Origin = Bounds.Origin + Normal * 1.0f; // 표면에서 약간 띄워서 샘플
}

bool AShadowPuzzleSurface::ComputeShadowAtPoint_Point(const FVector& WorldPoint) const
{
	if (!PointLight)
	{
		return false; // 라이트 미지정이면 밝음 취급
	}

	const FVector LightPos = PointLight->GetComponentLocation();
	const FVector Dir = (LightPos - WorldPoint).GetSafeNormal();
	const FVector Start = WorldPoint + Dir * 2.0f; // 표면에서 약간 라이트 쪽으로
	const FVector End   = LightPos;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ShadowTracePoint), true, this);
	Params.AddIgnoredActor(this);
	if (SurfaceMesh)                       // 컴포넌트까지 무시하게 추가
	{                                       
		Params.AddIgnoredComponent(SurfaceMesh);  
	}
	
	const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params);

	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Red : FColor::Green, false, 0.05f, 0, 1.0f);
		DrawDebugPoint(GetWorld(), WorldPoint, 6.0f, bHit ? FColor::Red : FColor::Green, false, 0.05f);
	}

	// 라이트까지 가는 길이 막히면 그림자
	return bHit;
}
void AShadowPuzzleSurface::UpdateCoverage(float DeltaSeconds)
{
	TimeAccumulator += DeltaSeconds;
	if (UpdateInterval > 0.0 && TimeAccumulator < UpdateInterval)
	{
		return;
	}
	TimeAccumulator = 0.0;

	if (!SurfaceMesh || SamplesX <= 0 || SamplesY <= 0)
	{
		return;
	}

	// 이번 단계는 Point Light 전용으로 계산
	FVector Origin, AxisX, AxisY, Normal;
	float SizeX = 0.f, SizeY = 0.f;
	GetSamplingArea(Origin, AxisX, AxisY, Normal, SizeX, SizeY);

	const int32 Total = SamplesX * SamplesY;
	int32 ShadowCount = 0;

	for (int32 iy = 0; iy < SamplesY; ++iy)
	{
		const float ty = (iy + 0.5f) / SamplesY; // 셀 중앙
		for (int32 ix = 0; ix < SamplesX; ++ix)
		{
			const float tx = (ix + 0.5f) / SamplesX;

			const FVector Offset = AxisX * ((tx - 0.5f) * SizeX) + AxisY * ((ty - 0.5f) * SizeY);
			const FVector SamplePoint = Origin + Offset;

			const bool bShadow = ComputeShadowAtPoint_Point(SamplePoint);
			if (bShadow)
			{
				ShadowCount++;
			}
		}
	}

	const float NewCoverage = Total > 0 ? float(ShadowCount) / float(Total) : 0.0f;
	const bool bChanged = !FMath::IsNearlyEqual(NewCoverage, CurrentCoverage, 0.001f);
	CurrentCoverage = NewCoverage;

	if (bChanged)
	{
		OnShadowCoverageChanged.Broadcast(CurrentCoverage);
		// 임시 관찰용 로그
		UE_LOG(LogTemp, Warning, TEXT("[ShadowPuzzleSurface] Coverage=%.2f"), CurrentCoverage);
	}

	if (!bSatisfied && CurrentCoverage >= ShadowCoverageThreshold)
	{
		OnShadowSatisfied.Broadcast();
		UE_LOG(LogTemp, Warning, TEXT("[ShadowPuzzleSurface] Satisfied: threshold=%.2f"), ShadowCoverageThreshold);

		if (bLatchOnSatisfied)
		{
			bSatisfied = true;
		}
	}
}
