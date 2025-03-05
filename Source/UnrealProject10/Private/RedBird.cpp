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
	
}

// Called every frame
void ARedBird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

