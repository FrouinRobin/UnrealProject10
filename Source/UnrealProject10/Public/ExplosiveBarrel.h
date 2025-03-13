// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles.h"
#include "Pigs.h"
#include "Birds.h"
#include "DefaultPig.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/EngineTypes.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemObjects.h"

#include "PhysicsEngine/RadialForceComponent.h" 

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
	AExplosiveBarrel();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	void CreateOverlapSphere(float Radius);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ADefaultPig> PigClass;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);

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
