// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ShopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shop Mesh"));
	ShopMesh->SetupAttachment(RootComponent);
	
	SellerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Seller Mesh"));
	SellerCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Seller collision"));
	SellerCollision->SetupAttachment(ShopMesh);
	SellerMesh->SetupAttachment(SellerCollision);
	SellerCollision->OnComponentBeginOverlap.AddDynamic(this, &AShop::OnBeginOverlaped);
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::OnBeginOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Test"));
}

