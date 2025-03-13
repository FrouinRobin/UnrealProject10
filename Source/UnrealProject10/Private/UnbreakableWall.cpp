// Fill out your copyright notice in the Description page of Project Settings.


#include "UnbreakableWall.h"

AUnbreakableWall::AUnbreakableWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh; 

	WallMesh->SetNotifyRigidBodyCollision(true);
}

void AUnbreakableWall::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void AUnbreakableWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUnbreakableWall::Init()
{
	if (WallMaterial)
	{
		WallMesh->SetMaterial(0, WallMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : WallMaterial undefined"));
	}
}

void AUnbreakableWall::OnHitByBird()
{
	if (WallMesh)
	{
		UE_LOG(LogTemp, Display, TEXT("Debug : UnbreakableWall hit by bird ! "));
	}
}

void AUnbreakableWall::OnHitByPig()
{
	if (WallMesh)
	{
		UE_LOG(LogTemp, Display, TEXT("Debug : UnbreakableWall hit by pig ! "));
	}
}

