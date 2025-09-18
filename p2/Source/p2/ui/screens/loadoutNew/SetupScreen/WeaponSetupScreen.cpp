#include "WeaponSetupScreen.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
#include "p2/weapon/enumUtil/WeaponAttachmentValidator.h"
#include "p2/weapon/enumUtil/WeaponEnumStringConverter.h"

//payloads will be handled on click.
#include "p2/ui/screens/loadoutNew/SetupScreen/PayLoadDerived/WeaponAttachmentPayLoad.h"
#include "p2/ui/screens/loadoutNew/SetupScreen/PayLoadDerived/WeaponTypePayLoad.h"



/// @brief passed weapon setup helper will be hold by ptr and updated!
void UWeaponSetupScreen::UpdateScreen(weaponSetupHelper &setup){
    referencedSetup = &setup;

    if(referencedSetup){
        ReversePick(referencedSetup->getWeaponTypeToCreate());
        ReversePick(referencedSetup->getSightTypeToCreate());
        ReversePick(referencedSetup->getGripTypeToCreate());
        ReversePick(referencedSetup->getMuzzleTypeToCreate());
        UpdateWeaponAndAttachmentDisplay();
    }
}

void UWeaponSetupScreen::OnOpenScreen(){
    CloseAllPickerMenus();
}

/// @brief will update the images with the attachments and all.
void UWeaponSetupScreen::UpdateWeaponAndAttachmentDisplay(){

}



void UWeaponSetupScreen::ReversePick(weaponEnum typeWeapon){
    if(weaponPicker){
        const TArray<IBaseUiInterface *> &listItems = weaponPicker->AccessListInternalItemsTmp();
        int index = FindIndexFromPayloadData(
            listItems,
            typeWeapon
        );
        if(index >= 0 && index < listItems.Num()){
            weaponPicker->SelectIndex(index);
        }
    }
}

