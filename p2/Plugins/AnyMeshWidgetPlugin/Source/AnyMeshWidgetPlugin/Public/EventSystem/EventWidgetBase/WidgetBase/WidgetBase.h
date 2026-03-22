// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "Blueprint/UserWidget.h"

#include "DebugPlugin/DebugHelper.h"

#include "WidgetBase.generated.h"

/// @brief ----- BASE CLASS FOR WIDGET ------
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UWidgetBase : public UUserWidget {
    GENERATED_BODY()
protected:
    bool bIsSetVisible = true;


    void TickAll(float deltatime);
    void HideAll();


    //important for base widgets dispatching events, if panel not Root
    UFUNCTION(BlueprintImplementableEvent, Category = "_DispatchBaseOption")
    UPanelWidget *FindPanelWidgetIfNotRoot();



public:
    //called on Event Dispatcher register widget! - very important for animation widget!
    // inits all childs recursive!
    virtual void Init();

    virtual void TickExternal(float deltatime);

    virtual void SetVisible(bool flag);

    static void SetVisible(UWidget *any, bool visible){
        if(any != nullptr){
            //ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

            ESlateVisibility newStatus = visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
            // ESlateVisibility::Hidden;
            any->SetVisibility(newStatus);

            //test
            any->SetIsEnabled(visible);

        }
    }


    template<typename T>
    TArray<T*> TGetDirectChildren(){
        TArray<T *> outArray;
        TArray<UWidget *> raw = GetDirectChildren();
        for (int i = 0; i < raw.Num(); i++){
            if(UWidget *current = raw[i]){
                if(T *casted = Cast<T>(current)){
                    outArray.Add(casted);
                }
            }
        }
        DebugHelper::logMessage("UWidgetBase::TGetDirectChildren() ", outArray.Num());
        return outArray;
    }

    TArray<UWidget *> GetDirectChildren(UPanelWidget *panel);
    TArray<UWidget *> GetDirectChildren();
};
