// Copyright Epic Games, Inc. All Rights Reserved.

#include "W1Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

//#include "Net/UnrealNetwork.h"

#include "StickPre.h"
#include "Stick.h"

AW1Character::AW1Character()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

void AW1Character::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AW1Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AW1Character::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AW1Character::Look);

		EnhancedInputComponent->BindAction(F, ETriggerEvent::Started, this, &AW1Character::TriggerF);

		EnhancedInputComponent->BindAction(LeftMouse, ETriggerEvent::Started, this, &AW1Character::TriggerLeftMouse);
		
		EnhancedInputComponent->BindAction(RightMouse, ETriggerEvent::Started, this, &AW1Character::TriggerRightMouse);

	}

}

void AW1Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AW1Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (bIsBuilding && StickPre)
	{
		LookOnBuilding(LookAxisVector);
		return;
	}

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AW1Character::TriggerF(const FInputActionValue& Value)
{
	if (StickPre == nullptr)
		SpawnStickPre();
}

void AW1Character::TriggerLeftMouse(const FInputActionValue& Value)
{
	if (StickPre == nullptr) return;

	SpawnStick();
}

void AW1Character::TriggerRightMouse(const FInputActionValue& Value)
{
	if (StickPre == nullptr) return;
	
	DestroyStickPre();
}

void AW1Character::SpawnStickPre()
{
	FActorSpawnParameters SpawnParameter;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	FVector Location = FVector(GetActorLocation().X, GetActorLocation().Y, 0.f);
	Location +=	ForwardDirection * 150.f;

	StickPre = GetWorld()->SpawnActor<AStickPre>(StickPreClass, Location, FRotator(0.f, 0.f, 0.f), SpawnParameter);
	if (StickPre)
		bIsBuilding = true;
}

void AW1Character::DestroyStickPre()
{
	StickPre->Destroy();
	StickPre = nullptr;
	bIsBuilding = false;
}

void AW1Character::SpawnStick()
{
	if (StickPre->bCanSpawn == false) return;

	FVector Location = StickPre->GetActorLocation();

	DestroyStickPre();

	ReqSpawnStick(Location);
}

void AW1Character::ReqSpawnStick_Implementation(FVector Location)
{
	ResSpawnStick(Location);
}

void AW1Character::ResSpawnStick_Implementation(FVector Location)
{
	FActorSpawnParameters SpawnParameter;

	GetWorld()->SpawnActor<AStick>(StickClass, Location, FRotator(0.f, 0.f, 0.f), SpawnParameter);
}

void AW1Character::LookOnBuilding(FVector2D Value)
{
	FVector Location = StickPre->GetActorLocation();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Location += ForwardDirection * -3.f * Value.Y;
	Location += RightDirection * 3.f * Value.X;

	StickPre->SetActorLocation(Location);
}
