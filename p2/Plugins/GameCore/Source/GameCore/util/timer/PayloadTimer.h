// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/timer/PayloadInterface/IPayloadInterface.h"

#include "GameCore/util/timer/PayloadInterface/Payload.h"

/**
 * 
 */
class GAMECORE_API PayloadTimer : public timer
{
public:
	PayloadTimer();
	virtual ~PayloadTimer();

	virtual bool TickWithTimesUpReset(float deltaTime) override;
    void AddPayload(Payload &load);

    void NotifyAll();

private:
    TArray<Payload> executeOnFinish;
};
