#include "ScreenOpenStack.h"
#include "customUiPlugin/UserInputTracking/UserInput.h"

ScreenOpenStack::ScreenOpenStack(){

}

ScreenOpenStack::~ScreenOpenStack(){
    
}


void ScreenOpenStack::dispatchClick(){
    //DebugHelper::logMessage("CLICK WAS REGISTERED Screen Stack");
    //DebugHelper::showScreenMessage("CLICK WAS REGISTERED Screen Stack");
    if(UCanvasScreen *back = latestScreen()){
        bool result = back->dispatchClick();
    }
}


bool ScreenOpenStack::ScreenIsOpenTop(UCanvasScreen *item){
    if(item != nullptr){
        if(UCanvasScreen *back = latestScreen()){
            
            return back == item;
        }
    }
    return false;
}

bool ScreenOpenStack::ScreenAlreadyOpen(UCanvasScreen *item){
    if(item != nullptr){

        if(UCanvasScreen *back = latestScreen()){
            if(back == item){
                item->SetVisible(true);
                return true;
            }
        }
        
    }
    return false;
}

/// @brief opens a new ucanvas screen, marks others ivisible, new item visible
/// @param item 
void ScreenOpenStack::open(UCanvasScreen *item){
    if(item){   
        //check if might be already open
        if(ScreenAlreadyOpen(item)){
            return;
        }

        //hide all others
        for (int i = 0; i < opened.size(); i++){
            UCanvasScreen *current = opened[i];
            if(current != nullptr){
                current->SetVisible(false);
            }
        }
        //open new
        opened.push_back(item);
        item->SetVisible(true);
        item->OnOpenScreen();
    }
}

/// @brief closes the latest screen and shows the previous screen
void ScreenOpenStack::closeBack(){
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        opened.pop_back();
        if(back != nullptr){
            back->SetVisible(false);
        }

        //set latest visible again
        if(opened.size() > 0){
            UCanvasScreen *backNew = opened.back();
            if(backNew != nullptr){
                backNew->SetVisible(true);

                //trigger reopen method: refresh ui if needed
                backNew->OnReOpenScreen();
            }
        }

    }
}

void ScreenOpenStack::closeAll(){
    while(!isEmpty()){
        closeBack();
    }
    UiDebugHelper::logMessage(
        FString::Printf(TEXT("UPlayerUi screenstack count: %d"), opened.size()));
}

bool ScreenOpenStack::isEmpty(){
    return opened.size() <= 0;
}



///@brief ticks all children on the latest screen
void ScreenOpenStack::Tick(float deltatime){
    if(UCanvasScreen *back = latestScreen()){
        back->Tick(deltatime);
    }
}


void ScreenOpenStack::dispatchUserInput(UserInput &input){
    if(UCanvasScreen *back = latestScreen()){
        back->dispatchUserInput(input);
    }
    
}


bool ScreenOpenStack::CurrentScreenUsesUserInput(){
    
    if(UCanvasScreen *back = latestScreen()){
        return back->UsesUserInput();
    }


    return false;
}


UCanvasScreen *ScreenOpenStack::latestScreen(){
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        return back;
    }
    return nullptr;
}