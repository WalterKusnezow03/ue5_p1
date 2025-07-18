#pragma once
#include "CoreMinimal.h"
#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "AutoBoxBase.h"

#include "Hbox.generated.h"

class UPlayerUi;
class UHorizontalBox; //forward declare for faster compile 


/// @brief hbox wrapper to track custom visibility and click listening
UCLASS()
class P2_API UHbox : public UAutoBoxBase {
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