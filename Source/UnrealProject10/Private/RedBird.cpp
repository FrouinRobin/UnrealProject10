// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBird.h"

// Sets default values
ARedBird::ARedBird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BirdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BirdMesh"));
	RootComponent = BirdMesh; // Définir le mesh comme root component

	//Initiation des physics components
	BirdMesh->SetSimulatePhysics(true);
	BirdMesh->SetEnableGravity(true);
	BirdMesh->SetNotifyRigidBodyCollision(true);

	UPhysicalMaterial* PhysicsMat = NewObject<UPhysicalMaterial>();
	PhysicsMat->Restitution = 0.8f;  // 0 = aucun rebond, 1 = rebond parfait
	PhysicsMat->Friction = 0.5f; // Réduit le frottement pour un rebond plus naturel
	BirdMesh->SetPhysMaterialOverride(PhysicsMat);
}

// Called when the game starts or when spawned
void ARedBird::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

// Called every frame
void ARedBird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARedBird::Init()
{
	SetBirdBounceCount(0);

	if (GetBirdMaxBounceCount() == 0)
	{
		SetBirdMaxBounceCount(3);
	}
	if (GetBirdMass() == 0.0f)
	{
		SetBirdMass(500.0f);
		UE_LOG(LogTemp, Display, TEXT("Using default BirdMass"));
	}
	if (GetBirdVelocity() == 0.0f)
	{
		BirdVelocity = 1000.0f;
		//SetBirdVelocity(GetBirdMass(), );
		UE_LOG(LogTemp, Display, TEXT("Using default BirdVelocity"));
	}
	if (GetBirdDamage() == 0.0f)
	{
		SetBirdDamage(10.0f);
		UE_LOG(LogTemp, Display, TEXT("Using default BirdDamage"));

	}
	if (BirdMaterial)
	{
		BirdMesh->SetMaterial(0, BirdMaterial);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning : BirdMaterial undefined"));
	}

	if (BirdMesh)
	{
		BirdMesh->SetMassOverrideInKg(NAME_None, GetBirdMass());
		BirdMesh->BodyInstance.bOverrideMass = true;
		//BirdMesh->SetPhysicsLinearVelocity(FVector(GetBirdVelocity(), 0.0f, GetBirdVelocity()));
	}
}

void ARedBird::TakeDamage(float DamageAmount)
{
	IBirds::TakeDamage(DamageAmount);
}

void ARedBird::OnDeath()
{
	IBirds::OnDeath();
}

float ARedBird::GetBirdVelocity() const
{
	return BirdVelocity;
}

void ARedBird::SetBirdVelocity(float CurrentBirdMass, float SlightshotPullStrength)
{
	BirdVelocity = SlightshotPullStrength/CurrentBirdMass;
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

int ARedBird::GetBirdMaxBounceCount()
{
	return BirdMaxBounceCount;
}

void ARedBird::SetBirdMaxBounceCount(int NewBirdMaxBounceCount)
{
	BirdMaxBounceCount = NewBirdMaxBounceCount;
}

int ARedBird::GetBirdBounceCount() const
{
	return BirdBounceCount;
}

void ARedBird::SetBirdBounceCount(int NewBirdBounceCount)
{
	BirdBounceCount = NewBirdBounceCount;
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
	//Gestion de l'impact avec le sol
	if (Other && Other->ActorHasTag("Ground"))
	{
		UE_LOG(LogTemp, Display, TEXT("Collision avec le sol effectue"));
		if (GetBirdBounceCount() < GetBirdMaxBounceCount())
		{
			UPhysicalMaterial* PhysicsMat = BirdMesh->GetBodyInstance()->GetSimplePhysicalMaterial();
			FVector CurrentVelocity = BirdMesh->GetPhysicsLinearVelocity();
			FVector BounceVelocity = FVector(CurrentVelocity.X, CurrentVelocity.Y, FMath::Abs(CurrentVelocity.Z) * PhysicsMat->Restitution);
			BirdMesh->SetPhysicsLinearVelocity(BounceVelocity);
			SetBirdBounceCount(GetBirdBounceCount() +1 );
			UE_LOG(LogTemp, Display, TEXT("Rebond effectue %i"), GetBirdBounceCount());
		} 
		else
		{
			BirdMesh->SetSimulatePhysics(false);
			OnDeath();
			UE_LOG(LogTemp, Display, TEXT("Desactivation de la physique"));
		}
	}
}
