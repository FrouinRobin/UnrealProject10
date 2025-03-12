#include "SlingshotPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"





// Constructor
ASlingshotPawn::ASlingshotPawn()
{
    PrimaryActorTick.bCanEverTick = true;


    
    Anchor = CreateDefaultSubobject<USceneComponent>(TEXT("MeshAnchor"));
    RootComponent = Anchor;
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(RootComponent);
    Camera->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
    Camera->bUsePawnControlRotation = true;

    ProjectilAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileAnchor"));
    ProjectilAnchor->SetupAttachment(Camera);

    bIsAiming = false;
    PullStrength = 0.0f;
}


void ASlingshotPawn::BeginPlay()
{
    Super::BeginPlay();


    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }


    SpawnBird();
}

void ASlingshotPawn::SpawnBird()
{
    if (BirdList.Num() > 0 && BirdList[0] != nullptr)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        FVector SpawnLocation = ProjectilAnchor->GetComponentLocation();
        FRotator SpawnRotation = GetActorRotation();  // Set initial rotation based on the pawn's rotation

        LoadedBird = GetWorld()->SpawnActor<ARedBird>(BirdList[0], SpawnLocation, SpawnRotation, SpawnParams);
        BirdList.RemoveAt(0);

        if (LoadedBird)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully spawned LoadedBird at location: %s"), *SpawnLocation.ToString());

            LoadedBird->SetOwner(this);
            LoadedBird->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
            LoadedBird->SetActorRotation(SpawnRotation); // Set initial bird rotation relative to the pawn's rotation

            UPrimitiveComponent* BirdRoot = Cast<UPrimitiveComponent>(LoadedBird->GetRootComponent());
            if (BirdRoot)
            {
                BirdRoot->SetSimulatePhysics(false);
                BirdRoot->SetEnableGravity(false);
            }

            InitialProjectileLocation = LoadedBird->GetActorLocation();
            ActorsToIgnore.Add(LoadedBird);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn LoadedBird!"));
        }
    }
    AttachCable();
}


void ASlingshotPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FVector Start = LoadedBird->GetActorLocation();
    FVector ForwardVector =LoadedBird->GetActorForwardVector();
    FVector End = Start + (ForwardVector * 100.0f);

    DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 0.0f, 0, 2.0f);

}

// Input Setup
void ASlingshotPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
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
        float InputValue = Value.Get<float>();
        float SensitivityX = 0.2f;  // Adjust sensitivity for yaw
        FRotator actorRotation = this->GetActorRotation();
        // Calculate the amount to rotate the bird around the pawn's up axis (Z)
        FRotator CurrentRotation = LoadedBird->GetActorRotation();
        float NewYaw = FMath::Clamp(CurrentRotation.Yaw + InputValue * SensitivityX, actorRotation.Yaw - 45.0f, actorRotation.Yaw + 45.0f);

        // Set the new rotation relative to the current pitch and roll of the bird
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
        float InputValue = Value.Get<float>();
        float SensitivityY = 0.5f;  // Adjust sensitivity for pitch
        FRotator actorRotation = this->GetActorRotation();
        // Calculate the amount to rotate the bird around the pawn's right axis (X)
        FRotator CurrentRotation = LoadedBird->GetActorRotation();
        float NewPitch = FMath::Clamp(CurrentRotation.Pitch + InputValue * SensitivityY, actorRotation.Pitch - 45.0f, actorRotation.Pitch + 45.0f);

        // Set the new rotation relative to the current yaw and roll of the bird
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
        if (GetPullStrength() <= 1000.f) {
            float NewPullStrength = GetPullStrength();
            NewPullStrength += GetWorld()->GetDeltaSeconds() * 1000.f;
            SetPullStrength(NewPullStrength);
            UE_LOG(LogTemp, Warning, TEXT("Aiming - PullStrength: %f"), GetPullStrength());

            if (LoadedBird)
            {
                FVector BackwardDirection = -LoadedBird->GetActorForwardVector();
                FVector NewLocation = LoadedBird->GetActorLocation() + BackwardDirection / 2.f;
                LoadedBird->SetActorLocation(NewLocation);
                ProjectilAnchor->SetWorldLocation(ProjectilAnchor->GetComponentLocation() + BackwardDirection / 2.f);
            }
        }
        FPredictProjectilePathParams PathParams;
        PathParams.StartLocation = LoadedBird->GetActorLocation(); // Start from the bird's current location
        PathParams.LaunchVelocity = LoadedBird->GetActorForwardVector() * GetPullStrength(); // Example velocity
        PathParams.bTraceWithCollision = true;
        PathParams.ProjectileRadius = 5.0f;
        PathParams.bTraceWithChannel = true;
        PathParams.TraceChannel = ECC_Visibility;
        PathParams.DrawDebugType = EDrawDebugTrace::ForOneFrame;
        PathParams.SimFrequency = 25.f;


        PathParams.ActorsToIgnore = ActorsToIgnore;

        FPredictProjectilePathResult PathResult;
        bool bSuccess = UGameplayStatics::PredictProjectilePath(GetWorld(), PathParams, PathResult);

        if (bSuccess)
        {

            UE_LOG(LogTemp, Warning, TEXT("Predicted %d points in the path"), PathResult.PathData.Num());


            for (const FPredictProjectilePathPointData& Point : PathResult.PathData)
            {
                DrawDebugSphere(GetWorld(), Point.Location, 5.0f, 12, FColor::Green, false);
            }

            if (PathResult.HitResult.bBlockingHit && !ActorsToIgnore.Contains(PathResult.HitResult.GetActor())) 
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
    ProjectilAnchor->SetWorldLocation(InitialProjectileLocation);
    AttachCable();
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