///@brief finds index from list items, where the payload is a weapon payload
/// returns -1 if failed.
int UWeaponSetupScreen::FindIndexFromPayloadData(
    const TArray<IBaseUiInterface *> &listItems,
    weaponEnum typeWeaponSearched
){
    for (int i = 0; i < listItems.Num(); i++){
        IBaseUiInterface *currentPtr = listItems[i];
        if(currentPtr){
            UPayLoadBase *payload = currentPtr->GetPayLoad();
            if(payload){
                //test all payload types.

                //try find weapon
                UWeaponTypePayLoad *castedWeaponPayload = Cast<UWeaponTypePayLoad>(payload);
                if(castedWeaponPayload){
                    weaponEnum read = castedWeaponPayload->GetType();
                    if(read == typeWeaponSearched){
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}


void UWeaponSetupScreen::ReversePick(weaponAttachmentEnum typeAttachment){
    TArray<UVerticalDropDownBase *> attachmentPickers = AttachmentLists();
    for (int i = 0; i < attachmentPickers.Num(); i++){
        UVerticalDropDownBase *currentAttachmentPicker = attachmentPickers[i];
        if(currentAttachmentPicker){
            
            //try to find the index from interfaces, having the payload with the 
            //searched type and select it. 
            const TArray<IBaseUiInterface *> &listItems = 
                currentAttachmentPicker->AccessListInternalItemsTmp();
            int index = FindIndexFromPayloadData(
                listItems,
                typeAttachment
            );
            if (index >= 0 && index < listItems.Num())
            {
                currentAttachmentPicker->SelectIndex(index);
                return;
            }
        }
    }
}

int UWeaponSetupScreen::FindIndexFromPayloadData(
    const TArray<IBaseUiInterface *> &listItems,
    weaponAttachmentEnum typeAttachmentSearched
){
    for (int i = 0; i < listItems.Num(); i++){
        IBaseUiInterface *currentPtr = listItems[i];
        if(currentPtr){
            UPayLoadBase *payload = currentPtr->GetPayLoad();
            if(payload){
                //test all payload types.

                //try find weapon
                UWeaponAttachmentPayLoad *attachmentPayload = Cast<UWeaponAttachmentPayLoad>(payload);
                if(attachmentPayload){
                    weaponAttachmentEnum read = attachmentPayload->GetType();
                    if(read == typeAttachmentSearched){
                        return i;
                    }
                }
            }
        }
    }
    return -1;
}









// ------- SETUP PICKERS --------

void UWeaponSetupScreen::CreateLayoutOnInit(){
    createBackgroundBlurAndDefaultColor();
    CreateTitleBar();
    SetupWeaponPicker();
    SetupSightPicker();
    SetupGripPicker();
    SetupMuzzlePicker();   
}

void UWeaponSetupScreen::CloseAllPickerMenus(){
    TArray<UVerticalDropDownBase *> pickers = AllLists();
    for (int i = 0; i < pickers.Num(); i++){
        if(UVerticalDropDownBase *current = pickers[i]){
            current->HidePickerMenu();
        }
    }
}


void UWeaponSetupScreen::CreateTitleBar(){
    UHbox *topBar = NewWidgetInitialized<UHbox>(this);
    AddChild(topBar, FVector2D(0, 0), FVector2D(0, 0));

    if(playerUiParent != nullptr){
        UTextButton *exitButton = NewWidgetInitialized<UTextButton>(this);

        if(exitButton){
            exitButton->SetText("Exit"); //exit

            //set callback here
            exitButton->SetCallBack(
                FSimpleDelegate::CreateUObject(playerUiParent, &UPlayerUiBase::closeLatestScreen)
            );
            topBar->AddChild(exitButton); //automatically listened in click dispatcher because menu is listed
        }
    }
}

/*
Helper:

//WeaponAttachmentValidator::
static std::vector<weaponAttachmentEnum> validSights();
static std::vector<weaponAttachmentEnum> validGrips();
static std::vector<weaponAttachmentEnum> validMuzzleAttachments();

static bool isASightAttachment(weaponAttachmentEnum type);
static bool isAMuzzleAttachment(weaponAttachmentEnum type);
static bool isAGripAttachment(weaponAttachmentEnum type);



//WeaponEnumStringConverter::
static FString toString(weaponEnum type);
static FString toString(weaponAttachmentEnum type);
*/
void UWeaponSetupScreen::SetupWeaponPicker(){
    if(!weaponPicker){
        weaponPicker = NewWidgetInitialized<UVerticalDropDownBase>(this);
        /*
        void UCanvasScreen::AddChild(
            IBaseUiInterface *item,
            FVector2D screenAnchor, //corner top left (0,0), bottom right (1,1)
            FVector2D alignment //gravity / pivot of item (0,0.5), make centered on y
        )
        */
        AddChild(weaponPicker, FVector2D(0.1, 0.1), FVector2D(0, 0));
        
        //setup items once
        std::vector<weaponEnum> types = WeaponEnumStringConverter::AllWeaponTypes();
        for (int i = 0; i < types.size(); i++){
            weaponEnum typeWeapon = types[i];
            FString displayName = WeaponEnumStringConverter::toString(typeWeapon);

            UTextButton *button = NewWidgetInitialized<UTextButton>(this);
            button->SetText(displayName);

            UWeaponTypePayLoad *payload = NewObject<UWeaponTypePayLoad>(this);
            payload->SetType(typeWeapon);

            button->SetPayLoad(payload);

            weaponPicker->AddChild(button); //added to picker list.
        }

    }
}

void UWeaponSetupScreen::SetupSightPicker(){
    /*
    WeaponAttachmentValidator::
    static std::vector<weaponAttachmentEnum> validSights();
    static std::vector<weaponAttachmentEnum> validGrips();
    static std::vector<weaponAttachmentEnum> validMuzzleAttachments();
    */
    if(!attachmentSightPicker){
        attachmentSightPicker = NewWidgetInitialized<UVerticalDropDownBase>(this);
        AddChild(attachmentSightPicker, FVector2D(0.3, 0.1), FVector2D(0, 0));
        
        //setup items once
        std::vector<weaponAttachmentEnum> types = WeaponAttachmentValidator::validSights();
        for (int i = 0; i < types.size(); i++){
            weaponAttachmentEnum typeSight = types[i];
            UTextButton *button = MakeWeaponAttachMentButton(typeSight);
            attachmentSightPicker->AddChild(button); //added to picker list.
        }
    }
}


void UWeaponSetupScreen::SetupGripPicker(){
    if (!attachmentGripPicker){
        attachmentGripPicker = NewWidgetInitialized<UVerticalDropDownBase>(this);
        AddChild(attachmentGripPicker, FVector2D(0.5, 0.1), FVector2D(0, 0));

        //setup items once
        std::vector<weaponAttachmentEnum> types = WeaponAttachmentValidator::validGrips();
        for (int i = 0; i < types.size(); i++){
            weaponAttachmentEnum typeGrip = types[i];
            UTextButton *button = MakeWeaponAttachMentButton(typeGrip);
            attachmentGripPicker->AddChild(button); //added to picker list.
        }
    }
}

void UWeaponSetupScreen::SetupMuzzlePicker(){
    if (!attachmentMuzzlePicker){
        attachmentMuzzlePicker = NewWidgetInitialized<UVerticalDropDownBase>(this);
        AddChild(attachmentMuzzlePicker, FVector2D(0.7, 0.1), FVector2D(0, 0));
        
        //setup items once
        std::vector<weaponAttachmentEnum> types = WeaponAttachmentValidator::validMuzzleAttachments();
        for (int i = 0; i < types.size(); i++){
            weaponAttachmentEnum typeMuzzle = types[i];
            UTextButton *button = MakeWeaponAttachMentButton(typeMuzzle);
            attachmentGripPicker->AddChild(button); //added to picker list.
        }
    }

}




//faster for attachments, abstracts redundant setup code per picker
UTextButton *UWeaponSetupScreen::MakeWeaponAttachMentButton(weaponAttachmentEnum typeAttachment){
    FString displayName = WeaponEnumStringConverter::toString(typeAttachment);

    UTextButton *button = NewWidgetInitialized<UTextButton>(this);
    button->SetText(displayName);

    UWeaponAttachmentPayLoad *payload = NewObject<UWeaponAttachmentPayLoad>(this);
    payload->SetType(typeAttachment);
    button->SetPayLoad(payload);

    return button;
}

/// ------- DISPATCH AND UPDATE ----------

TArray<UVerticalDropDownBase *> UWeaponSetupScreen::AllLists(){
    TArray<UVerticalDropDownBase *> output;
    output.Add(weaponPicker);
    output.Add(attachmentSightPicker);
    output.Add(attachmentGripPicker);
    output.Add(attachmentMuzzlePicker);
    return output;
}

TArray<UVerticalDropDownBase *> UWeaponSetupScreen::AttachmentLists(){
    TArray<UVerticalDropDownBase *> output;
    output.Add(attachmentSightPicker);
    output.Add(attachmentGripPicker);
    output.Add(attachmentMuzzlePicker);
    return output;
}



//overriden from super - custom dispatch needed here for handling picker lists readable.
bool UWeaponSetupScreen::dispatchClick(){



    //check all selectable lists
    TArray<UVerticalDropDownBase *> pickers = AllLists();
    for (int i = 0; i < pickers.Num(); i++){
        if(UVerticalDropDownBase *current = pickers[i]){
            if(current->dispatchClick()){ //may not hit a list item
                int indexHit = current->ListIndexHitFromClickDispatch(); //can be nullptr.
                
                //get interface from layout: handle click
                //and copy data to weapon setup, aswell as propagate to displaying ui later.
                //(may return nullptr if index invalid, which happens if no item from list was hit at all.)
                HandleClickedItem(
                    current->BaseInterfaceFromListAtIndex(indexHit)
                ); 
                
                return true;
            }
        }
    }

    //check base items
    return Super::dispatchClick();
}


void UWeaponSetupScreen::HandleClickedItem(IBaseUiInterface *item){
    if(!referencedSetup){
        return;
    }
    /*
    weaponSetupHelper::

    void setWeaponTypeToCreate(weaponEnum typeIn);
	void setSightAttachment(weaponAttachmentEnum sightIn);
	void setMuzzleAttachment(weaponAttachmentEnum typein);
	void setGripAttachment(weaponAttachmentEnum typein);
    */

    if(item){
        //get payload and handle payload internal data if possible.
        UPayLoadBase *payload = item->GetPayLoad();
        if(payload){

            //test all payload types.
            UWeaponTypePayLoad *castedWeaponPayload = Cast<UWeaponTypePayLoad>(payload);
            if(castedWeaponPayload){
                weaponEnum read = castedWeaponPayload->GetType();
                //update setup.
                referencedSetup->setWeaponTypeToCreate(read);
            }

            UWeaponAttachmentPayLoad *castedAttachmentPayload = Cast<UWeaponAttachmentPayLoad>(payload);
            if(castedAttachmentPayload){
                weaponAttachmentEnum read = castedAttachmentPayload->GetType();

                //filter for type (handeled by setup)
                //update setup
                referencedSetup->SetAnyAttachmentFilterInternal(read);
            }

            UpdateWeaponAndAttachmentDisplay();
        }
    }
}





