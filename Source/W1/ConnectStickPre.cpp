// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectStickPre.h"

AConnectStickPre::AConnectStickPre()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
}

void AConnectStickPre::BeginPlay()
{
	Super::BeginPlay();
	
}

void AConnectStickPre::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

