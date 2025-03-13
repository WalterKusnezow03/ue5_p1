#pragma once


class UPlayerUi;

class P2_API customUiComponentBase{

public:
    customUiComponentBase(){}
    ~customUiComponentBase(){
        playerUiParent = nullptr;
    }


protected:
    UPlayerUi *playerUiParent = nullptr;

    virtual bool correctInitialized(){
        return playerUiParent != nullptr;
    }
};