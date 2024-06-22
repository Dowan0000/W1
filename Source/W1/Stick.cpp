// Fill out your copyright notice in the Description page of Project Settings.


#include "Stick.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "StickPre.h"
#include "ConnectStick.h"
#include "ConnectStickPre.h"

AStick::AStick()
{
 	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TopSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TopSphere"));
	BotSphere = CreateDefaultSubobject<USphereComponent>(TEXT("BotSphere"));

	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);
	TopSphere->SetupAttachment(Mesh);
	BotSphere->SetupAttachment(Mesh);
}

void AStick::BeginPlay()
{
	Super::BeginPlay();
	
	TopSphere->OnComponentBeginOverlap.AddDynamic(this, &AStick::OnTopSphereBeginOverlap);

	BotSphere->OnComponentBeginOverlap.AddDynamic(this, &AStick::OnBotSphereBeginOverlap);
	BotSphere->OnComponentEndOverlap.AddDynamic(this, &AStick::OnBotSphereEndOverlap);
}

void AStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetConnectStickPreTransform();
}

void AStick::OnTopSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AStick::OnBotSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	AStick* OtherStick = Cast<AStick>(OtherActor);
	if (OtherStick && bCanSpawnConnectStick)
	{
		SpawnConnectStick(OtherActor->GetActorLocation());
	}

	StickPre = Cast<AStickPre>(OtherActor);
	if (StickPre && StickPre->GetOutSphere() == OtherComp)
	{
		SpawnConnectStickPre(StickPre->GetActorLocation());
	}
}

void AStick::OnBotSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == this) return;

	StickPre = Cast<AStickPre>(OtherActor);
	if (StickPre && StickPre->GetOutSphere() == OtherComp)
	{
		DestroyConnectStickPre();
	}
}

void AStick::SpawnConnectStick(FVector Location)
{
	FVector Loc = (GetActorLocation() + Location) / 2;
	Loc = FVector(Loc.X, Loc.Y, Loc.Z + 95.f);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);

	ReqSpawnConnectStick(Loc, Rot);
}

void AStick::ReqSpawnConnectStick_Implementation(FVector Loc, FRotator Rot)
{
	ResSpawnConnectStick(Loc, Rot);
}

void AStick::ResSpawnConnectStick_Implementation(FVector Loc, FRotator Rot)
{
	if (ConnectStickClass == nullptr) return;

	FActorSpawnParameters SpawnParameter;
	GetWorld()->SpawnActor<AConnectStick>(ConnectStickClass, Loc, Rot, SpawnParameter);
}

void AStick::SpawnConnectStickPre(FVector Location)
{
	if (ConnectStickPreClass == nullptr) return;

	FVector Loc = (GetActorLocation() + Location) / 2;
	Loc = FVector(Loc.X, Loc.Y, Loc.Z + 95.f);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location);
	
	FActorSpawnParameters SpawnParameter;
	ConnectStickPre = GetWorld()->SpawnActor<AConnectStickPre>(ConnectStickPreClass, Loc, Rot, SpawnParameter);
}

void AStick::DestroyConnectStickPre()
{
	if (StickPre && ConnectStickPre)
	{
		StickPre = nullptr;
		ConnectStickPre->Destroy();
		ConnectStickPre = nullptr;
	}
}

void AStick::SetConnectStickPreTransform()
{
	if (StickPre == nullptr || ConnectStickPre == nullptr) return;

	FVector Loc = (GetActorLocation() + StickPre->GetActorLocation()) / 2;
	Loc = FVector(Loc.X, Loc.Y, Loc.Z + 100.f);

	FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), StickPre->GetActorLocation());

	ConnectStickPre->SetActorLocationAndRotation(Loc, Rot);
}
