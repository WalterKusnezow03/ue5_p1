#pragma once

#include "FNNSampleSetBase.h"
#include "TNNSampleSetBase.h"
#include <map>

/// Will support multi part of TNNSampleSetBase
/// NOT TESTED! / NOT INTEGREATED!
template <typename T>
class NNCOMMUNICATIONPLUGIN_API TNNSampleSetBaseMultiPart : public FNNSampleSetBase {

protected:

    TPartListStorageInterface<T> partList;
    std::map<FString, TNNSampleSetBase<T> *> files;

    FString activeFileName = "NONE";

    TNNSampleSetBase<T> *activeFile(){
        OpenFile(activeFileName);
        return files[activeFileName];
    }

    TNNSampleSetBase<T> *GetFile(FString name){
        if(HasFile(name)){
            return &files[name];
        }
        return nullptr;
    }

    //may be disabled on async save -> TODO
    bool addAllowed = true;

    void LoadPartFile(){
        bool loaded = partList.Load();
        FillMap();
    }

    void FillMap(){
        TArray<FString> list = partList.GetAllPartNames();
        for (int i = 0; i < list.Num(); i++){
            OpenFile(list[i]);
        }
        //if empty: open new file
        if(list.Num() <= 0){
            OpenNextFile();
        }
    }

    bool HasFile(FString name){
        if(files.find(name) == files.end()){
            return false;
        }
        if(files[name] == nullptr){
            return false;
        }
        return true;
    }

    void OpenFile(FString name){
        if(!HasFile(name)){
            files[name] = new TNNSampleSetBase<T>();
            files[name]->UpdateFileNameSafe(name);
            files[name]->Load();
        }
        activeFileName = name;
    }

    void OpenNextFile(){
        FString nameUpdate = partList.AddNextFileName();
        OpenFile(nameUpdate);
    }

public:
    
    void Load() override {
        LoadPartFile();
        FixupTooLargeFilesOnLoad();
        CloseFilesExceptLast();
    };

    void EndSave() override {
        partList.Save();
        for(auto &pair : files){
            FString name = pair.second;
            CloseFile(name);
        }
    }


    void AddSample(SampleInterface &sample) override {
        if(!addAllowed){
            return;
        }

        /**
         * TODO:
         * to make this process async the sample set must be stored by ptr
         * and changed if needed
         * once the async process is finished, the remaining ptr can be deleted
         */
        AddSampleAsync(sample); //sync will probably be bad with performance issues and sample set cleanup.
    }

    //load batch and move to next unloaded batch file





protected:

    /** Add sample sync */
    void AddSampleSync(SampleInterface &sample){
        if(!addAllowed){
            return;
        }
        if(CurrentFileIsFull()){
            OpenNextFile();
        }
        if(TNNSampleSetBase<T> *file = activeFile()){
            file->AddSample(sample);
        }
    }


    /** Add sample async */
    void AddSampleAsync(SampleInterface &sample){
        if(!addAllowed){
            return;
        }
        
        //Runs code on async set and ignores samples while not finished with work
        if(CurrentFileIsFull()){
            this->addAllowed = false;
            Async(EAsyncExecution::Thread, [sample, this](){   
                OpenNextFile();
                if(TNNSampleSetBase<T> *file = activeFile()){
                    file->AddSample(sample);
                }

                // Wenn du danach zurück auf den Game-Thread musst (z. B. für UI-Updates):
                Async(EAsyncExecution::TaskGraphMainThread, [](){
                    this->addAllowed = true;
                });
            });
        }else{
            if(TNNSampleSetBase<T> *file = activeFile()){
                file->AddSample(sample);
            }
        }
    }
    

    /** checks if current file is full, if opened */
    bool CurrentFileIsFull(){
        if(TNNSampleSetBase<T> *file = activeFile()){
            return file->BatchIsFull();
        }
        return false;
    }

    void FixupTooLargeFilesOnLoad(){
        //check all possible files for a too large data set
        //and split it up, add over load samples to a new data set
        //split it up to not filled data sets
        TArray<T> bufferShared;
        for(auto &pair : files){
            FString name = pair.first;
            if(TNNSampleSetBase<T> *file = GetFile(name)){
                //get over spilled data
                bool ignored = file->GetOverMaxBatchSizeSamples(bufferShared);
                //add to last file (sync)
            }
        }

        //process buffer
        for (int i = 0; i < bufferShared.Num(); i++){
            T &current = bufferShared[i];
            AddSampleSync(current);
        }
    }



    void CloseFilesExceptLast(){
        for(auto &pair : files){
            FString name = pair.first;
            if(!name.Equals(activeFileName)){
                CloseFile(name);
            }
        }
    }

    void CloseFile(FString name){
        if(HasFile(name)){
            if(TNNSampleSetBase<T> *file = files[name]){
                file->EndSave();
                delete file;
                file = nullptr;
                files[name] = nullptr;
            }
        }
    }




};