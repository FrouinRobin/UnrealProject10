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

    ProjectilAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileAnchor"));
    ProjectilAnchor->SetupAttachment(Camera);

    bIsAiming = false;
    PullStrength = 0.0f;
}

// Begin Play
void ASlingshotPawn::BeginPlay()
{
    Super::BeginPlay();

    // Store the initial projectile location (so we can reset it)
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // Call the new function to spawn the bird
    SpawnBird();
}

void ASlingshotPawn::SpawnBird()
{
    if (BirdList.Num() > 0 && BirdList[0] != nullptr)
    {
        // Define spawn parameters
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // Get the spawn location from the Anchor component
        FVector SpawnLocation = ProjectilAnchor->GetComponentLocation();
        FRotator SpawnRotation = FRotator::ZeroRotator;

        // Spawn the bird
        LoadedBird = GetWorld()->SpawnActor<ARedBird>(BirdList[0], SpawnLocation, SpawnRotation, SpawnParams);
        BirdList.RemoveAt(0);
        if (LoadedBird)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully spawned LoadedBird at location: %s"), *SpawnLocation.ToString());

            // Attach to the anchor so it moves with the slingshot
            LoadedBird->SetOwner(this);
            LoadedBird->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

            // Disable physics initially so it behaves like it's on an elastic rope
            UPrimitiveComponent* BirdRoot = Cast<UPrimitiveComponent>(LoadedBird->GetRootComponent());
            if (BirdRoot)
            {
                BirdRoot->SetSimulatePhysics(false);
                BirdRoot->SetEnableGravity(false);
            }

            // Store the initial projectile location
            InitialProjectileLocation = LoadedBird->GetActorLocation();
            ActorsToIgnore.Add(LoadedBird);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn LoadedBird!"));
        }
    }
}


// Tick (for charge-up effect)
void ASlingshotPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector Start = LoadedBird->GetActorLocation();
    FVector ForwardVector =LoadedBird->GetActorForwardVector();
    FVector End = Start + (ForwardVector * 100.0f); // Extend the line length

    DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.0f, 0, 2.0f);
    // Increase pull strength over time
    
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
        EnhancedInput->BindAction(IA_LookOnX, ETriggerEvent::Triggered, this, &ASlingshotPawn::AdjustYawRotation);
        EnhancedInput->BindAction(IA_LookOnY, ETriggerEvent::Triggered, this, &ASlingshotPawn::AdjustPitchRotation);

    }
}

void ASlingshotPawn::AdjustYawRotation(const FInputActionValue& Value)
{
    if (LoadedBird)
    {
        // Extract horizontal input (X-axis) for yaw control
        float InputValue = Value.Get<float>();

        // Apply sensitivity scaling
        float SensitivityX = 0.2f;

        // Get the current rotation of the bird
        FRotator CurrentRotation = LoadedBird->GetActorRotation();

        // Adjust yaw (horizontal rotation) based on input
        float NewYaw = FMath::Clamp(CurrentRotation.Yaw + InputValue * SensitivityX, -45.0f, 45.0f);

        // Set the new rotation (keep pitch and roll values the same)
        LoadedBird->SetActorRotation(FRotator(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Loaded bird is nullptr. Can't adjust yaw rotation!"));
    }
}

void ASlingshotPawn::AdjustPitchRotation(const FInputActionValue& Value)
{
    if (LoadedBird)
    {
        // Extract vertical input (Y-axis) for pitch control
        float InputValue = Value.Get<float>();

        // Apply sensitivity scaling
        float SensitivityY = 0.5f;

        // Get the current rotation of the bird
        FRotator CurrentRotation = LoadedBird->GetActorRotation();

        // Adjust pitch (vertical rotation) based on input
        float NewPitch = FMath::Clamp(CurrentRotation.Pitch + InputValue * SensitivityY, -45.0f, 45.0f);

        // Set the new rotation (keep yaw and roll values the same)
        LoadedBird->SetActorRotation(FRotator(NewPitch, CurrentRotation.Yaw, CurrentRotation.Roll));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Loaded bird is nullptr. Can't adjust pitch rotation!"));
    }
}

// Start Aiming (begin charging the shot)
void ASlingshotPawn::StartAiming()
{
    bIsAiming = true;

    if (bIsAiming)
    {
        // Increase pull strength over time and clamp it to a maximum value
        if (GetPullStrength() <= 1000.f) {
            float NewPullStrength = GetPullStrength();
            NewPullStrength += GetWorld()->GetDeltaSeconds() * 1000.f;
            SetPullStrength(NewPullStrength);
            UE_LOG(LogTemp, Warning, TEXT("Aiming - PullStrength: %f"), GetPullStrength());

            // Calculate backward movement based on the pull strength
            if (LoadedBird)
            {
                FVector BackwardDirection = -LoadedBird->GetActorForwardVector(); // Get the opposite direction of the forward vector

                // Calculate the backward distance as proportional to the pull strength
                // The backward distance will stop increasing once it reaches 1 meter.

                // If pull strength has hit max (3000), make sure the backward movement doesn't go past 1 meter
                FVector NewLocation = LoadedBird->GetActorLocation() + BackwardDirection / 10.f;
                LoadedBird->SetActorLocation(NewLocation);
            }
        }
        

        // Set up the path prediction parameters
        FPredictProjectilePathParams PathParams;
        PathParams.StartLocation = LoadedBird->GetActorLocation(); // Start from the bird's current location
        PathParams.LaunchVelocity = LoadedBird->GetActorForwardVector() * GetPullStrength(); // Example velocity
        PathParams.bTraceWithCollision = true;
        PathParams.ProjectileRadius = 5.0f;
        PathParams.bTraceWithChannel = true;
        PathParams.TraceChannel = ECC_Visibility;
        PathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
        PathParams.SimFrequency = 25.f;

        // Ignore the actors in ActorsToIgnore during path prediction
        PathParams.ActorsToIgnore = ActorsToIgnore;

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

            if (PathResult.HitResult.bBlockingHit && !ActorsToIgnore.Contains(PathResult.HitResult.GetActor())) // Check if the projectile hit something not in ActorsToIgnore
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




// Fire the projectile
void ASlingshotPawn::FireProjectile()
{
    if (!bIsAiming) return;

    bIsAiming = false;
    FVector LaunchDirection = GetActorForwardVector(); // Adjust based on slingshot rotation
    LoadedBird->DetachRootComponentFromParent();
    UPrimitiveComponent* BirdRoot = Cast<UPrimitiveComponent>(LoadedBird->GetRootComponent());
    if (BirdRoot)
    {
        BirdRoot->SetSimulatePhysics(true);
        BirdRoot->SetEnableGravity(true);
        BirdRoot->AddImpulse(LoadedBird->GetActorForwardVector() * GetPullStrength(), NAME_None, true);
    }
    
    
    // Reset projectile position after 2 seconds (optional)
    FTimerHandle ResetTimer;
    GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this]()
        {
            SpawnBird();
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
