// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"

ATestGameMode::ATestGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TestCharacter/BP_Test_Character.BP_Test_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
