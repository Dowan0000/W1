// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "W1Character.generated.h"


UCLASS(config=Game)
class AW1Character : public ACharacter
{
	GENERATED_BODY()

public:
	AW1Character();

protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* F;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LeftMouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightMouse;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AW1GameMode* W1GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	class AStickPre* StickPre;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AStickPre> StickPreClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	bool bIsBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AStick> StickClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	TArray<class AConnectStick*> ConnectSticks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stick, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> StickLocations;

protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);
	
	void TriggerF(const FInputActionValue& Value);

	void TriggerLeftMouse(const FInputActionValue& Value);

	void TriggerRightMouse(const FInputActionValue& Value);

protected:
	void SpawnStickPre();

	void DestroyStickPre();

	void SpawnStick();
	UFUNCTION(Server, Reliable)
	void ReqSpawnStick(FVector Location);
	UFUNCTION(NetMulticast, Reliable)
	void ResSpawnStick(FVector Location);

	void LookOnBuilding(FVector2D Value);

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void CheckArea(AConnectStick* NewConnectStick);

	void AddConnectSticks(AConnectStick* NewConnectStick) { ConnectSticks.Add(NewConnectStick); }
	void InitialConnectSticksVisited();
};

