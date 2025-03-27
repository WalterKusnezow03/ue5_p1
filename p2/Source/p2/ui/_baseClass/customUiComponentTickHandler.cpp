

#include "customUiComponentBase.h"
#include "HAL/CriticalSection.h"
#include "customUiComponentTickHandler.h"

// Statische Variablen definieren
TArray<UcustomUiComponentBase *> customUiComponentTickHandler::tickedComponents;
FCriticalSection customUiComponentTickHandler::Mutex;


void customUiComponentTickHandler::Tick(float DeltaTime){
    //lock is nesecarry here, engine crashes otherwise.
    FScopeLock Lock(&Mutex);
    for (int i = 0; i < tickedComponents.Num(); i++){
        UcustomUiComponentBase *ptr = tickedComponents[i];
        if (ptr != nullptr && IsValid(ptr))
        {
            ptr->Tick(DeltaTime);
        }
    }
}

///@brief adds a ui component to be ticked if not added yet
void customUiComponentTickHandler::subscribe(UcustomUiComponentBase &ref){
    FScopeLock Lock(&Mutex);  // Automatically locks and unlocks
    if(!contains(&ref)){
        tickedComponents.Add(&ref);
    }
}

///@brief removes a ui component from the ticked list if tracked
void customUiComponentTickHandler::unSubscribe(UcustomUiComponentBase &ref){
    FScopeLock Lock(&Mutex);
    remove(&ref);
}

bool customUiComponentTickHandler::contains(UcustomUiComponentBase *ptr){
    if(ptr != nullptr){
        for (int i = 0; i < tickedComponents.Num(); i++){
            if(tickedComponents[i] == ptr){
                return true;
            }
        }
    }
    return false;
}

void customUiComponentTickHandler::remove(UcustomUiComponentBase *ptr){
    if(ptr != nullptr){
        for (int i = 0; i < tickedComponents.Num(); i++){
            if(tickedComponents[i] == ptr){
                //schneller als vector shift. Nicht vergessen.
                tickedComponents[i] = tickedComponents.Last();
                tickedComponents.Pop();
                return;
            }
        }
    }
}