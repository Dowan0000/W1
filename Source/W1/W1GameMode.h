// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "W1GameMode.generated.h"

struct Point {
	double x, y;
};

UCLASS(minimalapi)
class AW1GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AW1GameMode();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TArray<class AW1Point*> AllPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<class AConnectStick*> ConnectSticks;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bSetMat;*/

	TArray<Point> Polygons;

protected:
	bool onSegment(Point p, Point q, Point r);

	int orientation(Point p, Point q, Point r);

	bool doIntersect(Point p1, Point q1, Point p2, Point q2);

	bool isInside(TArray<Point> polygon, int n, Point p);

public:
	//void SetArea(class AConnectStick* NewConnectStick);
	void SetArea(class AStick* NewStick, AConnectStick* NewConnectStick);

	bool SetAreaLoop(AStick* StartStick, AStick* CurStick, AConnectStick* CurConnectStick);

protected:
	void AddPolygons(bool IsNextStick, AConnectStick* CurConnectStick);

	UFUNCTION(BlueprintNativeEvent)
	void SetAreaMaterial(AW1Point* Point);

};



