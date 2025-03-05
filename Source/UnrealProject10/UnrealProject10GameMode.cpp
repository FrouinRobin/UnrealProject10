// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealProject10GameMode.h"
#include "UnrealProject10Character.h"
#include "UObject/ConstructorHelpers.h"

AUnrealProject10GameMode::AUnrealProject10GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
