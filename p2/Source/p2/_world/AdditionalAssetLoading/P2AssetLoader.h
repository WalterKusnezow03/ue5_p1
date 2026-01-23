
#pragma once

#include "CoreMinimal.h"


class P2_API P2AssetLoader {

public:
    P2AssetLoader(){}
    ~P2AssetLoader(){}

    void LoadGameAssets();

private:
    void loadDebugCube();

    void loadMaterials(); //materials need to be loaded first to not have any issues
    void loadEntities();
    void loadWeapons();
    void loadWeaponAttachments();
    void loadThrowables();
    void loadParticles();
    void loadTextures();

    void loadGameStartRoomAssets();
    void loadLoadoutRoomAssets();
    void load3DWidgets();
};
