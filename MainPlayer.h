//
//Player controll class
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Containers/Array.h"
#include "Math/UnrealMathUtility.h"
#include "NiagaraFunctionLibrary.h"
#include "MainPlayer.generated.h"

class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ICEICE_API AMainPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMainPlayer();

	//mesh for player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
    USkeletalMesh* MeshComp;

	//niagara effect for the spikes
	UPROPERTY(EditAnywhere, Category = "Spawning")
		UNiagaraSystem* NiagaraEffect;


protected:
	virtual void BeginPlay() override;

	//functions reacting to input
	void MoveYAxis(float Value);
	void MoveXAxis(float Value);

private:
	//box for simple collision
	//UPROPERTY(EditAnywhere, Category = "HitComponents")
		//UBoxComponent* HitBox;

	//UFUNCTION()
		//void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Input override
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//landed override
	virtual void Landed(const FHitResult& Hit) override;

	//values to multiply the movement with
	UPROPERTY(EditAnywhere, Category = "MovementValue")
	float UpMultiplier;

	UPROPERTY(EditAnywhere, Category = "MovementValue")
	float SideMultiplier;

	//The spawned actor class (ice spike)
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AActor> ActorToSpawn;

	//list of icespike locations
	TArray<FVector> SpawnedIceSpikeLocations;
	TArray<AActor*> IceSpikes;

	//how many icespike locations are predefined
	int PreDefCount= 8;

	//predefined iceSpike locations
	TArray<FVector> IceSpikeLocations = {
		FVector(0.f,60.f,0.f),
		FVector(0.f,-60.f,0.f),
		FVector(60.f,0.f,0.f),
		FVector(-60.f,0.f,0.f),
		FVector(40.f,40.f,0.f),
		FVector(-40.f,30.f,0.f),
		FVector(-60.f,-40.f,0.f),
		FVector(60.f,-40.f,0.f)
	};

	//predefined rotations
	TArray<FRotator> IceSpikeRotations = {
		FRotator(0.f, 0.f, 30.0f),
		FRotator(0.0f, 0.f, -30.0f),
		FRotator(-30.0f, 0.f, 0.0f),
		FRotator(30.0f, 0.f, 0.0f),
		FRotator(-30.0f, 0.f, 30.0f),
		FRotator(30.0f, 0.f, 30.0f),
		FRotator(30.0f, 0.f, -30.0f),
		FRotator(-30.0f, 0.f, -30.0f),
	};

	//how widely the spikes are spawned and how many spikes are spawned
	const float SpawnRadius = 100;
	const int NumberOfSpikes = 30;

	//maximum amount to try to find new positions
	int MaxPosTries = 10;

	//timer properties for fading the ice spikes away
	FTimerHandle MemberTimerHandle;
	float Fadetime = 3.f;

	//are the old spikes still fading away
	bool CoolingDown = false;

	//for spawning ice spikes
	void SpawnIceSpikes();
	void SpawnSpike(FVector Location, FRotator Rotation, bool RandomSize);

	//random position, rotation, size for the spikes
	FVector FindRandomPosition(FVector PlayerLocation);
	FRotator RandomRotation();
	float RandomScale();

	//destroying spikes
	void DestroySpikes();

};
