// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Obstacles.generated.h"

UINTERFACE(MinimalAPI)
class UObstacles : public UInterface
{
	GENERATED_BODY()
};


class UNREALPROJECT10_API IObstacles
{
	GENERATED_BODY()

public:

	virtual void Init() = 0;

	virtual void OnHitByBird() = 0;

	virtual void OnHitByPig() = 0;
};
