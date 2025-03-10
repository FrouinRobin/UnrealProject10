#include "SlingshotPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"





// Constructor
ASlingshotPawn::ASlingshotPawn()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create Projectile Mesh (this will be assigned in Blueprint)
    
    Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("MeshAnchor"));
    RootComponent = Anchor;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);  // Attach the camera to the root component
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));  // Position the camera
    Camera->bUsePawnControlRotation = true;  // Rotate the camera with pawn control
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    ProjectileMesh->SetupAttachment(Camera);

    bIsAiming = false;
    PullStrength = 0.0f;
}

// Begin Play
void ASlingshotPawn::BeginPlay()
{
    Super::BeginPlay();

    // Store the initial projectile location (so we can reset it)
        InitialProjectileLocation = ProjectileMesh->GetComponentLocation();
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

// Tick (for charge-up effect)
void ASlingshotPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Input Setup
void ASlingshotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind IA_Shoot to FireProjectile function
        EnhancedInput->BindAction(IA_Pull, ETriggerEvent::Triggered, this, &ASlingshotPawn::StartAiming);
        EnhancedInput->BindAction(IA_Pull, ETriggerEvent::Completed, this, &ASlingshotPawn::FireProjectile);
        EnhancedInput->BindAction(IA_LookOnX, ETriggerEvent::Triggered, this, &ASlingshotPawn::Look);
        EnhancedInput->BindAction(IA_LookOnY, ETriggerEvent::Triggered, this, &ASlingshotPawn::AdjustProjectile);

    }
}

void ASlingshotPawn::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Apply sensitivity scaling to the X and Y input
        float SensitivityX = 0.2f;
        float SensitivityY = 0.5f;

        // Add input with sensitivity scaling
        AddControllerYawInput(LookAxisVector.X * SensitivityX);
        AddControllerPitchInput(LookAxisVector.Y * SensitivityY);

        // Get current yaw rotation and clamp it
        FRotator CurrentRotation = GetActorRotation();
        float ClampedYaw = FMath::Clamp(CurrentRotation.Yaw, -45.0f, 45.0f);  // Constrain yaw to [-45°, 45°]
        SetActorRotation(FRotator(CurrentRotation.Pitch, ClampedYaw, CurrentRotation.Roll));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Controller is nullptr. Can't process look input!"));
    }
}



// Start Aiming (begin charging the shot)
void ASlingshotPawn::StartAiming()
{
    bIsAiming = true;

    if (bIsAiming)
    {
        // Increase pull strength over time
        float NewPullStrength = GetPullStrength();
        NewPullStrength += GetWorld()->GetDeltaSeconds() * 500.f;
        SetPullStrength(NewPullStrength);
        UE_LOG(LogTemp, Warning, TEXT("Aiming - PullStrength: %f"), GetPullStrength());

        FPredictProjectilePathParams PathParams;
        PathParams.StartLocation = ProjectileMesh->GetComponentLocation(); // Start from the camera position
        PathParams.LaunchVelocity = ProjectileMesh->GetForwardVector() * GetPullStrength(); // Example velocity
        PathParams.bTraceWithCollision = true;
        PathParams.ProjectileRadius = 5.0f;
        PathParams.bTraceWithChannel = true;
        PathParams.TraceChannel = ECC_Visibility;
        PathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
        PathParams.SimFrequency = 25.f;

        FPredictProjectilePathResult PathResult;
        bool bSuccess = UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult);

        if (bSuccess)
        {
            // Log the predicted path result
            UE_LOG(LogTemp, Warning, TEXT("Predicted %d points in the path"), PathResult.PathData.Num());

            // Draw the predicted path using debug spheres
            for (const FPredictProjectilePathPointData& Point : PathResult.PathData)
            {
                DrawDebugSphere(GetWorld(), Point.Location, 5.0f, 12, FColor::Green, false);
            }

            if (PathResult.HitResult.bBlockingHit) // Check if the projectile hit something
            {
                DrawDebugSphere(GetWorld(), PathResult.HitResult.Location, 5.0f, 12, FColor::Red, false);
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Projectile path prediction failed."));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Not aiming"));
    }
}

void ASlingshotPawn::AdjustProjectile(const FInputActionValue& Value)
{
    if (ProjectileMesh)
    {
        // Extract input value (this should be a float that only controls vertical aiming)
        float InputValue = Value.Get<float>();

        // Get current rotation of the projectile
        FRotator CurrentRotation = ProjectileMesh->GetRelativeRotation();

        // Adjust pitch (Y-axis) based on input value
        float NewPitch = FMath::Clamp(CurrentRotation.Pitch + InputValue, -180.0f, 180.0f);  // Clamping pitch to 0-90 degrees

        // Set the new relative rotation for the projectile (no yaw changes, only pitch)
        ProjectileMesh->SetRelativeRotation(FRotator(NewPitch, 0.0f, 0.0f));

        // Adjust position based on the input value (move only vertically along Z-axis)
        FVector NewPosition = ProjectileMesh->GetRelativeLocation();

        // Adjust vertical position based on the input value
        NewPosition.Z -= InputValue * 10.0f;  // Vertical movement, multiply by factor to control the speed

        // Clamp position to keep it within reasonable bounds (adjust Z limits as necessary)
        NewPosition.Z = FMath::Clamp(NewPosition.Z, -180.0f, 180.0f);  // Limit vertical position to a reasonable range

        // Set the new position for the projectile
        ProjectileMesh->SetRelativeLocation(NewPosition);
    }
}


// Fire the projectile
void ASlingshotPawn::FireProjectile()
{
    if (!bIsAiming) return;

    bIsAiming = false;

    // Apply force to projectile
    FVector LaunchDirection = GetActorForwardVector(); // Adjust based on slingshot rotation
    ProjectileMesh->SetSimulatePhysics(true);
    ProjectileMesh->SetEnableGravity(true);
    ProjectileMesh->AddImpulse(LaunchDirection * PullStrength, NAME_None, true);

    // Reset projectile position after 2 seconds (optional)
    FTimerHandle ResetTimer;
    GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this]()
        {
            ProjectileMesh->SetEnableGravity(false);
            ProjectileMesh->SetWorldLocation(InitialProjectileLocation);
            ProjectileMesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        }, 2.0f, false);
    SetPullStrength(0.0f);
}


float ASlingshotPawn::GetPullStrength()
{
    return PullStrength;
}

void ASlingshotPawn::SetPullStrength(float NewStrength)
{
    PullStrength = NewStrength;
}
