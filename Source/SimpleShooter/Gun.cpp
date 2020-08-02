// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

void AGun::PullTrigger() 
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(!OwnerPawn) return;

	AController* OwnerController = OwnerPawn->GetController();
	if(!OwnerController) return;
	
	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(OUT Location, OUT Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;

	bool bSuccess = GetWorld()->LineTraceSingleByChannel(OUT Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1); //ECC_GameTraceChannel1 = Bullet channel

	if(bSuccess) {
			FVector ShotDirection = -Rotation.Vector(); //Where the shot is coming from
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Impact, Hit.Location, ShotDirection.Rotation());
			
			AActor* ActorHit = Hit.GetActor();
			if(ActorHit != nullptr) {
				FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
				ActorHit->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

