// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

// Sets default values
ADestructibleWall::ADestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	// Active la simulation physique et la destruction
	WallMesh->SetSimulatePhysics(true);
	WallMesh->SetEnableGravity(true);
	WallMesh->SetNotifyRigidBodyCollision(true);
	WallMesh->SetCollisionProfileName("PhysicsActor");
}

// Called when the game starts or when spawned
void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADestructibleWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Init();
}

void ADestructibleWall::Init()
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

void ADestructibleWall::OnHitByBird()
{
	if (WallMesh)
	{
		UE_LOG(LogTemp, Display, TEXT("Wall hit by bird! Fracturing..."));
	}
}

void ADestructibleWall::OnHitByPig()
{
	if (WallMesh)
	{
		UE_LOG(LogTemp, Display, TEXT("Wall hit by pig! Fracturing..."));
	}
}

float ADestructibleWall::GetWallMass() const
{
	return WallMass;
}

void ADestructibleWall::SetWallMass(float NewWallMass)
{
	WallMass = NewWallMass;
}

