// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Birds.generated.h"

UINTERFACE(MinimalAPI)
class UBirds : public UInterface
{
	GENERATED_BODY()
};


class UNREALPROJECT10_API IBirds
{
	GENERATED_BODY()

public:
	virtual void Init() = 0;

	virtual void TakeDamage(float DamageAmount) = 0;

	virtual void OnDeath() = 0;

};
