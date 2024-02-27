// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChoppalbeTree_OJS.generated.h"

UCLASS()
class TIVA_API AChoppalbeTree_OJS : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChoppalbeTree_OJS();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
