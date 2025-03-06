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

}

void ARedBird::TakeDamage(float DamageAmount)
{
}

void ARedBird::OnDeath()
{
}

float ARedBird::GetBirdVelocity() const
{
	return IBirds::GetBirdVelocity();
}

float ARedBird::SetBirdVelocity(float NewBirdVelocity)
{
	return IBirds::SetBirdVelocity(NewBirdVelocity);
}

float ARedBird::GetBirdMass() const
{
	return IBirds::GetBirdMass();
}

float ARedBird::SetBirdMass(float NewBirdMass)
{
	return IBirds::SetBirdMass(NewBirdMass);
}

