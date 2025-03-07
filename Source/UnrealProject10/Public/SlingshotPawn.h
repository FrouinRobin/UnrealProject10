// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "SlingshotPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;

UCLASS()
class UNREALPROJECT10_API ASlingshotPawn : public ACharacter
{
	GENERATED_BODY()
    /** Input Mapping Context */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    /** Existing Pull Action (IA_Shoot) */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Pull;

    /** Existing Look Action (IA_Shoot) */
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Look;

    UPROPERTY(EditAnywhere, Category = "ToIgnore")
    TArray<AActor*> ActorsToIgnore;


public:
    ASlingshotPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    UFUNCTION(BlueprintCallable)
    void Look(const FInputActionValue& Value);
    // Functions
    UFUNCTION(BlueprintCallable)
    void StartAiming();

    UFUNCTION(BlueprintCallable)
    void FireProjectile();
    // Aiming & Shooting
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    bool bIsAiming;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float PullStrength;
    UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Variables")
    FVector InitialProjectileLocation;

    float GetPullStrength();


    void SetPullStrength(float NewStrength);
private:
    // Projectile reference (can be set in Blueprint)
    UPROPERTY(EditAnywhere, Category = "Components")
    USceneComponent* Anchor;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(EditAnywhere, Category = "Components")
    UCameraComponent* Camera;


    

   
};
