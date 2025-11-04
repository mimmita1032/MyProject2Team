#include "MyProject/AI/TestAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ATestAIController::ATestAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ATestAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Blackboard 초기화
	if (BlackboardAsset)
	{
		UBlackboardComponent* TempBlackboardComponent = Blackboard.Get(); 

		if (UseBlackboard(BlackboardAsset, TempBlackboardComponent)) 
		{
			Blackboard = TempBlackboardComponent; 
		}
	}
	
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}