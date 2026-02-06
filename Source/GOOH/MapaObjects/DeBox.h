#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "DeBox.generated.h"

UCLASS()
class GOOH_API ADeBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADeBox();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnyWhere, Category="Mesh")
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(EditAnywhere, Category="Colision")
	UBoxComponent* BoxCollision;
};
