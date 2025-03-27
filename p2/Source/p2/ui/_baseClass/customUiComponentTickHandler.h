#pragma once

#include "HAL/CriticalSection.h"
#include <set>
#include "CoreMinimal.h"

/**
 * will receive the tick from worldLevel and tick subscribed ui elements
 * Tick is received from worldLevel to keep it thread safe.
 */
class UcustomUiComponentBase; //forward declared, this included in other header

class P2_API customUiComponentTickHandler {

public:
    //public api for world level
    static void Tick(float DeltaTime);

    static void subscribe(UcustomUiComponentBase &ref);
    static void unSubscribe(UcustomUiComponentBase &ref);

private:
    static TArray<UcustomUiComponentBase *> tickedComponents;

    static FCriticalSection Mutex;

    static bool contains(UcustomUiComponentBase *ptr);
    static void remove(UcustomUiComponentBase *ptr);

    //constructors marked private for simplicity
    customUiComponentTickHandler(){}
    ~customUiComponentTickHandler(){}

};