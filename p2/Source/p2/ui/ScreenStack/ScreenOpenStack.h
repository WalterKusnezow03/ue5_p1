#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"
#include "p2/ui/screens/CanvasScreen.h"


class P2_API ScreenOpenStack {

public:
    ScreenOpenStack();
    ~ScreenOpenStack();

    void open(UCanvasScreen *item);
    void closeBack();

    void closeAll();

    bool isEmpty();

    void dispatchClick();

private:
    std::vector<UCanvasScreen *> opened;
    
};