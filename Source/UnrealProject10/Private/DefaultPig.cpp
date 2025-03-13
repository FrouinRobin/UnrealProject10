// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPig.h"

ADefaultPig::ADefaultPig()
{
	PrimaryActorTick.bCanEverTick = true;

	PigMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PigMesh"));
	RootComponent = PigMesh;

	PigMesh->SetSimulatePhysics(true);
	PigMesh->SetEnableGravity(true);
	PigMesh->SetNotifyRigidBodyCollision(true);
}

void ADefaultPig::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ADefaultPig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPigHealth() <= 0)
	{
		OnDeath();
	}
}

void ADefaultPig::Init()
{
	if (GetPigMass() == 0.0f)
	{
		SetPigMass(5.0f);
	}
	if (GetPigHealth() == 0.0f)
	{
		SetPigHealth(3.0f);
	}
	if (GetPigDamage() == 0.0f)
	{
		SetPigDamage(3.0f);
	}

	if (PigMaterial)
	{
		PigMesh->SetMaterial(0, PigMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : PigMaterial undefined"));
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

	
	if (Other && Other->Implements<UPigs>())
	{
		IPigs* Pig = Cast<IPigs>(Other);
		if (Pig)
		{
			this->TakeDamage(GetPigDamage());
			Pig->TakeDamage(GetPigDamage());
			UE_LOG(LogTemp, Display, TEXT("Pig : Collide with pig"));
		}
	}
	
	if (Other && Other->Implements<UObstacles>())
	{
		IObstacles* Obstacle = Cast<IObstacles>(Other);
		if (Obstacle)
		{
			Obstacle->OnHitByPig();
			UE_LOG(LogTemp, Display, TEXT("Pig : Collide with obstacle"));
		}
	}
	
	if (Other && Other->ActorHasTag("Ground"))
	{
		this->TakeDamage(GetPigDamage());
		UE_LOG(LogTemp, Display, TEXT("Pig : Collide with Ground"));
	}
}

