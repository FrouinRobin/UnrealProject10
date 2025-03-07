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

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);  // Attach the camera to the root component
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));  // Position the camera
    Camera->bUsePawnControlRotation = true;  // Rotate the camera with pawn control

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
        EnhancedInput->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASlingshotPawn::Look);
    }
}

void ASlingshotPawn::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Log if the controller is valid
        UE_LOG(LogTemp, Warning, TEXT("Controller is valid! Look Input: %s"), *LookAxisVector.ToString());
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
    else
    {
        // Log if the controller is not valid
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
        PathParams.StartLocation = Camera->GetComponentLocation(); // Start from the camera position
        PathParams.LaunchVelocity = Camera->GetForwardVector() * GetPullStrength(); // Example velocity
        PathParams.bTraceWithCollision = true;
        PathParams.ProjectileRadius = 5.0f;
        PathParams.bTraceWithChannel = true;
        PathParams.TraceChannel = ECC_Visibility;
        PathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
        PathParams.DrawDebugTime = 0.0f;
        PathParams.SimFrequency = 25.f;


        FPredictProjectilePathResult PathResult;
        if (UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult))
        {
            // Successfully predicted path
            for (const FPredictProjectilePathPointData& Point : PathResult.PathData)
            {
                DrawDebugSphere(GetWorld(), Point.Location, 5.0f, 12, FColor::Green, false, 5.f);
            }
            if (PathResult.HitResult.bBlockingHit) // Check if the projectile hit something
            {
                DrawDebugSphere(GetWorld(), PathResult.HitResult.Location, 5.0f, 12, FColor::Red, false);

            }
            else {

            }
        }

    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("not aiming"));
    }
}

// Fire the projectile
void ASlingshotPawn::FireProjectile()
{
    if (!bIsAiming) return;

    bIsAiming = false;

    // Apply force to projectile
    FVector LaunchDirection = GetActorForwardVector(); // Adjust based on slingshot rotation
    ProjectileMesh->AddImpulse(LaunchDirection * PullStrength, NAME_None, true);

    // Reset projectile position after 2 seconds (optional)
    FTimerHandle ResetTimer;
    GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this]()
        {
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
