#pragma once

#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"

class CUSTOMUIPLUGINBASE_API BorderInterfaceUtilButton : public BorderInterfaceUtil {

public:
    BorderInterfaceUtilButton();
    ~BorderInterfaceUtilButton();

    void Tick(float deltatime);
    virtual bool dispatchClick(const FVector2D &position) override;

protected:
    float resetTime = 0.5f;
    float timeLeft = 0.0f;
};