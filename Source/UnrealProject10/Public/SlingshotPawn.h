// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlingshotPawn.generated.h"

UCLASS()
class UNREALPROJECT10_API ASlingshotPawn : public APawn
{
	GENERATED_BODY()

public:
    ASlingshotPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    // Projectile reference (can be set in Blueprint)
    UPROPERTY(EditAnywhere, Category = "Components")
    USceneComponent* Anchor;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* ProjectileMesh;

    // Aiming & Shooting
    bool bIsAiming;
    float PullStrength;
    FVector InitialProjectileLocation;

    // Functions
    void StartAiming();
    void ChargeShot();
    void FireProjectile();
};
