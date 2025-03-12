// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacles.h"
#include "UnbreakableWall.generated.h"

UCLASS()
class UNREALPROJECT10_API AUnbreakableWall : public AActor, public IObstacles
{
	GENERATED_BODY()
	
protected:
	// Composant de mesh pour l'apparence du cochon (déclaré mais pas défini ici)
	UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	class UStaticMeshComponent* WallMesh;
	UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	class UMaterialInterface* WallMaterial;

public:	
	// Sets default values for this actor's properties
	AUnbreakableWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void OnHitByBird() override;

	virtual void OnHitByPig() override;

};
