// Fill out your copyright notice in the Description page of Project Settings.


#include "Birds.h"

void IBirds::Init()
{
}

void IBirds::TakeDamage(float DamageAmount)
{
	OnDeath();
}

void IBirds::OnDeath()
{
    AActor* BirdActor = Cast<AActor>(this);
    if (BirdActor)
    {
        BirdActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Default function OnDeath called, Bird is destroyed"));
    }
}
