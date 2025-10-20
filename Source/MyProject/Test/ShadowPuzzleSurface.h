#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShadowPuzzleSurface.generated.h"

class UStaticMeshComponent;
class UDirectionalLightComponent;
class UPointLightComponent;

UENUM(BlueprintType)
enum class EShadowLightType : uint8
{
    Directional,
    Point
};

// Shadow 커버리지 변화/만족 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShadowCoverageChanged, float, Coverage01);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShadowSatisfied);

UCLASS()
class MYPROJECT_API AShadowPuzzleSurface : public AActor
{
    GENERATED_BODY()

public:
    // 생성자: 컴포넌트 생성 및 기본값 설정
    AShadowPuzzleSurface();

protected:
    // 게임 시작 시 초기화
    virtual void BeginPlay() override;

    // 매 프레임 갱신(샘플링/트레이스는 다음 단계에서 구현)
    virtual void Tick(float DeltaSeconds) override;

public:
    // 퍼즐 받는 면(바닥/벽) 메시. 루트로 사용.
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="ShadowPuzzle", meta=(AllowPrivateAccess="true"))
    TObjectPtr<UStaticMeshComponent> SurfaceMesh;

    // 라이트 타입 선택(방향/포인트)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Light")
    EShadowLightType LightType = EShadowLightType::Directional;

    // Directional 라이트 참조(선택)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Light")
    TObjectPtr<UDirectionalLightComponent> DirectionalLight;

    // Point 라이트 참조(선택)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Light")
    TObjectPtr<UPointLightComponent> PointLight;

    // 샘플 격자 크기(다음 단계에서 사용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Sampling", meta=(ClampMin=1, UIMin=1))
    int32 SamplesX = 16;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Sampling", meta=(ClampMin=1, UIMin=1))
    int32 SamplesY = 16;

    // 업데이트 간격(초). 0이면 매 Tick(다음 단계에서 사용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Performance", meta=(ClampMin=0.0))
    float UpdateInterval = 0.05f;

    // 디버그 드로잉 온/오프(다음 단계에서 사용)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Debug")
    bool bDrawDebug = false;

private:
    // 내부 시간 누적(업데이트 간격 제어용, 다음 단계에서 사용)
    double TimeAccumulator = 0.0;

public:
    //  SurfaceMesh, LightType, DirectionalLight, PointLight, SamplesX, SamplesY, UpdateInterval, bDrawDebug 그대로 유지

    //  트레이스 채널 선택(기본 Visibility)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Trace")
    TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;

    //  임계치: 이 값 이상이면 퍼즐 만족
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Logic", meta=(ClampMin=0.0, ClampMax=1.0))
    float ShadowCoverageThreshold = 0.7f;

    //  현재 커버리지(0~1)
    UPROPERTY(BlueprintReadOnly, Category="ShadowPuzzle|State")
    float CurrentCoverage = 0.0f;

    //  한번 만족하면 유지할지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ShadowPuzzle|Logic")
    bool bLatchOnSatisfied = true;

    //  현재 만족 상태인지
    UPROPERTY(BlueprintReadOnly, Category="ShadowPuzzle|State")
    bool bSatisfied = false;

    //  이벤트 바인딩용
    UPROPERTY(BlueprintAssignable, Category="ShadowPuzzle|Event")
    FOnShadowCoverageChanged OnShadowCoverageChanged;

    UPROPERTY(BlueprintAssignable, Category="ShadowPuzzle|Event")
    FOnShadowSatisfied OnShadowSatisfied;

private:
    //  TimeAccumulator 유지

    //  내부 함수 선언
    void UpdateCoverage(float DeltaSeconds);
    void GetSamplingArea(FVector& Origin, FVector& AxisX, FVector& AxisY, FVector& Normal, float& SizeX, float& SizeY) const;

    //  포인트 라이트용 샘플 판정
    bool ComputeShadowAtPoint_Point(const FVector& WorldPoint) const;

};
