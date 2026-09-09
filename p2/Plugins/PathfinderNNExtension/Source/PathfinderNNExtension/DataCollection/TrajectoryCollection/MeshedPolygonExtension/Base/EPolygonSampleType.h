#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EPolygonSampleType
{
    none,
	EMeshedPolygonTrajectoryLayered, //for rendered image 142 x 142 (netB)
    EMeshedPolygonTrajectoryRayModel //for ray model (netC)
};
