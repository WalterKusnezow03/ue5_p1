#pragma once

#include "customUiPlugin/ui/screens/CanvasScreen.h"

class UserInput;

class CUSTOMUIPLUGIN_API ScreenOpenStack {

public:
    ScreenOpenStack();
    ~ScreenOpenStack();

    bool CurrentScreenUsesUserInput();

    void open(UCanvasScreen *item);
    void closeBack();

    void closeAll();

    bool isEmpty();

    void dispatchClick();
    void dispatchUserInput(UserInput &input);

    void Tick(float deltatime);

    
    bool ScreenIsOpenTop(UCanvasScreen *item);
private:

    bool ScreenAlreadyOpen(UCanvasScreen *item);
    std::vector<UCanvasScreen *> opened;

    UCanvasScreen *latestScreen();
};