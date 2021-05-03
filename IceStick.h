//
//Icestick class header.
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IceStick.generated.h"

UCLASS()
class ICEICE_API AIceStick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIceStick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//static mesh of spike
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* Mesh;

	//paths for loading the spike meshes
	TArray<wchar_t*> SpikeMeshPaths =
	{ TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal1'"),
	  TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal2'"),
	  TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal3'"),
	  TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal4'"),
	  TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal5'"),
	  TEXT("StaticMesh'/Game/Models/Crystals/BLE_Crystal6'")};

	//loading spike mesh
	void LoadSpikeMesh(int PathIndex);

	//setting new size to mesh
	void SetSize(float scaleVal);
};
