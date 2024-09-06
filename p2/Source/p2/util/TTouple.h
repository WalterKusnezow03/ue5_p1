#pragma once

#include "CoreMinimal.h"

template <typename T, typename U>
class P2_API TTouple{

public:
    TTouple(T t, U u);
    ~TTouple();

    T& first();
    U& last();

private:
    T _first;
    U _last;
};