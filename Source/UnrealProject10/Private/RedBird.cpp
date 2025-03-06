// Fill out your copyright notice in the Description page of Project Settings.


#include "RedBird.h"

// Sets default values
ARedBird::ARedBird()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
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
	BirdMass = 500.0f;
	BirdVelocity = 1000.0f;
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