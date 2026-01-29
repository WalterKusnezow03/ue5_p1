#include "UserUi3DBaseWidget.h"
#include "AssetEnumCollection/assetEnums/EDataAssetEnum.h"
#include "customuipluginbase/DataAsset/ColorAsset/ColorAsset.h"
#include "DebugPlugin/DebugHelper.h"

void UUserUi3DBaseWidget::LoadDefaultColors(
    FLinearColor &colorBaseOut,
    FLinearColor &colorCheckedOut,
    FLinearColor &colorHoverOut
){
    if(assetManager *a = assetManager::instance()){
        UPrimaryDataAsset *raw = 
            a->Find<EDataAssetEnum, UPrimaryDataAsset>(EDataAssetEnum::EUiColorAsset);
        if(raw){
            if(UColorAsset *casted = Cast<UColorAsset>(raw)){
                casted->CopyColors(
                    colorBaseOut,
                    colorCheckedOut,
                    colorHoverOut
                );
                DebugHelper::logMessage("UUserUi3DBaseWidget::Asset Setup");
            }
        }else{
            DebugHelper::logMessage("UUserUi3DBaseWidget::Asset Setup Failed, Not Found");
        }
    }
}

void UUserUi3DBaseWidget::SetupFromDefaultColors(
    BorderInterfaceUtil &util,
    UWidget *borderWidget
){
    FLinearColor colorBaseIn(0,0,0,0);
    FLinearColor colorCheckedIn(0,0,0,0);
    FLinearColor colorHoverIn(0,0,0,0);
    LoadDefaultColors(colorBaseIn, colorCheckedIn, colorHoverIn);
    util.Init(
        borderWidget, 
        colorBaseIn,
        colorCheckedIn,
        colorHoverIn
    );
}


TArray<IBaseUiInterface *> UUserUi3DBaseWidget::GetAllItemsForDispatch(){
    TArray<IBaseUiInterface *> outArray;
    return outArray;
}

bool UUserUi3DBaseWidget::DispatchHoverToAllItemsForDispatch(const FVector2D &pos){
    TArray<IBaseUiInterface *> array = GetAllItemsForDispatch();
    bool result = false;
    for (int i = 0; i < array.Num(); i++){
        if(IBaseUiInterface *current = array[i]){
            if(current->dispatchHover(pos)){
                result = true;
            }
        }
    }
    return result;
}