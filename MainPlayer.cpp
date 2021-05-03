//
//Player controll class
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//References:
//1.Dev Enabled - C++ Playlist Overview UE4 / Unreal Engine 4 C++ :https://www.youtube.com/watch?v=b7qC1ogIFpk
//2.Player Input and Pawns. (2021). Unreal Engine Documentation. https://docs.unrealengine.com/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/PlayerInput/index.html
//3.aioobe. Generate a random point within a circle (uniformly). https://stackoverflow.com/questions/5837572/generate-a-random-point-within-a-circle-uniformly
//
#include "MainPlayer.h"
#include "Math/UnrealMathUtility.h"
#include "IceStick.h"
#include "TimerManager.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"



AMainPlayer::AMainPlayer()
{
	PrimaryActorTick.bCanEverTick = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Mesh
	MeshComp = CreateDefaultSubobject<USkeletalMesh>(TEXT("PlayerMesh"));

	//MeshComp->SetupAttachment(RootComponent);

	//collider box
	//HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));

	//dynamic binding for the hit function
	//HitBox->OnComponentHit.AddDynamic(this, &AMainPlayer::OnHit);
	//Landed(const FHitResult & Hit)

	//set multiplier default values
	UpMultiplier = 1.0f;
	SideMultiplier = 1.0f;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

}

// Inputs
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Jumping: ready in the character class
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//Movement: own fuctions called
	PlayerInputComponent->BindAxis("MoveY", this, &AMainPlayer::MoveYAxis);
	PlayerInputComponent->BindAxis("MoveX", this, &AMainPlayer::MoveXAxis);

}

//movement in up/down direction, used ref.1 Dev Enabled as a code example 
void AMainPlayer::MoveYAxis(float Value)
{
	if (Controller && Value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value * UpMultiplier);
	}
}

//movement in left/right direction, used ref.1 Dev Enabled as a code example 
void AMainPlayer::MoveXAxis(float Value)
{
	if (Controller && Value != 0.0f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value * SideMultiplier);
	}
}

//Overriders the landed function from character class
void AMainPlayer::Landed(const FHitResult& Hit){
	Super::Landed(Hit);

	//if old spikes are not gone yet -> destroy them
	if (CoolingDown) {
		DestroySpikes();
	}

	//spawn icespikes
    SpawnIceSpikes();
}

/*called when character hits the ground
void AMainPlayer::OnHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse,
	const FHitResult& Hit) {

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
}*/


//spawning all the spikes
void AMainPlayer::SpawnIceSpikes()
{

	//get player location
	FVector PlayerLocation = GetActorLocation();

	//spawn predefined spikes
	for (int i = 0; i < PreDefCount; i++) {
		if (IceSpikeRotations.Num() >= i && IceSpikeLocations.Num() >= i) {
			FVector Location = PlayerLocation + IceSpikeLocations[i];
			FRotator Rotation = IceSpikeRotations[i];
			SpawnSpike(Location, Rotation, false);
		}
	}

	//spawn rest of the spikes
	for (int i = PreDefCount; i < NumberOfSpikes; i++) {

		FVector Location = FVector(0.f,0.f,0.f);
		int tries = 0;
		bool posNotFound = true;

		//try to find location -> might notbe needed since player not might notice this (if wanted could be taken out for performance)
		while (tries < MaxPosTries && posNotFound) {

			//Find location 
			Location = FindRandomPosition(PlayerLocation);

			//location not taken
			if (!SpawnedIceSpikeLocations.Contains(Location)) {
				posNotFound = false;
			}
			tries++;
		}

		//found location -> spawn spike
		if (!posNotFound) {
			FRotator Rotation = RandomRotation();
			SpawnSpike(Location, Rotation, true);
		}

	}

	//timer for the spikes to be destroyed
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AMainPlayer::DestroySpikes, Fadetime, false);
	CoolingDown = true;

	//spawn the niagara effect 
	FRotator Rotation = FRotator(0.0f, 0.0f, 0.0f);
	UNiagaraComponent* effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, PlayerLocation, Rotation);

}

//returns a random rotation
FRotator AMainPlayer::RandomRotation() {
	float XRot = FMath::RandRange(-40.0f, 40.f);
	float ZRot = FMath::RandRange(-40.0f, 40.f);
	return FRotator(XRot, 0.0f, ZRot);
}


//returns new scaler value 
float AMainPlayer::RandomScale() {
	return FMath::RandRange(0.4f,0.7f);
}

//Finding random spawning spot within a circle radius of the player
//uses ref.3 aioobe: advice to generate coordinates within circle
FVector AMainPlayer::FindRandomPosition(FVector PlayerLocation)
{
	float r = SpawnRadius * sqrt(FMath::RandRange(0.f, 1.f));
	float theta = FMath::RandRange(0.f, 1.f) * 2 * PI;

	float x = PlayerLocation.X + r * cos(theta);
	float y = PlayerLocation.Y + r * sin(theta);

	return FVector(x,y, PlayerLocation.Z);
}

//spawning one spike (location, rotation, randomsize= do we add random size to the spawned actor)
void AMainPlayer::SpawnSpike(FVector Location, FRotator Rotation, bool RandomSize)
{
	//add location to store
	SpawnedIceSpikeLocations.Add(Location);

	//spawn new actor
	FActorSpawnParameters SpawnParameters;
	AIceStick* SpawnedActorRef = GetWorld()->SpawnActor<AIceStick>(ActorToSpawn, Location, Rotation, SpawnParameters);

	//for the ones that are not predefined -> set random size
	if (RandomSize) {
		SpawnedActorRef->SetSize(RandomScale());
	}

	//add to spike list of spawned actors
	IceSpikes.Add(SpawnedActorRef);
}


//destroy all the spawned spikes
void AMainPlayer::DestroySpikes()
{
	//cooldown done
	CoolingDown = false;

	//clear timer
	GetWorld()->GetTimerManager().ClearTimer(MemberTimerHandle);

	//detroy objects and empty list
	for (AActor* Actor : IceSpikes)
	{
	  Actor->Destroy();
	}
	IceSpikes.Empty();

}
