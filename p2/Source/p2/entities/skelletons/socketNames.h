// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * stores the socket names for each item consistently, 
 * also is an reference o naming sockets properly
 */
class P2_API socketNames
{
public:
	socketNames();
	~socketNames();

	static const FName rightHandSocket;
	static const FName leftHandSocket;
};
