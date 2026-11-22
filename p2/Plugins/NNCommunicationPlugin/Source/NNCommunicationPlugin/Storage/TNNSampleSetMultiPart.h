#pragma once

#include "FNNSampleSetBase.h"
#include "TNNSampleSetBase.h"
#include <map>
#include "DebugPlugin/DebugHelper.h"
#include "NNCommunicationPlugin/Storage/StorageInterface/TPartListStorageInterface.h"

/// Will support multi part of TNNSampleSetBase
/// NOT TESTED! / NOT INTEGREATED!
template <typename T>
class NNCOMMUNICATIONPLUGIN_API TNNSampleSetBaseMultiPart : public FNNSampleSetBase {

protected:

    TPartListStorageInterface<T> partList;
    std::map<FString, TNNSampleSetBase<T> *> files;
    std::map<FString, bool> trainedFlags;

    FString activeFileName = "NONE";

    TNNSampleSetBase<T> *activeFile(){
        OpenFile(activeFileName);
        return files[activeFileName];
    }

    TNNSampleSetBase<T> *GetFile(FString name){
        if(HasFile(name)){
            return files[name];
        }
        return nullptr;
    }

    //may be disabled on async save -> TODO
    bool addAllowed = true;

    /** acess lock for on async load of files and clean up bad file sizes / sample count */
    bool accessLocked = false;

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
            trainedFlags[name] = false;
        }
        activeFileName = name;
    }

    void OpenNextFile(){
        FString nameUpdate = partList.AddNextFileName();
        OpenFile(nameUpdate);
    }

public:
    
    void Load() override {
        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Load");
        LoadPartFile();
        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Loaded PartFile size ", partList.SizeList());
        
        this->accessLocked = true;
        
        // Run heavy lifting on low-priority background thread with minimal resources
        Async(EAsyncExecution::TaskGraph, [this](){   
            this->FixupTooLargeFilesOnLoad();
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Cleaned Buffers PartFile size ", this->partList.SizeList());

            this->CloseFilesExceptLast(); //only last one needed to add new samples.
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Close up on Load finished ", this->partList.SizeList());

            // Return to game thread to signal completion
            Async(EAsyncExecution::TaskGraphMainThread, [this](){
                this->accessLocked = false;
            });
        });
    };

    void EndSave() override {
        partList.Save();
        for(auto &pair : files){
            FString name = pair.first;
            CloseFile(name);
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Close file end save ", name);
        }
        // Final flush to ensure all files are written
        //FPlatformFileManager::Get().GetPlatformFile().FlushFiles();
    }


    void AddSample(SampleInterface &sample) override {
        if(!addAllowed){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("TNNSampleSetBaseMultiPart::AddSample ADD NOT ALLOWED")
                )
            );
            return;
        }
        if(accessLocked){
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("TNNSampleSetBaseMultiPart::AddSample ACCESS LOCKED")
                )
            );

            AddSampleToTempSet(sample);
            return;
        }

        AddSampleSync(sample);
    }

    //load batch and move to next unloaded batch file





