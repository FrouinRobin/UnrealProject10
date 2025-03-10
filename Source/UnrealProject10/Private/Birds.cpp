// Fill out your copyright notice in the Description page of Project Settings.


#include "Birds.h"

// Add default functionality here for any IBirds functions that are not pure virtual.

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
        // Si c'est un acteur, appeler Destroy() pour le supprimer
        BirdActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Default function OnDeath called, Bird is destroyed"));
    }
}
