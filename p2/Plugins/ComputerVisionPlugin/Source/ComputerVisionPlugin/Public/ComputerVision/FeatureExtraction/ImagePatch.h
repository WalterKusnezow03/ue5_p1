#pragma once 

#include "CoreMinimal.h"


class COMPUTERVISIONPLUGIN_API ImagePatch {

public:
    ImagePatch();
    ~ImagePatch();

    ImagePatch(const ImagePatch &other);
    ImagePatch &operator=(const ImagePatch &other);

    void SavePatch(TArray<FColor> &colorIn, int sizePatchX, int sizePatchY, int32 idIn);



    // ------ TODO SIFT ZU ENDE MACHEN ! ------
    //TODO: Compute Orientation before Copy Patch (To in blurred image DOG?)
    //TODO: Remove Orientation to match the player images (e.x. every image to compare!)

    void ScaleDown(float scalePixels);

    //debug
    void SaveToStorage();

    void OverrideId(int32 idIn){
        id = idIn;
    }

private:
    int ToIndex(int i, int j);
    FColor Sample(int iStart, int jStart, int iEnd, int jEnd);

    TArray<FColor> colorSaved;

    int sizeXSaved = 1;
    int sizeYSaved = 1;
    int32 id;

    
};