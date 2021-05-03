//
//Player animation class.
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//Refrences used:
//1.Flopperam-How to create a AnimInstance in Unreal Engine using C++ (Part 1/2). (2019, April 22). [Video]. YouTube. https://www.youtube.com/watch?v=Zxjf4efpgGY
//2.Flopperam-How to create a AnimInstance in Unreal Engine using C++ (Part 2/2). (2019, April 23). [Video]. YouTube. https://www.youtube.com/watch?v=ySeBG_X8Wwg
#include "PlayerAnimation.h"
#include "MainPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerAnimation::UPlayerAnimation()
{
	Speed = 0.0f;
	IsInAir = false;
}

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//get the actor class of player character 
	AActor* owningActor = GetOwningActor();

	if (owningActor != nullptr) {

		//set speed based on character vcelocity
		Speed = owningActor->GetVelocity().Size();

		//cast to character class
		AMainPlayer* OwningCharacter = Cast<AMainPlayer>(owningActor);
		if (OwningCharacter != nullptr) {

			//check if the character is falling
			IsInAir = OwningCharacter->GetCharacterMovement()->IsFalling();
		}
	}
}
