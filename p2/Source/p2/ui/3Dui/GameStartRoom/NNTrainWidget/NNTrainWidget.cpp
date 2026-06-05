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




UTextBlock *UNNTrainWidget::GetTextBlockWidget(){
    if(UWidget *raw = GetTextWidget()){
        if(UTextBlock *casted = Cast<UTextBlock>(raw)){
            return casted;
        }
    }
    return nullptr;
}

void UNNTrainWidget::SetText(FString message){
    if(message.Len() > 0){
        if(UTextBlock *widget = GetTextBlockWidget()){
            widget->SetText(FText::FromString(message));
        }
    }
}








//override
bool UNNTrainWidget::dispatchClick(const FVector2D &position){
    ClickDispatcher dispatcher;    
    if(UWidget *button = GetPlayButton()){
        if(dispatcher.InBound(button, position)){
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
    DebugHelper::logMessage("UNNTrainWidget::Receive ", message);
    if(message.Len() > 0){
        //print("NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED", i + 1, "_of_", epochs, " LOSS ", self.latestLoss)

        

        //UNNTrainWidget::Receive NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED 14 _of_ 50  LOSS  0.01974029839038849

        if(message.Contains("EPOCH_FINISHED")){
            int32 i = -1;

            DebugHelper::logMessage("UNNTrainWidget::Receive contains target ", message);

            
            FString resultText;
            if(ExtractProgress(message, resultText)){
                SetText(resultText);
            }
        }
    }
}


bool UNNTrainWidget::ExtractProgress(FString message, FString &result){
    message = message.Replace(TEXT(" "), TEXT("_"));

    TArray<FString> parts;
    bool cullEmpty = true;
    message.ParseIntoArray(parts, TEXT("_"), cullEmpty);

    FString debug = "UNNTrainWidget::ExtractProgress Debug: ";
    
    int j = -1;
    for (int i = 0; i < parts.Num(); i++){
        debug += " " + parts[i] + " ";
        if(parts[i].ToLower().Contains("finished")){
            j = i;
        }
    }
    //NNServerPathfinder_NetA_RUN_NN_BATCH_EPOCH_FINISHED 14 _of_ 50  LOSS  0.01974029839038849
    if(j >= 0 && j + 3 < parts.Num()){
        FString a = parts[j + 1]; //num ... of
        FString b = parts[j + 3]; //num
        result = FString::Printf(TEXT("Epoch %s of %s"), *a, *b);
        return true;
    }

    DebugHelper::logMessage(FString::Printf(TEXT("debug at %d for %s"), j, *debug));
    return false;
}
