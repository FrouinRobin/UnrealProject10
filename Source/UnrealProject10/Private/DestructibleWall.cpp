// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleWall.h"

// Sets default values
ADestructibleWall::ADestructibleWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	//RootComponent = WallMesh; // Définir le mesh comme root component
	//
	////Initiation des physics components
	//WallMesh->SetSimulatePhysics(true);
	//WallMesh->SetEnableGravity(true);
	//WallMesh->SetNotifyRigidBodyCollision(true);

	WallMesh2 = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("WallMesh2"));
	RootComponent = WallMesh2;

	// Active la simulation physique et la destruction
	WallMesh2->SetSimulatePhysics(true);
	WallMesh2->SetEnableGravity(true);
	WallMesh2->SetNotifyRigidBodyCollision(true);
	WallMesh2->SetCollisionProfileName("PhysicsActor");
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
		WallMesh2->SetMaterial(0, WallMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WallMaterial non reference"));
	}
}

void ADestructibleWall::OnHitByBird()
{
	if (WallMesh2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wall hit by bird! Fracturing..."));
	}
}

void ADestructibleWall::OnHitByPig()
{
	//Destroy();
	if (WallMesh2)
	{
		UE_LOG(LogTemp, Warning, TEXT("Wall hit by pig! Fracturing..."));
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

