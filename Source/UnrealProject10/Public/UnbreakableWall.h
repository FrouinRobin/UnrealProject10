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
	UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	class UStaticMeshComponent* WallMesh;
	UPROPERTY(EditAnywhere, Category = "Wall Appearance")
	class UMaterialInterface* WallMaterial;

public:	
	AUnbreakableWall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

	virtual void OnHitByBird() override;

	virtual void OnHitByPig() override;

};
