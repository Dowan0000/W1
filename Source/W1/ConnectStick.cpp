// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectStick.h"
#include "Components/SphereComponent.h"

AConnectStick::AConnectStick()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	PreSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PreSphere"));
	NextSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NextSphere"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	PreSphere->SetupAttachment(Mesh);
	NextSphere->SetupAttachment(Mesh);

}

void AConnectStick::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConnectStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

