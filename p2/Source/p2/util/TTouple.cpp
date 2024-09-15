#include "TTouple.h"


template <typename T, typename U>
TTouple<T,U>::TTouple(T tin, U uin){
    this->_first = tin;
    this->_last = uin;
}

template <typename T, typename U>
TTouple<T,U>::~TTouple(){
    //empty here.
}



template <typename T, typename U>
T& TTouple<T, U>::first(){
    return _first;
}

template <typename T, typename U>
U& TTouple<T,U>::last(){
    return _last;
}