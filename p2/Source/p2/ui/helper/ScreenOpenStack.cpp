#include "ScreenOpenStack.h"
#include "p2/ui/PlayerUi.h"


ScreenOpenStack::ScreenOpenStack(){

}

ScreenOpenStack::~ScreenOpenStack(){
    
}


void ScreenOpenStack::open(UcustomUiComponentBase *item){
    if(item){
        for (int i = 0; i < opened.size(); i++){
            UcustomUiComponentBase *current = opened[i];
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
        UcustomUiComponentBase *back = opened.back();
        opened.pop_back();
        if(back != nullptr){
            back->setVisible(false);
        }

        //set latest visible again
        if(opened.size() > 0){
            UcustomUiComponentBase *backNew = opened.back();
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