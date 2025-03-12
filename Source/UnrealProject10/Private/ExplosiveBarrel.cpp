// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;

	// Active la simulation physique et la destruction
	//BarrelMesh->SetSimulatePhysics(true);
	//BarrelMesh->SetEnableGravity(true);
	//BarrelMesh->SetNotifyRigidBodyCollision(true);
	BarrelMesh->SetCollisionProfileName("PhysicsActor");
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBarrel::Init()
{
	if (GetBarrelExplosionStrenght() == 0.0f)
	{
		SetBarrelExplosionStrenght(10000000.0f);
	}
	if (GetBarrelExplosionRadius() == 0.0f)
	{
		SetBarrelExplosionRadius(1000.0f);
	}

	if (BarrelMaterial)
	{
		BarrelMesh->SetMaterial(0, BarrelMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : BarrelMaterial undefined"));
	}
	//Explode(GetBarrelExplosionStrenght(), GetBarrelExplosionRadius());
	//BarrelMesh->ApplyRadialImpulse(GetActorLocation(), 5000.0f, 200.0f, ERadialImpulseFalloff::RIF_Linear, true);
}

void AExplosiveBarrel::OnHitByBird()
{
	Explode(GetBarrelExplosionStrenght(), GetBarrelExplosionRadius());
}

void AExplosiveBarrel::OnHitByPig()
{
	UE_LOG(LogTemp, Display, TEXT("ExplosiveBarrel: Hit by pig, exploding"));

	Explode(GetBarrelExplosionStrenght(), GetBarrelExplosionRadius());
}

void AExplosiveBarrel::Explode(float ExplosionStrength, float ExplosionRadius)
{
	UFieldSystemComponent* FieldSystem = NewObject<UFieldSystemComponent>(BarrelMesh->GetOwner());
	if (FieldSystem)
	{
		FieldSystem->RegisterComponent();
		//BarrelMesh->AddFieldCommand(true, FieldSystem); 
	
		URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
		RadialFalloff->Magnitude = ExplosionStrength;
		RadialFalloff->MinRange = 0.0f;
		RadialFalloff->MaxRange = ExplosionRadius;
		RadialFalloff->Default = 1.0f;
		RadialFalloff->Radius = ExplosionRadius;
		RadialFalloff->Position = GetActorLocation();
	
		//UChaosDestruction* ChaosDestruction = NewObject<UChaosDestruction>(); 
		//ChaosDestruction->AddFieldCommand(EFieldPhysicsType::Field_ExternalClusterStrain, RadialFalloff);
	
		BarrelMesh->ApplyPhysicsField(true, EGeometryCollectionPhysicsTypeEnum::Chaos_ExternalClusterStrain, nullptr, RadialFalloff);
	}
}

void AExplosiveBarrel::SetBarrelExplosionStrenght(float NewBarrelExplosionStrenght)
{
	BarrelExplosionStrenght = NewBarrelExplosionStrenght;
}

float AExplosiveBarrel::GetBarrelExplosionStrenght()
{
	return BarrelExplosionStrenght;
}

void AExplosiveBarrel::SetBarrelExplosionRadius(float NewBarrelExplosionRadius)
{
	BarrelExplosionRadius = NewBarrelExplosionRadius;
}

float AExplosiveBarrel::GetBarrelExplosionRadius()
{
	return BarrelExplosionRadius;
}

