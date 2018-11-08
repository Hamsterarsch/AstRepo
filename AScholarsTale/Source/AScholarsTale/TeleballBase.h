// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Slingable.h"
#include "TeleballBase.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegateVoid);



UCLASS()
class ASCHOLARSTALE_API ATeleballBase : public AActor, public ISlingable
{
	GENERATED_BODY()
	
public:	
	ATeleballBase();

	virtual void Launch(FVector Velocity) override;
	
	void OnTeleballRecast();

	void OnTeleportTo();
	   

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable)
		void StopMovement();

	UFUNCTION(BlueprintImplementableEvent, Category="Telebal|Events", Meta = (DisplayName="OnTeleballRecast"))
		void ReceiveOnTeleballRecast();

	UFUNCTION(BlueprintImplementableEvent, Category="Teleball|Events", Meta = (DisplayName="OnTeleportTo"))
		void ReceiveOnTeleportTo();

	UPROPERTY(VisibleAnywhere, Meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent *m_pProjectileMovement;
	
	
};
