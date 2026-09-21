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
    
    if(dispatcher.InBound(GetPlayButton(), position)){
        SetText("Training Launched!");
        NotifyLaunch();
        return true;
    }
    if(dispatcher.InBound(GetHeatMapEnableDisableButton(), position)){
        UpdateHeatMapSaveOnEndTextNextState();
        return true;
    }

    
    
    return false; 
}







bool UNNTrainWidget::dispatchHover(const FVector2D &position){
    //DebugHelper::showScreenMessage("UNNTrainWidget::HOVER", FColor::Red);
    bool hoverA = playButtonUtil.dispatchHover(position);
    bool hoverB = heatMapButtonUtil.dispatchHover(position);

    if(hoverA || hoverB){
        return true;
    }

    /*if (playButtonUtil.dispatchHover(position))
    {
        //DebugHelper::showScreenMessage("UNNTrainWidget::HOVER 2", FColor::Red);
        return true;
    }*/

    return false;
}

//remove hover on widget left
void UNNTrainWidget::removeHover(){
    playButtonUtil.removeHover();
    heatMapButtonUtil.removeHover();
}

void UNNTrainWidget::Init(){
    SetupFromDefaultColors(
        playButtonUtil,
        GetPlayButton()
    );
    SetupFromDefaultColors(
        heatMapButtonUtil,
        GetHeatMapEnableDisableButton()
    );
    UpdateHeatMapSaveOnEndText();
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





/*bool UNNTrainWidget::ExtractLoss(const TArray<FString>& Parts, FString& OutResult)
{
    if (Parts.Num() == 0) return false;

    int32 Index = FindIndexLowerCase(Parts, TEXT("loss"));
    if (Index >= 0 && Index + 1 < Parts.Num())
    {
        FString LossString = Parts[Index + 1];

        // Transforms "2.510701415303629e-05" -> "0.(4)25107"
        FString FormattedLoss = FormatLossScientificCustom(LossString, 5);

        OutResult = "Loss: " + FormattedLoss;
        return true;
    }
    return false;
}

FString UNNTrainWidget::FormatLossScientificCustom(const FString& InLossString, int32 SignificantDigits)
{
    int32 EIndex = INDEX_NONE;
    if (!InLossString.FindChar('e', EIndex) && !InLossString.FindChar('E', EIndex))
    {
        // Not scientific notation; return as-is
        return InLossString;
    }

    // 1. Split into Mantissa ("2.510701415303629") and Exponent ("-05")
    FString MantissaStr = InLossString.Left(EIndex);
    FString ExponentStr = InLossString.RightChop(EIndex + 1);

    int32 Exponent = FCString::Atoi(*ExponentStr);

    // Clean mantissa digits (remove sign and decimal point)
    FString Sign = MantissaStr.StartsWith(TEXT("-")) ? TEXT("-") : TEXT("");
    MantissaStr = MantissaStr.Replace(TEXT("-"), TEXT("")).Replace(TEXT("."), TEXT(""));

    // Trim mantissa to desired significant digits
    if (MantissaStr.Len() > SignificantDigits)
    {
        MantissaStr = MantissaStr.Left(SignificantDigits);
    }

    // 2. Format Negative Exponents -> 0.(ZerosCount)Mantissa
    if (Exponent < 0)
    {
        int32 LeadingZerosCount = FMath::Abs(Exponent) - 1;

        if (LeadingZerosCount > 0)
        {
            return FString::Printf(TEXT("%s0.(%d)%s"), *Sign, LeadingZerosCount, *MantissaStr);
        }
        else
        {
            return FString::Printf(TEXT("%s0.%s"), *Sign, *MantissaStr);
        }
    }

    // 3. Fallback for positive exponents if encountered
    return InLossString;
}*/

bool UNNTrainWidget::ExtractLoss(const TArray<FString>& Parts, FString& OutResult)
{
    if (Parts.Num() == 0) return false;

    int32 Index = FindIndexLowerCase(Parts, TEXT("loss"));
    if (Index >= 0 && Index + 1 < Parts.Num())
    {
        FString LossString = Parts[Index + 1];

        // Transforms scientific notation into custom compact format
        FString FormattedLoss = FormatLossScientificCustom(LossString, 5);

        OutResult = "Loss: " + FormattedLoss;
        return true;
    }
    return false;
}

FString UNNTrainWidget::FormatLossScientificCustom(const FString& InLossString, int32 SignificantDigits)
{
    int32 EIndex = INDEX_NONE;
    if (!InLossString.FindChar('e', EIndex) && !InLossString.FindChar('E', EIndex))
    {
        // Not scientific notation; return as-is
        return InLossString;
    }

    // 1. Split into Mantissa and Exponent
    FString MantissaStr = InLossString.Left(EIndex);
    FString ExponentStr = InLossString.RightChop(EIndex + 1);

    int32 Exponent = FCString::Atoi(*ExponentStr);

    // Clean mantissa digits (remove sign and decimal point)
    FString Sign = MantissaStr.StartsWith(TEXT("-")) ? TEXT("-") : TEXT("");
    MantissaStr = MantissaStr.Replace(TEXT("-"), TEXT("")).Replace(TEXT("."), TEXT(""));

    // 2. Format Negative Exponents -> 0.(ZerosCount)Mantissa
    if (Exponent < 0)
    {
        int32 LeadingZerosCount = FMath::Abs(Exponent) - 1;

        // Ensure we take only the requested significant digits starting from the first non-zero representation
        if (MantissaStr.Len() > SignificantDigits)
        {
            MantissaStr = MantissaStr.Left(SignificantDigits);
        }

        if (LeadingZerosCount > 0)
        {
            return FString::Printf(TEXT("%s0.(%d)%s"), *Sign, LeadingZerosCount, *MantissaStr);
        }
        else
        {
            return FString::Printf(TEXT("%s0.%s"), *Sign, *MantissaStr);
        }
    }

    // 3. Fallback for positive exponents if encountered
    return InLossString;
}



//update for heatmap output enabled y/n
//on dispatch click change
//aswell as update the text
void UNNTrainWidget::UpdateHeatMapSaveOnEndTextNextState(){
    bool currentState = NNPathFinderExtensionApi::HeatMapSaveOnEndEnabled();
    bool nextState = !currentState;
    NNPathFinderExtensionApi::EnableHeatMapSaveOnEnd(nextState);
    UpdateHeatMapSaveOnEndText();
}

void UNNTrainWidget::UpdateHeatMapSaveOnEndText(){
    bool currentState = NNPathFinderExtensionApi::HeatMapSaveOnEndEnabled();
    FString updateText = currentState ? "Heatmap output Enabled" : "Heatmap output Disabled";
    if(UTextBlock *widget = GetTextBlockWidget(GetHeatMapSettingTextWidget())){
        widget->SetText(FText::FromString(updateText));
    }
}

