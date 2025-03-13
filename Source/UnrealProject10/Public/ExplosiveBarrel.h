// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles.h"

#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"

#include "PhysicsEngine/RadialForceComponent.h" 
//#include "Chaos/ChaosSolverActor.h"
//#include "Chaos/ChaosGameplayEventDispatcher.h"

#include "ExplosiveBarrel.generated.h"

UCLASS()
class UNREALPROJECT10_API AExplosiveBarrel : public AActor, public IObstacles
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "Barrel Appearance")
	class UMaterialInterface* BarrelMaterial;

	UPROPERTY(EditAnywhere, Category = "Barrel Physics")
	class UGeometryCollectionComponent* BarrelMesh;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	UFieldSystemComponent* FieldSystem;

	UPROPERTY(EditAnywhere, Category = "Barrel Explosion Values")
	float BarrelExplosionStrenght;
	UPROPERTY(EditAnywhere, Category = "Barrel Explosion Values")
	float BarrelExplosionRadius;

public:	
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void OnHitByBird() override;

	UFUNCTION(CallInEditor, Category = "Explode")
	virtual void OnHitByPig() override;

	UFUNCTION(BlueprintCallable)
	void Explode(float ExplosionStrength, float ExplosionRadius);

	UFUNCTION(BlueprintCallable)
	void SetBarrelExplosionStrenght(float NewBarrelExplosionStrenght);

	UFUNCTION(BlueprintCallable)
	float GetBarrelExplosionStrenght();
	UFUNCTION(BlueprintCallable)
	void SetBarrelExplosionRadius(float NewBarrelExplosionRadius);
	UFUNCTION(BlueprintCallable)
	float GetBarrelExplosionRadius();
};
