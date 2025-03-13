// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	FieldSystem->SetupAttachment(RootComponent);

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
		SetBarrelExplosionStrenght(1.0f);
	}
	if (GetBarrelExplosionRadius() == 0.0f)
	{
		SetBarrelExplosionRadius(1.0f);
	}

	if (BarrelMaterial)
	{
		BarrelMesh->SetMaterial(0, BarrelMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : BarrelMaterial undefined"));
	}
}

void AExplosiveBarrel::OnHitByBird()
{
	UE_LOG(LogTemp, Display, TEXT("ExplosiveBarrel: Hit by bird, exploding"));

	Explode(GetBarrelExplosionStrenght(), GetBarrelExplosionRadius());
}

void AExplosiveBarrel::OnHitByPig()
{
	UE_LOG(LogTemp, Display, TEXT("ExplosiveBarrel: Hit by pig, exploding"));

	Explode(GetBarrelExplosionStrenght(), GetBarrelExplosionRadius());
}

void AExplosiveBarrel::Explode(float ExplosionStrength, float ExplosionRadius)
{
	// Création d’un Radial Vector Field pour appliquer une force d’explosion
	URadialVector* RadialVector = NewObject<URadialVector>();
	RadialVector->Magnitude = ExplosionStrength;  // Force de l'explosion
	RadialVector->Position = GetActorLocation();
	
	//Création d'un Radial Fall Off pour atténuer la force de l'explosion
	URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
	RadialFalloff->Magnitude = ExplosionStrength;
	RadialFalloff->Radius = ExplosionRadius;
	RadialFalloff->Position = GetActorLocation();
	//RadialFalloff->Falloff = EFieldFalloffType::Field_Falloff_Linear;

	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, RadialVector );

	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), ExplosionRadius, 50, FLinearColor::Red, 5.0f, 0.0f);

	// Debug Draw : Dessiner la force de l'explosion (une sphère représentant la force)
	// Utilisation d'une sphère pour représenter l'effet de l'explosion à une certaine intensité
	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), ExplosionStrength, 12, FLinearColor::Blue, 5.0f, 0.0f);
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

