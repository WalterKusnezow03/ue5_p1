#pragma once

#include "CoreMinimal.h"

UENUM()
enum class EAssetType
{
    EUndefined,
    EUTexture2D,
    EUClassBlueprint,
    EUMaterial,
    EUStaticMesh,
    EUAnimSequence,
};