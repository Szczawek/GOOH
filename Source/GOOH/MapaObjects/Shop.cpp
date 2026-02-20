#include "Shop.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Player/Gamer.h"


AShop::AShop()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	SellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Seller Mesh"));
	SellerMesh->SetupAttachment(BoxCollision);
	BoxCollision->SetupAttachment(BuildingMesh);
}

void AShop::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, TEXT("Nie test 222"));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnBeginOverlaped);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &AShop::OnEndOverlaped);
	
}
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::OnBeginOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Player = Cast<AGamer>(OtherActor)) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("2313213123"));
	}
	
}

void AShop::OnEndOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
}

