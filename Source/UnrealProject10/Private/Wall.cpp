// Fill out your copyright notice in the Description page of Project Settings.


#include "Wall.h"

// Sets default values
AWall::AWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallMesh"));
	RootComponent = WallMesh; // Définir le mesh comme root component

	//Initiation des physics components
	WallMesh->SetSimulatePhysics(true);
	WallMesh->SetEnableGravity(true);
	WallMesh->SetNotifyRigidBodyCollision(true);

	GeoCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeoCollection"));
	GeoCollection->SetupAttachment(RootComponent);
	GeoCollection->SetVisibility(false);
	GeoCollection->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AWall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Init();
}

void AWall::Init()
{
	if (WallMaterial)
	{
		WallMesh->SetMaterial(0, WallMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WallMaterial non reference"));
	}
}

void AWall::OnHitByBird()
{
	if (WallMaterial)
	{
		WallMesh->SetMaterial(0, WallMaterial2);
		UE_LOG(LogTemp, Warning, TEXT("Changed WallMaterial, got hit by bird"));
	}
}

void AWall::OnHitByPig()
{
	Destroy();
}

float AWall::GetWallMass() const
{
	return WallMass;
}

void AWall::SetWallMass(float NewWallMass)
{
	WallMass = NewWallMass;
}

