// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	springArm = this->FindComponentByClass<USpringArmComponent>();
	if(!springArm) {
		UE_LOG(LogTemp, Error, TEXT("No spring arm found on component: %s"), *GetOwner()->GetName());
	}

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

	CurrentHealth = MaxHealth;
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveLeft);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAxis(TEXT("ScrollWheel"), this, &AShooterCharacter::CameraZoom);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Shoot);
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageToApply = FMath::Min(CurrentHealth, DamageToApply);
	CurrentHealth -= DamageToApply;

	UE_LOG(LogTemp, Warning, TEXT("Health remaining: %f"), CurrentHealth);

	return DamageToApply;
}

void AShooterCharacter::MoveForward(float axisValue) 
{
	AddMovementInput(GetActorForwardVector() * axisValue);
}

void AShooterCharacter::MoveLeft(float axisValue) 
{
	AddMovementInput(GetActorRightVector() * axisValue);
}

void AShooterCharacter::LookUpRate(float axisValue) 
{
	AddControllerPitchInput(axisValue * lookSensitivity * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float axisValue) 
{
	AddControllerYawInput(axisValue * lookSensitivity * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::CameraZoom(float axisValue) 
{
	if(springArm) {
		float targetZoom = springArm->TargetArmLength + axisValue * cameraZoomSensitivity;
		springArm->TargetArmLength = FMath::Min(FMath::Max(targetZoom, cameraMinDist), cameraMaxDist);
	}
}

void AShooterCharacter::Shoot() 
{
	if(Gun != nullptr) {
		Gun->PullTrigger();
	}
}

