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
}

void ADeBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}