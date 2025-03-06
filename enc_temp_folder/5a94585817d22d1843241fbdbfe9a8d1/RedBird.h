// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Birds.h"
#include "RedBird.generated.h"

UCLASS()
class UNREALPROJECT10_API ARedBird : public AActor, public IBirds
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARedBird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void TakeDamage(float DamageAmount) override;

	virtual void OnDeath() override;

	virtual float GetBirdVelocity() const override;

	virtual float SetBirdVelocity(float NewBirdVelocity) override;

	virtual float GetBirdMass() const override;

	virtual float SetBirdMass(float NewBirdMass) override;
};
