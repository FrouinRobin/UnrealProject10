// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SphereComponent.h>

AExplosiveBarrel::AExplosiveBarrel()
{
	PrimaryActorTick.bCanEverTick = true;

	BarrelMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BarrelMesh"));
	RootComponent = BarrelMesh;

	FieldSystem = CreateDefaultSubobject<UFieldSystemComponent>(TEXT("FieldSystem"));
	FieldSystem->SetupAttachment(RootComponent);

	BarrelMesh->SetCollisionProfileName("PhysicsActor");
}

void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

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
	URadialVector* RadialVector = NewObject<URadialVector>();
	RadialVector->Magnitude = ExplosionStrength;
	RadialVector->Position = GetActorLocation();

	URadialFalloff* RadialFalloff = NewObject<URadialFalloff>();
	RadialFalloff->Magnitude = ExplosionStrength;
	RadialFalloff->Radius = ExplosionRadius;
	RadialFalloff->Position = GetActorLocation();

	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_ExternalClusterStrain, nullptr, RadialFalloff);
	FieldSystem->ApplyPhysicsField(true, EFieldPhysicsType::Field_LinearVelocity, nullptr, RadialVector);
	CreateOverlapSphere(ExplosionRadius);

	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), ExplosionRadius, 50, FLinearColor::Red, 5.0f, 0.0f);
	//UKismetSystemLibrary::DrawDebugSphere(this, GetActorLocation(), ExplosionStrength, 12, FLinearColor::Blue, 5.0f, 0.0f);
}

void AExplosiveBarrel::CreateOverlapSphere(float Radius)
{
	if (!this) return;

	USphereComponent* SphereComponent = NewObject<USphereComponent>(this);
	if (!SphereComponent) return;

	SphereComponent->RegisterComponent();
	SphereComponent->AttachToComponent(this->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	SphereComponent->SetSphereRadius(Radius);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnOverlapBegin);
}

UFUNCTION()
void AExplosiveBarrel::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(PigClass))
	{
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Overlapping with: %s"), *OtherActor->GetName());
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