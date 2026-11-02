#include "LoadingScreenData.h"
#include "DebugPlugin/DebugHelper.h"

FString LoadingScreenData::GetText(){

    FString loading = "";
    int found = 0;

    
    for(auto &pair : enabledMap){
        if(pair.second){
            FString key = pair.first;
            if(textMap.find(key) != textMap.end()){
                FString name = textMap[key];
                if (!name.IsEmpty())
                {
                    loading += name + " \n";
                    found++;
                }
            }
        }
    }
    
    
    if(found <= 0){
        loading = "Loading...";
    }

    //TODO:
    //check if inner update is possible...

    return loading;
}

void LoadingScreenData::UpdateLoadingEntry(FString tag, FString text){
    textMap[tag] = text;
    enabledMap[tag] = true;
    DebugHelper::logMessage("LoadingScreenData Set Log Entry: ", text);
}

void LoadingScreenData::HideLoadingEntry(FString tag){
    enabledMap[tag] = false;
}

bool LoadingScreenData::ShowLoadingScreen(){
    //if any entry is true: yes
    for(auto &pair : enabledMap){
        if(pair.second){
            return true;
        }
    }
    return false;
}