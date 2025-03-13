// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInterface.h"
#include "Birds.h"
#include "Pigs.h"
#include "Obstacles.h"
#include "RedBird.generated.h"

UCLASS()
class UNREALPROJECT10_API ARedBird : public AActor, public IBirds
{
	GENERATED_BODY()
	
public:	
	ARedBird();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere);
	float BirdMass;
	UPROPERTY(EditAnywhere);
	float BirdVelocity;
	UPROPERTY(EditAnywhere);
	float BirdDamage;
	UPROPERTY(EditAnywhere);
	int BirdMaxBounceCount;
	int BirdBounceCount;

	UPROPERTY(EditAnywhere, Category = "Bird Appearance")
	class UStaticMeshComponent* BirdMesh;
	UPROPERTY(EditAnywhere, Category = "Bird Appearance")
	class UMaterialInterface* BirdMaterial;
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void TakeDamage(float DamageAmount) override;

	virtual void OnDeath() override;

	float GetBirdVelocity() const;

	void SetBirdVelocity(float CurrentBirdMass, float SlightshotPullStrength);

	float GetBirdMass() const;

	void SetBirdMass(float NewBirdMass);

	float GetBirdDamage() const;

	void SetBirdDamage(float NewBirdDamage);

	int GetBirdMaxBounceCount();

	void SetBirdMaxBounceCount(int NewBirdMaxBounceCount);

	int GetBirdBounceCount() const;

	void SetBirdBounceCount(int NewBirdBounceCount);

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

};