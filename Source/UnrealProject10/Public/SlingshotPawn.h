// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "RedBird.h"
#include "SlingshotPawn.generated.h"

class UInputAction;
class UInputMappingContext;
class UInputComponent;

UCLASS()
class UNREALPROJECT10_API ASlingshotPawn : public ACharacter
{
	GENERATED_BODY()
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_Pull;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_LookOnX;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* IA_LookOnY;

    UPROPERTY(EditAnywhere, Category = "ToIgnore")
    TArray<AActor*> ActorsToIgnore;

    
public:
    ASlingshotPawn();

protected:
    virtual void BeginPlay() override;

    void SpawnBird();

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    UFUNCTION()
    void AdjustYawRotation(const FInputActionValue& Value);
    UFUNCTION()
    void AdjustPitchRotation(const FInputActionValue& Value);

    UFUNCTION(BlueprintCallable)
    void StartAiming();

    UFUNCTION(BlueprintCallable)
    void FireProjectile();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    bool bIsAiming;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float PullStrength;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
    float BirdImpulse;
    UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "Variables")
    FVector InitialProjectileLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bird")
    ARedBird* LoadedBird;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bird")
    TArray<TSubclassOf<ARedBird>> BirdList;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnchorProjectile")
    USceneComponent* ProjectilAnchor;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnchorProjectile")
    TSubclassOf<AActor> ProjectilePreviewActor;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AnchorProjectile")
    TArray<AActor*> ProjectilePreviewList;

    UFUNCTION(BlueprintImplementableEvent)
    void AttachCable();

    float GetPullStrength();

    void SetPullStrength(float NewStrength);
private:
    UPROPERTY(EditAnywhere, Category = "Components")
    USceneComponent* Anchor;

    
    UPROPERTY(EditAnywhere, Category = "Components")
    UCameraComponent* Camera;


    

   
};
