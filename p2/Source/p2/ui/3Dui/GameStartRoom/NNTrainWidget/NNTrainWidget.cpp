#include "NNTrainWidget.h"
#include "customuipluginbase/Dispatcher/ClickDispatcher.h"
#include "p2/ui/3Dui/GameStartRoom/GameStartRoom.h"

void UNNTrainWidget::SetParent(AGameStartRoom *parentIn){
    parent = parentIn;
    Init();
}

void UNNTrainWidget::ResetParent(){
    parent = nullptr;
}

void UNNTrainWidget::NotifyLaunch(){
    if(parent){
        parent->NotifyNNTrainLaunch();
    }
}




UTextBlock *UNNTrainWidget::GetTextBlockWidget(UWidget *raw){
    if(raw){
        if(UTextBlock *casted = Cast<UTextBlock>(raw)){
            return casted;
        }
    }
    return nullptr;
}

void UNNTrainWidget::SetText(FString message){
    if(message.Len() > 0){
        if(UTextBlock *widget = GetTextBlockWidget(GetTextWidget())){
            widget->SetText(FText::FromString(message));
        }
    }
}

void UNNTrainWidget::SetTextLoss(FString message){
    if(message.Len() > 0){
        if(UTextBlock *widget = GetTextBlockWidget(GetTextLossWidget())){
            widget->SetText(FText::FromString(message));
        }
    }
}








//override
bool UNNTrainWidget::dispatchClick(const FVector2D &position){
    ClickDispatcher dispatcher;    
    if(UWidget *button = GetPlayButton()){
        if(dispatcher.InBound(button, position)){
            SetText("Training Launched!");
            NotifyLaunch();
            return true;
        }
    }
    return false; 
}

bool UNNTrainWidget::dispatchHover(const FVector2D &position){
    DebugHelper::showScreenMessage("UNNTrainWidget::HOVER", FColor::Red);
    if (playButtonUtil.dispatchHover(position))
    {
        DebugHelper::showScreenMessage("UNNTrainWidget::HOVER 2", FColor::Red);
        return true;
    }
    return false;
}

//remove hover on widget left
void UNNTrainWidget::removeHover(){
    playButtonUtil.removeHover();
    
}





void UNNTrainWidget::Init(){
    SetupFromDefaultColors(
        playButtonUtil,
        GetPlayButton()
    );
}




void UNNTrainWidget::ReceiveMessage(FString message){
    //DebugHelper::logMessage("UNNTrainWidget::Receive ", message);
    if(message.Len() > 0){
        //print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED", i + 1, "_of_", epochs, " LOSS ", self.latestLoss)

        

        //UNNTrainWidget::Receive NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED 14 _of_ 50  LOSS  0.01974029839038849

        if(message.Contains("EPOCH_FINISHED")){
        
            //DebugHelper::logMessage("UNNTrainWidget::Receive contains target ", message);

            TArray<FString> parts;
            DecomposeIntoArray(message, parts, TEXT("_"));




            FString resultText;
            if(ExtractProgress(parts, resultText)){
                SetText(resultText);
            }
            FString resultLoss;
            if (ExtractLoss(parts, resultLoss)){
                SetTextLoss(resultLoss);
            }
        }
    }
}

void UNNTrainWidget::DecomposeIntoArray(FString message, TArray<FString> &outArray, FString targetSeperator){
    outArray.Empty();
    // message = message.Replace(TEXT(" "), TEXT("_"));
    message = message.Replace(TEXT(" "), *targetSeperator);

    bool cullEmpty = true;
    //message.ParseIntoArray(parts, TEXT("_"), cullEmpty);
    message.ParseIntoArray(outArray, *targetSeperator, cullEmpty);
}


int UNNTrainWidget::FindIndexLowerCase(const TArray<FString> &parts, FString target){
    int j = -1;
    FString targetLower = target.ToLower();
    if(targetLower.Len() > 0){
        for (int i = 0; i < parts.Num(); i++)
        {
            if(parts[i].ToLower().Contains(targetLower)){
                j = i;
                break;
            }
        }
    }
    return j;
}

bool UNNTrainWidget::ExtractProgress(const TArray<FString> &parts, FString &result){
    
    int j = FindIndexLowerCase(parts, TEXT("finished"));
    
    //NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED 14 _of_ 50  LOSS  0.01974029839038849
    if(j >= 0 && j + 3 < parts.Num()){
        FString a = parts[j + 1]; //num ... of
        FString b = parts[j + 3]; //num
        result = FString::Printf(TEXT("Epoch %s of %s"), *a, *b);
        return true;
    }

    //DebugHelper::logMessage(FString::Printf(TEXT("debug at %d for %s"), j, *debug));
    return false;
}


bool UNNTrainWidget::ExtractLoss(const TArray<FString> &parts, FString &result){
    if(parts.Num() > 0){
        int j = FindIndexLowerCase(parts, TEXT("loss"));
        if(j>= 0 && j +1 < parts.Num()){
            FString lossString = parts[j + 1];

            int digits = FindDigitsNeeded(lossString); // 5; //"." also inside.
            if(digits > 0){
                lossString = lossString.Left(digits + 1);
                result = "Loss: " + lossString;
                return true;
            }
        }
    }
    return false;
}

int UNNTrainWidget::FindDigitsNeeded(FString &lossString){
    //0.xxx //start from index 2
    int minNeeded = 2;
    for (int i = 2; i < lossString.Len(); i++){
        TCHAR current = lossString[i];
        
        if(current != '0'){
            minNeeded += i;
            return minNeeded;
        }
    }
    return minNeeded;
}