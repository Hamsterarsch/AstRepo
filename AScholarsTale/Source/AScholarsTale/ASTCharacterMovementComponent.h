#pragma once
#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ASTCharacterMovementComponent.generated.h"


UENUM(BlueprintType)
//Enum containing custom movement mode identifiers.
enum class EASTMovementMode : uint8
{
	None UMETA(DisplayName="None"),
	Gliding UMETA(DisplayName="Gliding"),
	Roping UMETA(DisplayName="Roping")


};


UCLASS()
//Extended movement component. 
//Includes gliding movment mode and roping movement mode,
//depends on ASTCharacter on movement mode changed
class ASCHOLARSTALE_API UASTCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	//Ignores set movement if NewMovementMode is flying and current mode is gliding,
	//upates the movement mode otherwise.
	//@param NewMovementMode: movement mode to transition to.
	//@param NewCustomMode: custom sub mode to transition to.
	virtual void SetMovementMode(EMovementMode NewMovementMode, uint8 NewCustomMode = 0) override;

	//Adds a force value that is included into the gliding velocity calculation.
	//Used to displace the character e.g. during gliding
	//@param ForceOffset: The amount of force to add to the force offset.
	void AddGlidingForceOffset(const FVector &ForceOffset) { m_GlidingForceOffset += ForceOffset; }


	UPROPERTY(EditAnywhere, DisplayName="Gliding Lift Amount", Meta = (Category="AST Character Movement: Gliding"))
		//The amount of lift to apply during gliding in percent relative to the force of gravity.
		float m_GlidingLiftAmount{ 0.9 };

	UPROPERTY(EditAnywhere, DisplayName = "Gliding Forward Drift", Meta = (Category = "AST Character Movement: Gliding"))
		//The unitless amount of force to apply towards the camera direction.
		float m_GlidingForwardDrift{ 700 };

	UPROPERTY(EditAnywhere, DisplayName = "Gliding Lean Speed", Meta = (Category = "AST Character Movement: Gliding"))
		//The unitless amount of velocity to apply during leaning forwards or backwards (symmetrically).
		float m_GlidingLeanSpeed{ 400 };

	UPROPERTY(EditAnywhere, DisplayName = "Gliding Turn Speed", Meta = (Category = "AST Character Movement: Gliding"))
		//The unitless amount of rotation to apply when steering left or right.
		float m_GlidingTurnSpeed{ 200 };
	
	UPROPERTY(EditAnywhere, DisplayName = "Roping Swing Force", Meta = (Category = "AST Character Movement: Roping"))
		//The the force to apply towards the input direction during roping.
		float m_RopingSwingForce{ 200 };

	//The world space position of the point we want to swing around during roping movement.
	FVector m_RopingAttachmentPoint;

	
protected:
	//Directs movement execution based on the current movement mode.
	//param DeltaTime: time since the last frame.
	//param Iterations: number of computation iterations already performed .
	virtual void PhysCustom(float DeltaTime, int32 Iterations) override;
		
	//Handles gliding movement.
	//param DeltaTime: time since the last frame.
	//param Iterations: number of computation iterations already performed .
	void PerformGlidingMovement(float DeltaTime, int32 Iterations);

	//Handles roping movement.
	//param DeltaTime: time since the last frame.
	//param Iterations: number of computation iterations already performed .
	void PerformRopingMovement(float DeltaTime, int32 Iterations);
	
	//The amount of force that is included in the gliding force computation.
	//(Used for airstream force application).
	FVector m_GlidingForceOffset{ EForceInit::ForceInitToZero };
	

};
