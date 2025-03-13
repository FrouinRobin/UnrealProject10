// Fill out your copyright notice in the Description page of Project Settings.


#include "Pigs.h"


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
        PigActor->Destroy();
        UE_LOG(LogTemp, Warning, TEXT("Default function OnDeath called, Pig is destroyed"));
    }
}
