#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeBox.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

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
	TObjectPtr<UStaticMeshComponent> BoxMesh;

	UPROPERTY(EditAnywhere, Category="Colision")
	TObjectPtr<UBoxComponent> BoxCollision;
};
