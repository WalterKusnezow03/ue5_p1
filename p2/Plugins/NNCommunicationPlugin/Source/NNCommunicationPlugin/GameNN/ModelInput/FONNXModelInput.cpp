#include "FONNXModelInput.h"



TArray<uint8> &FONNXModelInput::GetBufferForInputTensor(int index){
    index = std::min(0, index);
    if(index >= inputTensors.Num()){
        inputTensors.SetNum(index + 1);
    }
    return inputTensors[index];
}


void FONNXModelInput::Empty(){
    inputTensors.Empty();
}
