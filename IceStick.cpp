//
//Icestick class.
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//
//

#include "IceStick.h"
#include "Math/UnrealMathUtility.h"

AIceStick::AIceStick()
{
	//don't call Tick() every frame
	PrimaryActorTick.bCanEverTick = false;

	// Creating a static mesh
	Mesh = CreateDefaultSubobject<std::decay<decltype(*Mesh)>::type>("Mesh");

	//loads mesh chosen from list of spike meshes
	LoadSpikeMesh(1);
}

// Called when the game starts or when spawned
void AIceStick::BeginPlay()
{
	Super::BeginPlay();

	//scale the mesh to be a lot smaller
	float newScale = Mesh->GetComponentScale().X / 20;
	Mesh->SetWorldScale3D(FVector(newScale));
}

//set new size for the spike
void AIceStick::SetSize(float scaleVal) {
	float newScale = Mesh->GetComponentScale().X * scaleVal;
	Mesh->SetWorldScale3D(FVector(newScale));
}


void AIceStick::LoadSpikeMesh(int PathIndex) {

	if (PathIndex <= SpikeMeshPaths.Num()) {
		// Load the mesh using helper class
		const auto* meshPath = SpikeMeshPaths[PathIndex];

		if (meshPath) {
			static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(meshPath);
			// If the mesh loaded successfully, then assign it to our mesh
			if (meshAsset.Succeeded()) {
				Mesh->SetStaticMesh(meshAsset.Object);
			}
		}
		SetRootComponent(Mesh);

		//disable collision
		SetActorEnableCollision(false);
	}

}



