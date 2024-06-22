// Fill out your copyright notice in the Description page of Project Settings.


#include "StickPre.h"
#include "Components/SphereComponent.h"

AStickPre::AStickPre()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InSphere"));
	OutSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OutSphere"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	InSphere->SetupAttachment(Mesh);
	OutSphere->SetupAttachment(Mesh);

}

void AStickPre::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation();
}

void AStickPre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSpawnable();
}

void AStickPre::CheckSpawnable()
{
	bHasOverlappingActor = false;

	TArray<AActor*> OverlappingActors;
	InSphere->GetOverlappingActors(OverlappingActors);
	for (auto& OverlappingActor : OverlappingActors)
	{
		if (OverlappingActor != this)
		{
			bHasOverlappingActor = true;
			break;
		}
	}

	if (bHasOverlappingActor)
	{
		bCanSpawn = false;
		SetMaterial(false);
	}
	else
	{
		bCanSpawn = true;
		SetMaterial(true);
	}
}

void AStickPre::SetMaterial_Implementation(bool CanSpawn)
{
}
