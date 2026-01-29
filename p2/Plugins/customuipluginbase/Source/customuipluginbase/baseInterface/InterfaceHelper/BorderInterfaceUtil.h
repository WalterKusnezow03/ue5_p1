#pragma once

#include "CoreMinimal.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "Components/Border.h"



/// @brief selectable button which can be hovered
class CUSTOMUIPLUGINBASE_API BorderInterfaceUtil {

public:
    BorderInterfaceUtil();
    virtual ~BorderInterfaceUtil();

    void Init(UWidget *borderWidget);
    void Init(
        UWidget *borderWidget, 
        FLinearColor colorBaseIn,
        FLinearColor colorCheckedIn,
        FLinearColor colorHoverIn
    );

    FLinearColor colorBase = FLinearColor::Black;
    FLinearColor colorChecked = FLinearColor::Red;
    FLinearColor colorHover = FLinearColor::Yellow;


    //override
    virtual bool dispatchClick(const FVector2D &position);
    bool dispatchHover(const FVector2D &position);
   
    bool IsChecked(){
        return bIsChecked;
    }
    void SetChecked(bool flag);
    void ResetCheckedState();

protected:
    
    void SetHovered(bool flag);
    void SetDefault();

    bool bIsChecked = false;
    bool bIsHovered = false;

    UWidget *rawWidget = nullptr;
    UBorder *borderWidget = nullptr;

    bool InBound(const FVector2D &position);




};