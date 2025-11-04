#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BTService_DetectPlayer.generated.h"


UCLASS()
class MYPROJECT_API UBTService_DetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_DetectPlayer();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	FBlackboardKeySelector TargetActorKey;

	// 감지 거리 범위
	UPROPERTY(EditAnywhere, Category = "AI")
	float DetectionRadius = 1500.0f;
};
