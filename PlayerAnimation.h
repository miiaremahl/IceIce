//
//Player animation header.
//Miia Remahl 27.4.2021, mrema003@gold.ac.uk
//
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimation.generated.h"

UCLASS(transient, Blueprintable, hideCategories=AnimInstance, BlueprintType)
class UPlayerAnimation: public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimation();

    //speed of character (used for movement)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;

	//is character in air (used for jump)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
	bool IsInAir;

	//override native update function
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
