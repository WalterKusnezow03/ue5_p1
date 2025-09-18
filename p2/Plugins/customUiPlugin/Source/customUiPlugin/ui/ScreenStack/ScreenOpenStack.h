#pragma once

#include "customUiPlugin/ui/screens/CanvasScreen.h"

class UserInput;

class CUSTOMUIPLUGIN_API ScreenOpenStack {

public:
    ScreenOpenStack();
    ~ScreenOpenStack();

    void open(UCanvasScreen *item);
    void closeBack();

    void closeAll();

    bool isEmpty();

    void dispatchClick();
    void dispatchUserInput(UserInput &input);

    void Tick(float deltatime);

    bool ScreenAlreadyOpen(UCanvasScreen *item);

private:
    std::vector<UCanvasScreen *> opened;
    
};