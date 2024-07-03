// Fill out your copyright notice in the Description page of Project Settings.


#include "W1Point.h"
#include "ConnectStick.h"

AW1Point::AW1Point()
{
 	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));

	SetRootComponent(Root);
	Floor->SetupAttachment(Root);
}

void AW1Point::BeginPlay()
{
	Super::BeginPlay();
	
}

void AW1Point::CheckArea()
{
	// ���� ����
	GetWorld()->GetTimerManager().SetTimer(CheckAreaTimerHandle, this, &AW1Point::CheckAreaTimer, 0.1f);
}

void AW1Point::CheckAreaTimer()
{
	// ������ ������
	TArray<FHitResult> RightHits;
	FCollisionQueryParams RightParams;
	GetWorld()->LineTraceMultiByChannel(RightHits, GetActorLocation(),
		GetActorLocation() + (GetActorRightVector() * 50'000),
		ECollisionChannel::ECC_Camera, RightParams);

	if (RightHits.Num() <= 0) return;

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *RightHits[0].GetActor()->GetName());

	for (auto& RightHit : RightHits)
	{
		if (RightHit.GetActor() == nullptr) return;
		// ���� connectstick�� ��������

		AConnectStick* FirstConnectStick = Cast<AConnectStick>(RightHit.GetActor());
		if (FirstConnectStick == nullptr) continue;

		if (FirstConnectStick->GetPreConnectSticks().Num() <= 0 || FirstConnectStick->GetNextConnectSticks().Num() <= 0) continue;
		ConnectSticks.Add(FirstConnectStick);

		//UE_LOG(LogTemp, Warning, TEXT("qwerasdf")); //

		// �ӽ÷� 0�� idx
		AConnectStick* PreConnectStick = FirstConnectStick;
		AConnectStick* ConnectStick = PreConnectStick->GetNextConnectSticks()[0];

		// ���ѷ��� ///
		while (true)
		{
			AConnectStick* NextConnectStick;

			if (ConnectStick->GetPreConnectSticks().Contains(PreConnectStick))
			{
				if (ConnectStick->GetNextConnectSticks().Num() <= 0)
				{
					ConnectSticks.Empty();
					break;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("11111"));
					NextConnectStick = ConnectStick->GetNextConnectSticks()[0];
				}	
			}
			else
			{
				if (ConnectStick->GetPreConnectSticks().Num() <= 0)
				{
					ConnectSticks.Empty();
					break;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("22222"));
					NextConnectStick = ConnectStick->GetPreConnectSticks()[0];
				}
			}

			ConnectSticks.Add(ConnectStick);

			// ���۰� ���� ���� ��
			if (FirstConnectStick == NextConnectStick)
			{
				UE_LOG(LogTemp, Warning, TEXT("33333"));

				bIsChecked = true;

				// ���� ������
				TArray<FHitResult> LeftHits;
				FCollisionQueryParams LeftParams;
				GetWorld()->LineTraceMultiByChannel(LeftHits, GetActorLocation(),
					GetActorLocation() - (GetActorRightVector() * 50'000),
					ECollisionChannel::ECC_Camera, LeftParams);

				if (LeftHits.Num() <= 0) return;

				// �����ʿ��� ���� connectstick�� �Ѽ�Ʈ����
				for (auto& LeftHit : LeftHits)
				{
					if (LeftHit.GetActor() == nullptr) return;

					AConnectStick* CheckConnectStick = Cast<AConnectStick>(LeftHit.GetActor());
					if (CheckConnectStick == nullptr) continue;

					if (ConnectSticks.Contains(CheckConnectStick))
					{
						bSetMat = true;
						break;
					}
				}


			}

			if (bIsChecked)
				break;

			PreConnectStick = ConnectStick;
			ConnectStick = NextConnectStick;
		}

		if (bSetMat)
		{
			SetAreaMaterial();
			break;
		}


	}

	bIsChecked = false;
	bSetMat = false;
	ConnectSticks.Empty();
}

void AW1Point::SetAreaMaterial_Implementation()
{
}
