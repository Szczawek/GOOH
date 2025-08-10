// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/CapsuleComponent.h>
#include "Shop.generated.h"


UCLASS()
class GOOH_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:

	UPROPERTY(EditAnyWhere, Category = "Mesh")
	UStaticMeshComponent* ShopMesh;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* SellerMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	UCapsuleComponent* SellerCollision;
	
	UFUNCTION()
	void OnBeginOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
