// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

ADestructibleWall::ADestructibleWall()
{
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh;

	WallMesh->SetSimulatePhysics(true);
	WallMesh->SetEnableGravity(true);
	WallMesh->SetNotifyRigidBodyCollision(true);
	WallMesh->SetCollisionProfileName("PhysicsActor");
}

void ADestructibleWall::BeginPlay()
{
	Super::BeginPlay();
}

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

