#include "ScreenOpenStack.h"


ScreenOpenStack::ScreenOpenStack(){

}

ScreenOpenStack::~ScreenOpenStack(){
    
}


void ScreenOpenStack::dispatchClick(){
    //DebugHelper::logMessage("CLICK WAS REGISTERED Screen Stack");
    //DebugHelper::showScreenMessage("CLICK WAS REGISTERED Screen Stack");
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        if(back){
            if(back->dispatchClick()){
                //DebugHelper::logMessage("CLICK WAS DISPATCHED");
                //DebugHelper::showScreenMessage("CLICK WAS DISPATCHED Screen Stack");
            }
        }
    }
}

bool ScreenOpenStack::ScreenAlreadyOpen(UCanvasScreen *item){
    if(item != nullptr){
        if(opened.size() > 0){
            if(opened.back() == item){
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

        //open
        for (int i = 0; i < opened.size(); i++){
            UCanvasScreen *current = opened[i];
            if(current != nullptr){
                current->SetVisible(false);
            }
        }
        opened.push_back(item);
        item->SetVisible(true);
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
            }
        }

    }
}

void ScreenOpenStack::closeAll(){
    while(!isEmpty()){
        closeBack();
    }
}


bool ScreenOpenStack::isEmpty(){
    return opened.size() <= 0;
}



///@brief ticks all children on the latest screen
void ScreenOpenStack::Tick(float deltatime){
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        if(back){
            back->Tick(deltatime);
        }
    }
}