#pragma once

#include "customUiPlugin/ui/screens/CanvasScreen.h"


class CUSTOMUIPLUGIN_API ScreenOpenStack {

public:
    ScreenOpenStack();
    ~ScreenOpenStack();

    void open(UCanvasScreen *item);
    void closeBack();

    void closeAll();

    bool isEmpty();

    void dispatchClick();

    void Tick(float deltatime);

    bool ScreenAlreadyOpen(UCanvasScreen *item);

private:
    std::vector<UCanvasScreen *> opened;
    
};