protected:

    /** Add sample to temporary data collection (if access locked in async)
     * will get processed later.
     */
    void AddSampleToTempSet(SampleInterface &sample){
        //todo
    }

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
            //if async process still running ignore the incoming sample.
            //(i dont care.)
            return;
        }
        
        //Runs code on async set and ignores samples while not finished with work
        if(CurrentFileIsFull()){
            this->addAllowed = false;
            SampleInterface *samplePtr = &sample;
            Async(EAsyncExecution::Thread, [samplePtr, this](){   
                OpenNextFile();
                if(TNNSampleSetBase<T> *file = activeFile()){
                    file->AddSample(*samplePtr);
                }

                //unlock add allowed
                Async(EAsyncExecution::TaskGraphMainThread, [this](){
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
        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::FixupTooLargeFilesOnLoad");
        //check all possible files for a too large data set
        //and split it up, add over load samples to a new data set
        //split it up to not filled data sets
        TArray<T> bufferShared;
        for(auto &pair : files){
            FString name = pair.first;
            if(TNNSampleSetBase<T> *file = GetFile(name)){
                //get over spilled data
                bool changed = file->GetOverMaxBatchSizeSamples(bufferShared);
                
                //add to last file (sync)

                //process buffer
                for (int i = 0; i < bufferShared.Num(); i++){
                    T &current = bufferShared[i];
                    AddSampleSync(current);
                }
                bufferShared.Empty();
                file = nullptr;
                CloseFile(name);
            }
        }

        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::FixupTooLargeFilesOnLoad count ", bufferShared.Num());

        
    }



    void CloseFilesExceptLast(){
        for(auto &pair : files){
            FString name = pair.first;
            if(!name.Equals(activeFileName)){
                //mark for check false for quick save
                if(TNNSampleSetBase<T> *file = files[name]){
                    file->checkDuplicatesOnSave = false;
                }

                CloseFile(name, false);
            }
        }
    }

    void CloseFile(FString name, bool checkDuplicates){
        if(HasFile(name)){
            if(TNNSampleSetBase<T> *file = files[name]){
                DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Close file ", name);
                file->checkDuplicatesOnSave = checkDuplicates;
                file->EndSave();
                delete file;
                file = nullptr;
                files[name] = nullptr;
            }
        }
        // Flush file system to ensure files are written to disk
        //FPlatformFileManager::Get().GetPlatformFile().FlushFiles();
    }

    void CloseFile(FString name){
        CloseFile(name, true);
    }

public: 
    /// ----- Batch loading in steps -----
    virtual void PrepareBatchBinary(TArray<uint8> &bytes) {
        if(accessLocked){
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Cant Load Batch, locked");
            return;
        }
        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Load Batch, unlocked");
        TNNSampleSetBase<T> *file = activeFile();
        if(file){
            file->PrepareBatchBinary(bytes);
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::PrepareBatchBinary ", bytes.Num());
            //block next batch loading
            batchPrepared = true;

            //flag as trained
            trainedFlags[activeFileName] = true;
        }
    };

    bool BatchPrepared(){
        return batchPrepared;
    }

    /** tries to load next batch file for training (hasNext for sucessfull out flag) */
    void NotifyLoadNextBatch(bool &hasNext) override {
        hasNext = false;
        DebugHelper::logMessage("TNNSampleSetBaseMultiPart::NotifyLoadNextBatch");
        if(accessLocked){
            DebugHelper::logMessage("TNNSampleSetBaseMultiPart::Cant Load Batch, locked");
            return;
        }
        /** 
         *  From NN Request Handle:
         *  -> if flagged as batch not prepared the NN can train.
         * 
         *  NNRequestHandleBase::
         *  ...
            if(!batchTaskPtr->BatchPrepared()){
                ...
                batchTaskPtr->PrepareBatchBinary(buffer);
                ...
         *
         *  -> thats why batch prepared will be set to false.
        */
        FString nameNext;
        if(GetNextUnloadedBatchName(nameNext)){
            batchPrepared = false;
            DebugHelper::logMessage(
                FString::Printf(
                    TEXT("TNNSampleSetBaseMultiPart::NotifyLoadNextBatch -> LoadNextBatchFile %s -> %s"),
                    *activeFileName,
                    *nameNext
                )
            );
            CloseFile(activeFileName, false); //close previous file
            OpenFile(nameNext);
            hasNext = true;
        }
    };

protected:
    /** 
     * finds from the map of unloaded files a file name which was not
     * trained yet. Not nesecarrily in order, but that doesnt
     * matter!
     */
    bool GetNextUnloadedBatchName(FString &outName){
        for(const auto &pair : trainedFlags){
            const FString &name = pair.first;
            const bool wasTrained = pair.second;
            if(!wasTrained){
                outName = name;
                return true;
            }
        }

        return false;
    }
};