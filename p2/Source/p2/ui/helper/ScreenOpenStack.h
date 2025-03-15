#pragma once

#include "p2/ui/_baseClass/customUiComponentBase.h"


class P2_API ScreenOpenStack {

public:
    ScreenOpenStack();
    ~ScreenOpenStack();

    void open(customUiComponentBase *item);
    void closeBack();

    void closeAll();

    bool isEmpty();

private:
    std::vector<customUiComponentBase *> opened;
    
};