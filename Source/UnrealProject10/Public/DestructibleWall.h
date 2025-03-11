// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Obstacles.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Chaos/ChaosGameplayEventDispatcher.h"
#include "DestructibleWall.generated.h"

UCLASS()
class UNREALPROJECT10_API ADestructibleWall : public AActor, public IObstacles
{
	GENERATED_BODY()
protected:
	// Composant de mesh pour l'apparence du cochon (déclaré mais pas défini ici)
	//UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	//class UStaticMeshComponent* WallMesh;
	UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	class UMaterialInterface* WallMaterial;

	UPROPERTY(EditAnywhere, Category = "Wall Physics")
	class UGeometryCollectionComponent* WallMesh2;

	UPROPERTY(EditAnywhere)
	float WallMass;

public:	
	// Sets default values for this actor's properties
	ADestructibleWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every fra me
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void OnHitByBird() override;

	virtual void OnHitByPig() override;

	float GetWallMass() const;

	void SetWallMass(float NewWallMass);
};
