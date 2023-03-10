// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubSystems.h"
#include "Kismet/GameplayStatics.h" //유틸리티 관련 헤더
#include "Kismet/KismetMathLibrary.h"  //수학 관련 헤더


// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateAbstractDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	
	FollowCamera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90.0f, 0));


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController * PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		PC->GetLocalPlayer();
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
		
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
		if (EIC)
		{
			EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
			EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

			EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
			EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		}

}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();
	
	FRotator ForwardRotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector FowardVector = UKismetMathLibrary::GetForwardVector(ForwardRotation);

	FRotator RightRotation = FRotator(0, GetControlRotation().Yaw, GetControlRotation().Roll);
	FVector RightVector = UKismetMathLibrary::GetRightVector(RightRotation);

	AddMovementInput(FowardVector, MoveVector.Y);
	AddMovementInput(RightVector, MoveVector.X);

}

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

