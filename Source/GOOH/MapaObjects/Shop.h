#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USkeletalMeshComponent;
class AGamer;

UCLASS()
class GOOH_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	AShop();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnyWhere, Category = "Mesh");
	TObjectPtr<UStaticMeshComponent> BuildingMesh;

	UPROPERTY(EditAnyWhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SellerMesh;

	UPROPERTY(EditAnyWhere, Category = "Collision")
	TObjectPtr<UBoxComponent> BoxCollision;

private:
	TObjectPtr<AGamer> Player;

private:
	UFUNCTION()
	void OnBeginOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlaped(UPrimitiveComponent* PrimitiveComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);
};
