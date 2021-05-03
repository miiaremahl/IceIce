//
//Camera control class.
//Miia Remahl 2.4.2021, mrema003@gold.ac.uk
//Used as a reference: Unreal Engine C++ Tutorial #5 - Cameras. (2015, December 9). [Video]. YouTube. https://www.youtube.com/watch?v=4nFMxKkbMXM
//Used for binding inputs to actor: Moiz, M. (2021). Binding Input in C++ with Unreal Engine 4 | Unrealistic. Unrealistic. https://unrealistic.dev/posts/binding-input-in-c
//Used for parameters/input bindings : Oldsiren. (2018). C++ Binding Action’s with parameters Unreal Engine 4 [Video]. YouTube. https://www.youtube.com/watch?v=gpvmxGb2Yt0
//Used ItsaMeTuni for binding help: Unbind an action from the input component. Unreal Engine Forums. https://forums.unrealengine.com/t/how-to-unbind-an-action-from-the-input-component/119310
//
#include "CameraFunctionality.h"
#include "kismet/GamePlayStatics.h"

ACameraFunctionality::ACameraFunctionality()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACameraFunctionality::BeginPlay()
{
	Super::BeginPlay();

	//set first camera so avoid smoothing at beginning
	APlayerController* OurPlayer = UGameplayStatics::GetPlayerController(this, 0);
	OurPlayer->SetViewTarget(CameraOne);

	BindToInput();

	//creating delegate to connect input binding
	FInputActionHandlerSignature ActionHandler;

	//Set action bindings for camera1, camera2, camera3 (keys:1,2,3)
	ActionHandler.BindUFunction(this, TEXT("TurnOnCamera"), 1);
	BindActionBindings(ActionHandler, TEXT("Camera1"));

	ActionHandler.BindUFunction(this, TEXT("TurnOnCamera"), 2);
	BindActionBindings(ActionHandler, TEXT("Camera2"));

	ActionHandler.BindUFunction(this, TEXT("TurnOnCamera"), 3);
	BindActionBindings(ActionHandler, TEXT("Camera3"));

}

//Binding the input actions
void ACameraFunctionality::BindActionBindings(FInputActionHandlerSignature& Handler, FName ActionName)
{
	//binding delegate to action
	FInputActionBinding ActionBinding;
	ActionBinding = FInputActionBinding(ActionName, IE_Pressed);

	//ActionBinding.actionName= TEXT("Camera1");
	//ActionBinding.KeyEvent = IE_Pressed;

	//connecting delegate to input
	ActionBinding.ActionDelegate = Handler;

	//add bindings
	InputComponent->AddActionBinding(ActionBinding);

}

void ACameraFunctionality::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//take time from cameratimer
	//NextCameraTimer -= DeltaTime;
 
	/*if (NextCameraTimer <= 0.0f) {
		NextCameraTimer = ChangeTime;

		//get current gameplaystatistics
		APlayerController* CurrPlayer = UGameplayStatics::GetPlayerController(this, 0);

		//change camera with smooth time
		//getViewtarget() -> Get the actor the controller is looking at
		//SetViewTargetWithBlend()-> Set the view target blending with variable control
		if (CurrPlayer) {
			if (CameraTwo && (CurrPlayer->GetViewTarget() == CameraOne)) {
				CurrPlayer->SetViewTargetWithBlend(CameraTwo, SmoothTime);
			}
			else if (CameraTree && (CurrPlayer->GetViewTarget() == CameraTwo)) {
				CurrPlayer->SetViewTargetWithBlend(CameraTree, SmoothTime);
			}
			else if (CameraOne) {
				CurrPlayer->SetViewTargetWithBlend(CameraOne, SmoothTime);
			}
		}
	}*/
}

//Turn on camera based on user input (camera number given as parameter)
void ACameraFunctionality::TurnOnCamera(int Number)
{
	//get current gameplaystatistics
	APlayerController* CurrPlayer = UGameplayStatics::GetPlayerController(this, 0);

	//getViewtarget() -> Get the actor the controller is looking at
	//SetViewTargetWithBlend()-> Set the view target blending with variable control
	if (CurrPlayer) {

		//change between cameras
		switch (Number) {
			case 1:
				if (CameraOne && (CurrPlayer->GetViewTarget() != CameraOne)) {
					CurrPlayer->SetViewTargetWithBlend(CameraOne, SmoothTime);
				}
				break;
			case 2:
				if (CameraTwo && (CurrPlayer->GetViewTarget() != CameraTwo)) {
					CurrPlayer->SetViewTargetWithBlend(CameraTwo, SmoothTime);
				}
				break;
			case 3:
				if (CameraTree && (CurrPlayer->GetViewTarget() != CameraTree)) {
					CurrPlayer->SetViewTargetWithBlend(CameraTree, SmoothTime);
				}
				break;
		}
	}
}

//Connecting input to player controller
void ACameraFunctionality::BindToInput()
{
	// Initializing input component / registering
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();

	if (InputComponent)
	{
		//InputComponent->BindAction("Camera1", IE_Pressed, this, &ACameraFunctionality::TurnOnCamera);

		// Hooking into player controller
		EnableInput(GetWorld()->GetFirstPlayerController());
	}
}

