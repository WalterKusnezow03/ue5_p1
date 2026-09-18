#pragma once

#include "FMiniMapMarkerTransform.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"

//will provide a transform and custom image data if needed
class P2_API FMiniMapMarkerSetup {


public:

    FMiniMapMarkerTransform &GetTransform(){
        return transform;
    }

    const FMiniMapMarkerTransform &GetTransformConst() const {
        return transform;
    }

    

    void UpdateImageDataPtr(Image *ptr, float sizeMaxMinimap){
        UpdateImageDataPtr(ptr);
        sizeOfWidget = SizeOfImage(sizeMaxMinimap);
        /*DebugHelper::logMessage(
            FString::Printf(
                TEXT("FMiniMapMarkerSetup size update %.2f"), sizeOfWidget
            )
        );*/
    }

    const Image *GetImageDataConst() const {
        return imageData;
    }

    bool HasImageData() const {
        return imageData != nullptr;
    }

    //use if has image data
    float GetSizeOfWidgetDesired() const {
        return sizeOfWidget;
    }

private:
    FMiniMapMarkerTransform transform;
    Image *imageData = nullptr; //is not owned!
    float sizeOfWidget = 1.0f;

    void UpdateImageDataPtr(Image *ptr){
        imageData = ptr;
    }

    float SizeOfImage(float maxSizeMap){
        if(HasImageData()){
            //scalar = distTarget / distAll

            float maxSide = std::max(imageData->widthX(), imageData->heightY()) * imageData->GetUnitStepPerPixel();
            return maxSide / std::abs(maxSizeMap);


            //return imageData->GetUnitStepPerPixel() / std::abs(maxSizeMap);
        }
        return 1.0f;
    }
};