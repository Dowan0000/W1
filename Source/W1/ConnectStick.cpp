// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectStick.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

#include "W1Character.h"
#include "Stick.h"

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

	PreSphere->OnComponentBeginOverlap.AddDynamic(this, &AConnectStick::OnPreSphereBeginOverlap);
	NextSphere->OnComponentBeginOverlap.AddDynamic(this, &AConnectStick::OnNextSphereBeginOverlap);
}

void AConnectStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConnectStick::OnPreSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	AConnectStick* PreConnectStick = Cast<AConnectStick>(OtherActor);
	if (PreConnectStick)
	{
		if (PreConnectSticks.Contains(PreConnectStick)) return;

		PreConnectSticks.Add(PreConnectStick);

		return;
	}

	AStick* Stick = Cast<AStick>(OtherActor);
	if (Stick)
		PreStick = Stick;
}

void AConnectStick::OnNextSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	AConnectStick* NextConnectStick = Cast<AConnectStick>(OtherActor);
	if (NextConnectStick)
	{
		if (NextConnectSticks.Contains(NextConnectStick)) return;

		NextConnectSticks.Add(NextConnectStick);

		return;
	}

	AStick* Stick = Cast<AStick>(OtherActor);
	if (Stick)
		NextStick = Stick;
}

