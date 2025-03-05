// Fill out your copyright notice in the Description page of Project Settings.


#include "Birds.h"

// Add default functionality here for any IBirds functions that are not pure virtual.

void IBirds::Init()
{
}

void IBirds::TakeDamage(float DamageAmount)
{
	OnDeath();
}

void IBirds::OnDeath()
{
}

float IBirds::GetBirdVelocity() const
{
	return BirdVelocity;
}

float IBirds::SetBirdVelocity(float NewBirdVelocity)
{
	return BirdVelocity = NewBirdVelocity;
}

float IBirds::GetBirdMass() const
{
	return BirdMass;
}

float IBirds::SetBirdMass(float NewBirdMass)
{
	return BirdMass = NewBirdMass;
}
