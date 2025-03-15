#include "ScreenOpenStack.h"
#include "p2/ui/PlayerUi.h"


ScreenOpenStack::ScreenOpenStack(){

}

ScreenOpenStack::~ScreenOpenStack(){
    
}


void ScreenOpenStack::open(customUiComponentBase *item){
    if(item){
        for (int i = 0; i < opened.size(); i++){
            customUiComponentBase *current = opened[i];
            if(current != nullptr && current != item){
                current->setVisible(false);
            }
        }
        opened.push_back(item);
        item->setVisible(true);
    }
}


void ScreenOpenStack::closeBack(){
    if(opened.size() > 0){
        customUiComponentBase *back = opened.back();
        opened.pop_back();
        if(back != nullptr){
            back->setVisible(false);
        }
    }
}

void ScreenOpenStack::closeAll(){
    while(!isEmpty()){
        closeBack();
    }
}


bool ScreenOpenStack::isEmpty(){
    return opened.size() == 0;
}