// Fill out your copyright notice in the Description page of Project Settings.


#include "Pigs.h"

// Add default functionality here for any IPigs functions that are not pure virtual.

void IPigs::Init()
{
}

void IPigs::TakeDamage(float DamageAmount)
{
}

void IPigs::OnDeath()
{
    AActor* PigActor = Cast<AActor>(this);
    if (PigActor)
    {
        // Si c'est un acteur, appeler Destroy() pour le supprimer
        PigActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Default function OnDeath called, Pig is destroyed"));
    }
}
