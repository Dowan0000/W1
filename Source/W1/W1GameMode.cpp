// Copyright Epic Games, Inc. All Rights Reserved.

#include "W1GameMode.h"
#include "W1Character.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "W1Character.h"
#include "W1Point.h"
#include "ConnectStick.h"
#include "Stick.h"

AW1GameMode::AW1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

bool AW1GameMode::onSegment(Point p, Point q, Point r)
{
	if (q.x <= UKismetMathLibrary::Max(p.x, r.x) && q.x >= UKismetMathLibrary::Min(p.x, r.x) &&
		q.y <= UKismetMathLibrary::Max(p.y, r.y) && q.y >= UKismetMathLibrary::Min(p.y, r.y))
		return true;
	return false;
}

int AW1GameMode::orientation(Point p, Point q, Point r)
{
	double val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;  // collinear
	return (val > 0) ? 1 : 2; // clock or counterclockwise
}

bool AW1GameMode::doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

bool AW1GameMode::isInside(TArray<Point> polygon, int n, Point p)
{
	if (n < 3) return false; // A polygon must have at least 3 vertices

	Point extreme = { 1e10, p.y };

	int count = 0, i = 0;
	do 
	{
		int next = (i + 1) % n;

		if (doIntersect(polygon[i], polygon[next], p, extreme)) 
		{
			if (orientation(polygon[i], p, polygon[next]) == 0)
				return onSegment(polygon[i], p, polygon[next]);

			count++;
		}
		i = next;
	} while (i != 0);

	return (count % 2 == 1);
}

//void AW1GameMode::SetArea(class AConnectStick* NewConnectStick)
//{
//	// AllPoints -> Laser
//
//	/*UE_LOG(LogTemp, Warning, TEXT("SetArea"));
//
//	for (auto& Point : AllPoints)
//	{
//		if (Point == nullptr) continue;
//
//		Point->CheckArea();
//	}*/
//
//	ConnectSticks.Empty();
//
//	if (NewConnectStick->GetPreConnectSticks().Num() <= 0 || NewConnectStick->GetNextConnectSticks().Num() <= 0)
//		return;
//
//	ConnectSticks.Add(NewConnectStick);
//
//	//UE_LOG(LogTemp, Warning, TEXT("qwerasdf")); //
//
//	// 임시로 0번 idx
//	AConnectStick* PreConnectStick = NewConnectStick;
//	AConnectStick* ConnectStick = PreConnectStick->GetNextConnectSticks()[0];
//
//	// 무한루프 ///
//	while (true)
//	{
//		AConnectStick* NextConnectStick;
//
//		if (ConnectStick->GetPreConnectSticks().Contains(PreConnectStick))
//		{
//			if (ConnectStick->GetNextConnectSticks().Num() <= 0)
//			{
//				ConnectSticks.Empty();
//				break;
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("11111"));
//				NextConnectStick = ConnectStick->GetNextConnectSticks()[0];
//			}
//		}
//		else
//		{
//			if (ConnectStick->GetPreConnectSticks().Num() <= 0)
//			{
//				ConnectSticks.Empty();
//				break;
//			}
//			else
//			{
//				UE_LOG(LogTemp, Warning, TEXT("22222"));
//				NextConnectStick = ConnectStick->GetPreConnectSticks()[0];
//			}
//		}
//
//		ConnectSticks.Add(ConnectStick);
//
//		// 시작과 끝이 같을 때
//		if (NewConnectStick == NextConnectStick)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("33333"));
//
//			bSetMat = true;
//			break;
//		}
//
//
//		PreConnectStick = ConnectStick;
//		ConnectStick = NextConnectStick;
//	}
//
//	if (bSetMat)
//	{
//		TArray<Point> polygon;
//
//		int n = ConnectSticks.Num();
//		UE_LOG(LogTemp, Warning, TEXT("ConnectSticks.Num() : %d"), n);
//
//		for (int i = 0; i < n; i++) {
//			Point p1, p2;
//			//cin >> p1.x >> p1.y >> p2.x >> p2.y;
//			p1.x = ConnectSticks[i]->GetPreSphere()->GetComponentLocation().X;
//			p1.y = ConnectSticks[i]->GetPreSphere()->GetComponentLocation().Y;
//			p2.x = ConnectSticks[i]->GetNextSphere()->GetComponentLocation().X;
//			p2.y = ConnectSticks[i]->GetNextSphere()->GetComponentLocation().Y;
//			polygon.Add(p1);
//			// We only need to add the end point if it's not the same as the start point of the next segment
//			if (i == n - 1 || !(p2.x == p1.x && p2.y == p1.y)) {
//				polygon.Add(p2);
//			}
//		}
//
//		/*Point p;
//		cout << "Enter the point coordinates to check: ";
//		cin >> p.x >> p.y;
//
//		if (isInside(polygon, polygon.size(), p)) {
//			cout << "The point is inside the polygon.\n";
//		}
//		else {
//			cout << "The point is outside the polygon.\n";
//		}*/
//
//		for (auto& Dot : AllPoints)
//		{
//			if (Dot == nullptr) continue;
//
//			Point p;
//			p.x = Dot->GetActorLocation().X;
//			p.y = Dot->GetActorLocation().Y;
//
//			if (isInside(polygon, polygon.Num(), p))
//			{
//				UE_LOG(LogTemp, Warning, TEXT("The point is inside the polygon."));
//				Dot->SetAreaMaterial();
//			}
//			else
//				;//UE_LOG(LogTemp, Warning, TEXT("The point is outside the polygon."));
//		}
//
//		
//	}
//
//}

