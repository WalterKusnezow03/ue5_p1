#pragma once
#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "AutoBoxBase.h"
#include "Components/HorizontalBox.h"

#include "Hbox.generated.h"



/// @brief hbox wrapper to track custom visibility and click listening
UCLASS()
class CUSTOMUIPLUGIN_API UHbox : public UAutoBoxBase {
    GENERATED_BODY()

public:
    
    virtual void init() override;
    virtual UWidget *baseLayoutPointer() override{
        return baseHBox; //button //scalebox(wrong)
    }

    virtual void AddChild(UcustomUiComponentBase *item) override;

    //not needed here: happens in auto box parent!
    //virtual void setVisible(bool flag) override;

    /// @brief adds any child, not in click listening!
    /// @param any
    virtual void AddChild(UWidget *any);

private:
    void initHboxIfNeeded();
    UHorizontalBox *baseHBox = nullptr;
};