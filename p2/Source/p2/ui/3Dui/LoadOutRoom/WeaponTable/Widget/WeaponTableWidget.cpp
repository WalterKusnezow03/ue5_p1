#include "WeaponTableWidget.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/WeaponTableWidgetActor.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponTypeWidget.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/Options/WeaponAttachmentWidget.h"
#include "p2/weapon/enumUtil/WeaponAttachmentValidator.h"
#include "customuipluginbase/baseInterface/WidgetHelper.h"

#include "customuipluginbase/Dispatcher/Filter/WidgetFilter.h"



TArray<IBaseUiInterface *> UWeaponTableWidget::GetAllItemsForDispatch(){
    
    TArray<IBaseUiInterface *> items;
    WidgetFilter filter;
    filter.AppendDirectChildrenFromPanel<IBaseUiInterface>(GetPanelWeaponSwitch(), items);
    filter.AppendDirectChildrenFromPanel<IBaseUiInterface>(GetPanelSights(), items);
    filter.AppendDirectChildrenFromPanel<IBaseUiInterface>(GetPanelMuzzleAttachments(), items);
    filter.AppendDirectChildrenFromPanel<IBaseUiInterface>(GetPanelGripAttachments(), items);

    return items;
}

bool UWeaponTableWidget::dispatchHover(const FVector2D &position){

    return DispatchHoverToAllItemsForDispatch(position);
}

bool UWeaponTableWidget::dispatchClick(const FVector2D &screenPos){
    LogScreenCoordinate(screenPos);

    //check for all individual layouts

    //check weapon switch
    if(dispatchClick(screenPos, GetPanelWeaponSwitch())){
        
        return true;
    }

    //check weapon attachment change
    if(dispatchClick(screenPos, GetPanelSights())){
        return true;
    }
    if(dispatchClick(screenPos, GetPanelMuzzleAttachments())){
        return true;
    }
    if(dispatchClick(screenPos, GetPanelGripAttachments())){
        return true;
    }

    return false;
}

void UWeaponTableWidget::LogScreenCoordinate(const FVector2D &screenPos){
    const FGeometry& Geo = GetCachedGeometry();
    FVector2D localSize = Geo.GetLocalSize();

    FString message = FString::Printf(
        TEXT("UWeaponTableWidget Dispatch Click: %.2f %.2f of %.2f %.2f"),
        screenPos.X,
        screenPos.Y,
        localSize.X,
        localSize.Y
    );
    DebugHelper::logMessage(message);
    //DebugHelper::showScreenMessage(message);
}

bool UWeaponTableWidget::dispatchClick(const FVector2D &screenPos, UWidget *panel){
    
    if (!panel)
    {
        return false;
    }

    ClickDispatcher dispatcher;
    if(!dispatcher.InBound(panel, screenPos)){
        DebugHelper::logMessage("UWeaponTableWidget Dispatch Click Not In Panel Bounds!");
        return false;
    }else{
        DebugHelper::logMessage("UWeaponTableWidget Dispatch Click IS in panel bound!");
    }

    //check weapon switch
    UWeaponTypeWidget* found = 
    dispatcher.FindResultFromPanel<UWeaponTypeWidget>(
        panel, 
        screenPos
    );
    if(found){
        SelectWidgetFromPanel(panel, found);
        OnClickUpdateWeapon(found);
        DebugHelper::logMessage("UWeaponTableWidget Dispatch Click UWeaponTypeWidget!");
        return true;
    }

    //check weapon attachment change
    UWeaponAttachmentWidget* foundAttachment = 
    dispatcher.FindResultFromPanel<UWeaponAttachmentWidget>(
        panel, 
        screenPos
    );
    if(foundAttachment){
        SelectWidgetFromPanel(panel, foundAttachment);
        OnClickUpdateSetup(foundAttachment);
        DebugHelper::logMessage("UWeaponTableWidget Dispatch Click UWeaponAttachmentWidget!");
        return true;
    }
    DebugHelper::logMessage("UWeaponTableWidget Dispatch Click NONE!");
    return false;
}

void UWeaponTableWidget::SelectWidgetFromPanel(UWidget *panel, UWidget *found){
    if(panel && found){

        WidgetFilter filter;
        TArray<UWeaponOptionBaseWidget*> array = 
            filter.FindDirectChildrenFromPanel<UWeaponOptionBaseWidget>(panel);

        UWeaponOptionBaseWidget *casted = Cast<UWeaponOptionBaseWidget>(found);
        if(array.Num() > 0 && casted){
            if(array.Contains(casted)){
                for (int i = 0; i < array.Num(); i++){
                    if(UWeaponOptionBaseWidget *current = array[i]){
                        bool enabled = current == casted;

                        //log
                        FString message = enabled ? 
                        TEXT("UWeaponTableWidget option Enabled") :
                        TEXT("UWeaponTableWidget option Not Enabled");
                        DebugHelper::logMessage(message);

                        current->SetChecked(enabled);
                    }
                }
            }
        }
    }
}


void UWeaponTableWidget::InitWidgets(){

    InitOptionWidgetsFromPanel(GetPanelWeaponSwitch());

    InitOptionWidgetsFromPanel(GetPanelSights());
    InitOptionWidgetsFromPanel(GetPanelMuzzleAttachments());
    InitOptionWidgetsFromPanel(GetPanelGripAttachments());
    
}

void UWeaponTableWidget::InitOptionWidgetsFromPanel(UWidget *panel){
    if(panel){
        WidgetFilter filter;
        TArray<UWeaponOptionBaseWidget *> array =
            filter.FindDirectChildrenFromPanel<UWeaponOptionBaseWidget>(panel);

        for (int i = 0; i < array.Num(); i++){
            if(UWeaponOptionBaseWidget *current = array[i]){
                current->Init();
            }
        }
    }
}







void UWeaponTableWidget::OnClickUpdateSetup(UWeaponAttachmentWidget *widget){
    if(widget){
        OnClickUpdateSetup(widget->type);
    }
}

void UWeaponTableWidget::OnClickUpdateSetup(weaponAttachmentEnum type){
    if(setupHelper){
        setupHelper->SetAnyAttachmentFilterInternal(type);
        if(parentActorWidget){
            parentActorWidget->NotifyWeaponSetupChange();
        }
    }
}



void UWeaponTableWidget::OnClickUpdateWeapon(UWeaponTypeWidget *widget){
    if(widget){
        OnClickUpdateWeapon(widget->type);
    }
}

void UWeaponTableWidget::OnClickUpdateWeapon(weaponEnum type){
    if(setupHelper){
        setupHelper->setWeaponTypeToCreate(type);
        if(parentActorWidget){
            parentActorWidget->NotifyWeaponSetupChange();
        }
    }
    OnClickUpdateWeaponShowAttachmentMenus(type);
}

void UWeaponTableWidget::OnClickUpdateWeaponShowAttachmentMenus(weaponEnum type){
    bool visible = WeaponAttachmentValidator::CanHaveAttachments(type);
    WidgetHelper::SetVisible(GetPanelSights(), visible);
    WidgetHelper::SetVisible(GetPanelMuzzleAttachments(), visible);
    WidgetHelper::SetVisible(GetPanelGripAttachments(), visible);
}









void UWeaponTableWidget::SetWeaponSetupHelperRefernce(weaponSetupHelper *setup){
    setupHelper = setup;


    //update widget based on passed setup!

}


void UWeaponTableWidget::SetParentActor(AWeaponTableWidgetActor *parent){
    if(parent){
        parentActorWidget = parent;
    }
}