void AW1GameMode::SetArea(AStick* NewStick, AConnectStick* NewConnectStick)
{

	ConnectSticks.Empty();
	Polygons.Empty();
	//bSetMat = false;

	//SetAreaLoop(NewStick, NewStick, NewConnectStick);

	UE_LOG(LogTemp, Warning, TEXT("ConnectSticks.Num() : %d"), ConnectSticks.Num());

	if (SetAreaLoop(NewStick, NewStick, NewConnectStick))
	{
		
		for (auto& Dot : AllPoints)
		{
			if (Dot == nullptr) continue;

			Point p;
			p.x = Dot->GetActorLocation().X;
			p.y = Dot->GetActorLocation().Y;

			if (isInside(Polygons, Polygons.Num(), p))
			{
				Dot->SetAreaMaterial();
				//UE_LOG(LogTemp, Warning, TEXT("Dot Location - X : %f, Y : %f"), Dot->GetActorLocation().X, Dot->GetActorLocation().Y);
			}

		}

	}
}

bool AW1GameMode::SetAreaLoop(AStick* StartStick, AStick* CurStick, AConnectStick* CurConnectStick)
{
	if (CurConnectStick->GetPreConnectSticks().Num() <= 0 || CurConnectStick->GetNextConnectSticks().Num() <= 0 ||
		CurConnectStick->GetPreStick() == nullptr || CurConnectStick->GetNextStick() == nullptr)
		return false;

	CurConnectStick->SetIsVisited(true);

	//AConnectStick* PreConnectStick = CurConnectStick;
	AStick* NextStick;
	bool bIsNextStick;

	if (CurStick == CurConnectStick->GetPreStick())
	{
		NextStick = CurConnectStick->GetNextStick();
		bIsNextStick = true;
	}
	else
	{
		NextStick = CurConnectStick->GetPreStick();
		bIsNextStick = false;
	}

	for (int i = 0; i < NextStick->GetConnectSticks().Num(); i++)
	{
		AConnectStick* NextConnectStick = NextStick->GetConnectSticks()[i];

		if (NextConnectStick == nullptr || NextConnectStick == CurConnectStick) continue;

		if (StartStick == NextStick)
		{
			UE_LOG(LogTemp, Warning, TEXT("33333"));

			//bSetMat = true;

			ConnectSticks.Add(NextConnectStick);

			AddPolygons(bIsNextStick, CurConnectStick);

			return true;
		}

		if (NextConnectStick->GetIsVisited()) continue;

		if (SetAreaLoop(StartStick, NextStick, NextConnectStick))
		{
			ConnectSticks.Add(CurConnectStick);

			AddPolygons(bIsNextStick, CurConnectStick);

			return true;
		}
	}


	return false;
}

void AW1GameMode::AddPolygons(bool IsNextStick, AConnectStick* CurConnectStick)
{
	Point p1, p2;
	if (IsNextStick)
	{
		p1.x = CurConnectStick->GetNextStick()->GetActorLocation().X;
		p1.y = CurConnectStick->GetNextStick()->GetActorLocation().Y;
		p2.x = CurConnectStick->GetPreStick()->GetActorLocation().X;
		p2.y = CurConnectStick->GetPreStick()->GetActorLocation().Y;
		Polygons.Add(p1);
		Polygons.Add(p2);
	}
	else
	{
		p1.x = CurConnectStick->GetPreStick()->GetActorLocation().X;
		p1.y = CurConnectStick->GetPreStick()->GetActorLocation().Y;
		p2.x = CurConnectStick->GetNextStick()->GetActorLocation().X;
		p2.y = CurConnectStick->GetNextStick()->GetActorLocation().Y;
		Polygons.Add(p1);
		Polygons.Add(p2);
	}
}

void AW1GameMode::SetAreaMaterial_Implementation(AW1Point* Point)
{
}
