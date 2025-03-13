// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "Birds.h"
#include "Pigs.h"
#include "Obstacles.h"
#include "DefaultPig.generated.h"

UCLASS()
class UNREALPROJECT10_API ADefaultPig : public AActor, public IPigs
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Pig Appearance")
	class UStaticMeshComponent* PigMesh;
	UPROPERTY(EditAnywhere, Category = "Pig Appearance")
	class UMaterialInterface* PigMaterial;

	UPROPERTY(EditAnywhere);
	float PigMass;
	UPROPERTY(EditAnywhere);
	float PigDamage;
	UPROPERTY(EditAnywhere);
	float PigHealth;
	
public:	
	ADefaultPig();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void TakeDamage(float DamageAmount) override;

	virtual void OnDeath() override;

	float GetPigMass() const;

	void SetPigMass(float NewPigMass);

	float GetPigHealth() const;

	void SetPigHealth(float NewPigHealth);	
	
	float GetPigDamage() const;

	void SetPigDamage(float NewPigDamage);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
};
