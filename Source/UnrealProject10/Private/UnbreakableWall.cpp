// Fill out your copyright notice in the Description page of Project Settings.


#include "UnbreakableWall.h"

// Sets default values
AUnbreakableWall::AUnbreakableWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh; // Définir le mesh comme root component

	////Initiation des physics components
	WallMesh->SetSimulatePhysics(true);
	WallMesh->SetEnableGravity(true);
	WallMesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AUnbreakableWall::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
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

