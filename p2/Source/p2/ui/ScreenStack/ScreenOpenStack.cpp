#include "ScreenOpenStack.h"
#include "p2/ui/PlayerUi.h"
#include "GameCore/DebugHelper.h"


ScreenOpenStack::ScreenOpenStack(){

}

ScreenOpenStack::~ScreenOpenStack(){
    
}


void ScreenOpenStack::dispatchClick(){
    DebugHelper::logMessage("CLICK WAS REGISTERED Screen Stack");
    DebugHelper::showScreenMessage("CLICK WAS REGISTERED Screen Stack");
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        if(back){
            if(back->dispatchClick()){
                DebugHelper::logMessage("CLICK WAS DISPATCHED");
                DebugHelper::showScreenMessage("CLICK WAS DISPATCHED Screen Stack");
            }
        }
    }
}


void ScreenOpenStack::open(UCanvasScreen *item){
    if(item){
        for (int i = 0; i < opened.size(); i++){
            UCanvasScreen *current = opened[i];
            if(current != nullptr){
                current->setVisible(false);
            }
        }
        opened.push_back(item);
        item->setVisible(true);
    }
}


void ScreenOpenStack::closeBack(){
    if(opened.size() > 0){
        UCanvasScreen *back = opened.back();
        opened.pop_back();
        if(back != nullptr){
            back->setVisible(false);
        }

        //set latest visible again
        if(opened.size() > 0){
            UCanvasScreen *backNew = opened.back();
            if(backNew != nullptr){
                backNew->setVisible(true);
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