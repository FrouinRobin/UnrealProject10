// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBird.h"

// Sets default values
ARedBird::ARedBird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMesh"));
	RootComponent = BirdMesh; // Définir le mesh comme root component
}

// Called when the game starts or when spawned
void ARedBird::BeginPlay()
{
	Super::BeginPlay();
	Init();
	UE_LOG(LogTemp, Warning, TEXT("BirdMass : %f, BirdVelocity : %f"), BirdMass, BirdVelocity);
}

// Called every frame
void ARedBird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARedBird::Init()
{
	if (GetBirdMass() == 0.0f)
	{
		SetBirdMass(500.0f);
	}
	if (GetBirdVelocity() == 0.0f)
	{
		SetBirdVelocity(1000.0f);
	}
	if (GetBirdDamage() == 0.0f)
	{
		SetBirdDamage(10.0f);
	}
}

void ARedBird::TakeDamage(float DamageAmount)
{
	IBirds::TakeDamage(DamageAmount);
}

void ARedBird::OnDeath()
{
}

float ARedBird::GetBirdVelocity() const
{
	return BirdVelocity;
}

void ARedBird::SetBirdVelocity(float NewBirdVelocity)
{
	BirdVelocity = NewBirdVelocity;
}

float ARedBird::GetBirdMass() const
{
	return BirdMass;
}

void ARedBird::SetBirdMass(float NewBirdMass)
{
	BirdMass = NewBirdMass;
}

float ARedBird::GetBirdDamage() const
{
	return BirdDamage;
}

void ARedBird::SetBirdDamage(float NewBirdDamage)
{
	BirdDamage = NewBirdDamage;
}

void ARedBird::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//Gestion de l'impact avec un cochon
	if (Other && Other->Implements<UPigs>())
	{
		IPigs* Pig = Cast<IPigs>(Other);
		if (Pig) 
		{
			Pig->TakeDamage(GetBirdDamage());
		}
	}
	//Gestion de l'impact avec un obstacle
	if (Other && Other->Implements<UObstacles>())
	{
		IObstacles* Obstacle = Cast<IObstacles>(Other);
		if (Obstacle)
		{
			Obstacle->OnHitByBird();
		}
	}
}
