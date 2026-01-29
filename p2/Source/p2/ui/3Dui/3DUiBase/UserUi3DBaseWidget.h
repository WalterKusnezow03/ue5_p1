#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/baseInterface/InterfaceHelper/BorderInterfaceUtil.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"


#include "UserUi3DBaseWidget.generated.h"


UCLASS()
class P2_API UUserUi3DBaseWidget : public UUserWidget {
    GENERATED_BODY()


public:
    void LoadDefaultColors(
        FLinearColor &colorBaseOut,
        FLinearColor &colorCheckedOut,
        FLinearColor &colorHoverOut
    );

    //set ups border widget and util with found asset data
    void SetupFromDefaultColors(
        BorderInterfaceUtil &util,
        UWidget *borderWidget
    );

protected:
    //doesnt have to be overriden but can be.
    virtual TArray<IBaseUiInterface *> GetAllItemsForDispatch();
    bool DispatchHoverToAllItemsForDispatch(const FVector2D &pos);

private:
    


};