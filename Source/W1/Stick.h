// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stick.generated.h"

UCLASS()
class W1_API AStick : public AActor
{
	GENERATED_BODY()
	
public:
	AStick();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* TopSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BotSphere;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AW1Character* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanSpawnConnectStick;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AConnectStick> ConnectStickClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AStickPre* StickPre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AConnectStickPre* ConnectStickPre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AConnectStickPre> ConnectStickPreClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AConnectStick*> ConnectSticks;

	FTimerHandle StartTimer;

protected:
	UFUNCTION()
	void OnTopSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnBotSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBotSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SpawnConnectStick(FVector Location);
	UFUNCTION(Server, Reliable)
	void ReqSpawnConnectStick(FVector Loc, FRotator Rot);
	UFUNCTION(NetMulticast, Reliable)
	void ResSpawnConnectStick(FVector Loc, FRotator Rot);

	void SpawnConnectStickPre(FVector Location);

	void DestroyConnectStickPre();

	void SetConnectStickPreTransform();

	void StartFenceCheck();

public:
	void SetCharacter(AW1Character* NewCharacter) { Character = NewCharacter; }

	FORCEINLINE TArray<AConnectStick*> GetConnectSticks() const { return ConnectSticks; }

};
