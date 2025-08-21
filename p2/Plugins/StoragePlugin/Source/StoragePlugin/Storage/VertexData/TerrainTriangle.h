#pragma once
#include "CoreMinimal.h"

/// 12 + 12 + 8 = 32 byte
#pragma pack(push, 1) // keine Padding-Bytes
struct STORAGEPLUGIN_API FTerrainTriangle
{
    int32[3] indices; //4 bytes *3 = 12 bytes
};
#pragma pack(pop)