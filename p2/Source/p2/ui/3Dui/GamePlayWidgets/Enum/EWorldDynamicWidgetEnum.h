#pragma once

#include "CoreMinimal.h"

///enum for all dynamic world widgets which can be collected by entity manager object pool
///is different than ui 3D assets on purpose here.
UENUM()
enum class EWorldDynamicWidgetEnum
{
    ENone,
    EInteractWidget,
};