// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConnectStick.generated.h"

UCLASS()
class W1_API AConnectStick : public AActor
{
	GENERATED_BODY()
	
public:
	AConnectStick();

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
	class USphereComponent* PreSphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* NextSphere;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AW1Character* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AStick* PreStick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AStick* NextStick;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AConnectStick*> PreConnectSticks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<AConnectStick*> NextConnectSticks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsVisited;

protected:
	UFUNCTION()
	void OnPreSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnNextSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetCharacter(AW1Character* NewCharacter) { Character = NewCharacter; }
	
	FORCEINLINE class USphereComponent* GetPreSphere() const { return PreSphere; }
	FORCEINLINE class USphereComponent* GetNextSphere() const { return NextSphere; }

	FORCEINLINE class TArray<AConnectStick*> GetPreConnectSticks() const { return PreConnectSticks; }
	FORCEINLINE class TArray<AConnectStick*> GetNextConnectSticks() const { return NextConnectSticks; }

	FORCEINLINE class AStick* GetPreStick() const { return PreStick; }
	FORCEINLINE class AStick* GetNextStick() const { return NextStick; }

	FORCEINLINE bool GetIsVisited() const { return bIsVisited; }
	void SetIsVisited(bool NewVisited) { bIsVisited = NewVisited; }
};
