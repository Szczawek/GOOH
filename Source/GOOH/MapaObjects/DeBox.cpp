#include "../Player/Gamer.h"
#include "DeBox.h"

ADeBox::ADeBox()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	this->Tags.Add(FName("Destructable"));
}

void ADeBox::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ADeBox::OnComponentBeginOverlaped);
	
}

void ADeBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeBox::OnComponentBeginOverlaped(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AGamer* Player = Cast<AGamer>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("TRES"));
		if (Player->bIsAttacking) {
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("2"));
		}
	}
}

