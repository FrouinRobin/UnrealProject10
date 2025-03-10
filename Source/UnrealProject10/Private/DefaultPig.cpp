// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPig.h"

// Sets default values
ADefaultPig::ADefaultPig()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PigMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PigMesh"));
	RootComponent = PigMesh; // Définir le mesh comme root component

	//Initiation des physics components
	PigMesh->SetSimulatePhysics(true);
	PigMesh->SetEnableGravity(true);
	PigMesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ADefaultPig::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void ADefaultPig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADefaultPig::Init()
{
	if (GetPigMass() == 0.0f)
	{
		SetPigMass(500.0f);
	}
	if (GetPigHealth() == 0.0f)
	{
		SetPigHealth(3.0f);
	}
	if (GetPigDamage() == 0.0f)
	{
		SetPigDamage(1.0f);
	}

	if (PigMaterial)
	{
		PigMesh->SetMaterial(0, PigMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PigMaterial non reference"));
	}
}

void ADefaultPig::TakeDamage(float DamageAmount)
{
	if (GetPigHealth() <= 0)
	{
		OnDeath();
	}
	else
	{
		SetPigHealth(GetPigHealth() - DamageAmount);
	}
}

void ADefaultPig::OnDeath()
{
	IPigs::OnDeath();
}

float ADefaultPig::GetPigMass() const
{
	return PigMass;
}

void ADefaultPig::SetPigMass(float NewPigMass)
{
	PigMass = NewPigMass;
}

float ADefaultPig::GetPigHealth() const
{
	return PigHealth;
}

void ADefaultPig::SetPigHealth(float NewPigHealth)
{
	PigHealth = NewPigHealth;
}

float ADefaultPig::GetPigDamage() const
{
	return PigDamage;
}

void ADefaultPig::SetPigDamage(float NewPigDamage)
{
	PigDamage = NewPigDamage;
}

void ADefaultPig::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	//Gestion de l'impact avec un cochon
	if (Other && Other->Implements<UPigs>())
	{
		IPigs* Pig = Cast<IPigs>(Other);
		if (Pig)
		{
			this->TakeDamage(GetPigDamage());
			Pig->TakeDamage(GetPigDamage());
		}
	}
	//Gestion de l'impact avec un obstacle
	if (Other && Other->Implements<UObstacles>())
	{
		IObstacles* Obstacle = Cast<IObstacles>(Other);
		if (Obstacle)
		{
			Obstacle->OnHitByPig();
		}
	}
	//Gestion de l'impact avec le sol
	if (Other && Other->ActorHasTag("Ground"))
	{
		this->TakeDamage(GetPigDamage());
	}
}

