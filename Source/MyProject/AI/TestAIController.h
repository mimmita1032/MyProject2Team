#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TestAIController.generated.h"

class UBehaviorTree;
class UBlackboardData;

UCLASS()
class MYPROJECT_API ATestAIController : public AAIController
{
	GENERATED_BODY()

public:
	ATestAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBlackboardData> BlackboardAsset;
};
