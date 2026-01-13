#include "WeaponTableWidget.h"
#include "p2/ui/3Dui/LoadOutRoom/WeaponTable/Widget/WeaponTableWidgetActor.h"



void UWeaponTableWidget::OnClickUpdateSetup(weaponAttachmentEnum type){
    if(setupHelper){
        setupHelper->SetAnyAttachmentFilterInternal(type);
        if(parentActorWidget){
            parentActorWidget->NotifyWeaponSetupChange();
        }
    }
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