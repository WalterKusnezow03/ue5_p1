#pragma once
#include "CoreMinimal.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
#include "AutoBoxBase.h"
#include "Components/VerticalBox.h"


#include "Vbox.generated.h"

/// @brief vbox wrapper to track custom visibility and click listening
UCLASS()
class CUSTOMUIPLUGIN_API UVbox : public UAutoBoxBase {
    GENERATED_BODY()

public:
    
    virtual void init() override;
    virtual UWidget *baseLayoutPointer() override{
        return baseVBox; //button //scalebox(wrong)
    }

    virtual void AddChild(UcustomUiComponentBase *item) override;

    /// @brief adds ANY child to the vbox, no custom click or visibility listening
    /// @param any 
    virtual void AddChild(UWidget *any) override;

    //not needed here: happens in auto box parent!
    //virtual void setVisible(bool flag) override;

private:
    void initVboxIfNeeded();
    UVerticalBox *baseVBox = nullptr;
};