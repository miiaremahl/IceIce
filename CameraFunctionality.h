//
//Camera control header.
//Miia Remahl 2.4.2021, mrema003@gold.ac.uk
//Used as a reference: Unreal Engine C++ Tutorial #5 - Cameras. (2015, December 9). [Video]. YouTube. https://www.youtube.com/watch?v=4nFMxKkbMXM
//Used for binding inputs to actor: Moiz, M. (2021). Binding Input in C++ with Unreal Engine 4 | Unrealistic. Unrealistic. https://unrealistic.dev/posts/binding-input-in-c
//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraFunctionality.generated.h"

UCLASS()
class ICEICE_API ACameraFunctionality : public AActor
{
	GENERATED_BODY()
	
public:	
	ACameraFunctionality();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void TurnOnCamera(int Number);

	//for binding input actions
    void BindActionBindings(FInputActionHandlerSignature& Handler, FName ActionName);

public:	
	virtual void Tick(float DeltaTime) override;

	//cameras
	UPROPERTY(EditAnywhere)
		AActor* CameraOne;

	UPROPERTY(EditAnywhere)
		AActor* CameraTwo;

	UPROPERTY(EditAnywhere)
		AActor* CameraTree;

	//timings
	float SmoothTime = 0.75f;
	//float NextCameraTimer;
	//float ChangeTime = 3.0f;

	//input binding
	void BindToInput();
};
