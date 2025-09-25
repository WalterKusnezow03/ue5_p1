#pragma once

#include "CoreMinimal.h"

class TERRAINPLUGIN_API TerrainPluginAssetLoader {

#if WITH_EDITOR
public:
    static void InitOnLaunchModule();

private:
    static void InitGrassAssets();
#endif
};