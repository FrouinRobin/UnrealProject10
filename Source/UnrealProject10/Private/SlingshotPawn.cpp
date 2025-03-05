#include "SlingshotPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

// Constructor
ASlingshotPawn::ASlingshotPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create Projectile Mesh (this will be assigned in Blueprint)
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("MeshAnchor"));
    RootComponent = Anchor;

    bIsAiming = false;
    PullStrength = 0.0f;
}

// Begin Play
void ASlingshotPawn::BeginPlay()
{
    Super::BeginPlay();

    // Store the initial projectile location (so we can reset it)
    InitialProjectileLocation = ProjectileMesh->GetComponentLocation();
}

// Tick (for charge-up effect)
void ASlingshotPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsAiming)
    {
        // Increase pull strength over time
        PullStrength += DeltaTime * 500.0f; // Adjust force multiplier
    }
}

// Input Setup
void ASlingshotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASlingshotPawn::StartAiming);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASlingshotPawn::FireProjectile);
}

// Start Aiming (begin charging the shot)
void ASlingshotPawn::StartAiming()
{
    bIsAiming = true;
    PullStrength = 0.0f;
}

// Fire the projectile
void ASlingshotPawn::FireProjectile()
{
    if (!bIsAiming) return;

    bIsAiming = false;

    // Apply force to projectile
    FVector LaunchDirection = -GetActorForwardVector(); // Adjust based on slingshot rotation
    ProjectileMesh->AddImpulse(LaunchDirection * PullStrength, NAME_None, true);

    // Reset projectile position after 2 seconds (optional)
    FTimerHandle ResetTimer;
    GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this]()
        {
            ProjectileMesh->SetWorldLocation(InitialProjectileLocation);
            ProjectileMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        }, 2.0f, false);
}
