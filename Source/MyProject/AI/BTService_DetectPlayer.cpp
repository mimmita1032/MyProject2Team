#include "MyProject/AI/BTService_DetectPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h" 
#include "AIController.h"

UBTService_DetectPlayer::UBTService_DetectPlayer()
{
	NodeName = TEXT("Detect Player");
	// 실행 텀 시간
	Interval = 0.1f; 
	
	TargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_DetectPlayer, TargetActorKey), APawn::StaticClass());
}

void UBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn)
	{
		// 거리 계산
		float Distance = FVector::Dist(PlayerPawn->GetActorLocation(), AIPawn->GetActorLocation());

		if (Distance <= DetectionRadius)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsObject(TargetActorKey.SelectedKeyName, PlayerPawn);
		}
		else
		{
			OwnerComp.GetBlackboardComponent()->ClearValue(TargetActorKey.SelectedKeyName);
		}
	}
}
