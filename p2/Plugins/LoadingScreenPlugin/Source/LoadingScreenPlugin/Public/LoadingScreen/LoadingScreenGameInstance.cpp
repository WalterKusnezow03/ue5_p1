#include "LoadingScreenGameInstance.h"
#include "MoviePlayer.h"
#include "Widgets/SWeakWidget.h"
#include "DebugPlugin/DebugHelper.h"

ULoadingScreenGameInstance *ULoadingScreenGameInstance::instancePtr = nullptr;

void ULoadingScreenGameInstance::Init()
{
    Super::Init();
    MakeData();
    DebugHelper::logMessage("ULoadingScreenGameInstance::Init");
    ULoadingScreenGameInstance::instancePtr = this;

    // Delegates registrieren, die beim Levelwechsel automatisch greifen
    //FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ULoadingScreenGameInstance::BeginLoadingScreen);
    //FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ULoadingScreenGameInstance::EndLoadingScreen);

}

bool ULoadingScreenGameInstance::AllowedToShowLoadingScreen(){
    if (!instancePtr) 
    {
        DebugHelper::logMessage("ULoadingScreenGameInstance::ERROR: instancePtr is NULL!");
        return false;
    }

    if (IsRunningDedicatedServer()){
        return false;
    }

    if (!GEngine) 
    {
        DebugHelper::logMessage("ULoadingScreenGameInstance::ERROR: GEngine is NULL!");
        return false;
    }

    if (!GEngine->GameViewport) 
    {
        DebugHelper::logMessage("ULoadingScreenGameInstance::ERROR: GameViewport is NULL!");
        return false;
    }

    /*if (instancePtr->ActiveLoadingWidget.IsValid())
    {
        DebugHelper::logMessage("ULoadingScreenGameInstance::WARNING: Widget is already active!");
        return false;
    }*/
    return true;
}

void ULoadingScreenGameInstance::ShowManualLoadingScreen()
{
    if(!AllowedToShowLoadingScreen()){
        return;
    }

    DebugHelper::logMessage("ULoadingScreenGameInstance::SHOW - Starting creation...");

    if (!instancePtr->ActiveLoadingWidget.IsValid()){
        // Widget erstellen
        instancePtr->ActiveLoadingWidget = SNew(SLoadingScreenWidget)
            .LoadingScreenDataPtr(instancePtr->GetLoadingScreenDataPtr());

        instancePtr->AddSlateWidgetToViewPort(instancePtr->ActiveLoadingWidget, 99999);
    }
}

/*
void addWidget(TSubClassOf<UWidget> &widget){


    // Hol dir das rohe Slate-Widget aus dem UMG-Widget heraus
    //TSharedRef<SWidget> SlateWidgetRef = widget->TakeWidget();
}
*/

bool ULoadingScreenGameInstance::GEngineStatusOk(){
    return GEngine && GEngine->GameViewport;
}

void ULoadingScreenGameInstance::AddSlateWidgetToViewPort(
    TSharedPtr<SWidget> SlateWidgetPtr,
    int order
){
    if(SlateWidgetPtr.IsValid() && GEngineStatusOk())
    {
        // Zum Viewport hinzufügen
        TSharedRef<SWidget> WidgetRef = SlateWidgetPtr.ToSharedRef();
        GEngine->GameViewport->AddViewportWidgetContent(WidgetRef, order); //at front
        DebugHelper::logMessage("ULoadingScreenGameInstance::SUCCESS: Widget added to Viewport!");

        //ready for blind updates
        isReady = true;
    }
}



void ULoadingScreenGameInstance::HideManualLoadingScreen()
{
    if (instancePtr){
        if (instancePtr->ActiveLoadingWidget.IsValid())
        {
            if (GEngine && GEngine->GameViewport)
            {
                DebugHelper::logMessage("ULoadingScreenGameInstance::HIDE");
                // Widget vom Bildschirm entfernen
                GEngine->GameViewport->RemoveViewportWidgetContent(
                    instancePtr->ActiveLoadingWidget.ToSharedRef()
                );
            }
            instancePtr->ActiveLoadingWidget.Reset();
            instancePtr->isReady = false;
        }
    }
}

void ULoadingScreenGameInstance::MakeData(){
    TSharedPtr<LoadingScreenData> ignored = GetLoadingScreenDataPtr();
}

TSharedPtr<LoadingScreenData> ULoadingScreenGameInstance::GetLoadingScreenDataPtr(){
    if(!ownedLoadingScreenDataPtr.IsValid()){
        ownedLoadingScreenDataPtr = MakeShared<LoadingScreenData>();
    }
    return ownedLoadingScreenDataPtr;
}


void ULoadingScreenGameInstance::Shutdown()
{
    DebugHelper::logMessage("ULoadingScreenGameInstance::Shutdown");
    ULoadingScreenGameInstance::instancePtr = nullptr;
    // Hier wird aufgeräumt, wenn das Spiel komplett geschlossen wird
    Super::Shutdown();
}

//external refresh ui
void ULoadingScreenGameInstance::TickRefreshSlate(){
    DebugHelper::logMessage("ULoadingScreenGameInstance::AsyncTick");
    if (FSlateApplication::IsInitialized())
    {
        //updates all slate widgets.
        FSlateApplication::Get().Tick(ESlateTickType::All);
    }
}



void ULoadingScreenGameInstance::UpdateLoadingEntrySlateTick(
    FString tag, 
    FString text
){
    if(!instancePtr){
        return;
    }

    //hot refresh
    if(!instancePtr->isReady){
        UpdateLoadingEntry(tag, text, []() {});
        return;
    }

    if(instancePtr->ownedLoadingScreenDataPtr){
        instancePtr->ownedLoadingScreenDataPtr->UpdateLoadingEntry(tag, text);
    }
    TickRefreshSlate();
}

void ULoadingScreenGameInstance::HideLoadingEntrySlateTick(FString tag){
    if(!instancePtr){
        return;
    }
    if(instancePtr->ownedLoadingScreenDataPtr){
        instancePtr->ownedLoadingScreenDataPtr->HideLoadingEntry(tag);
    }
    instancePtr->UpdateEnabledState();
    TickRefreshSlate();
}




void ULoadingScreenGameInstance::UpdateLoadingEntry(
    FString tag, 
    FString text,
    TFunction<void()> OnReadyAction
){
    if(!instancePtr){
        return;
    }
    if(instancePtr->ownedLoadingScreenDataPtr){
        instancePtr->ownedLoadingScreenDataPtr->UpdateLoadingEntry(tag, text);
        DebugHelper::logMessage("ULoadingScreenGameInstance::Updated Loading Entry");
    }
    ExecuteWithLoadingScreenTuple(
        OnReadyAction,
        //on finish: hide entry.
        [tag]()
        {
            HideLoadingEntrySlateTick(tag);
        }
    );

    //instancePtr->UpdateEnabledState();
}



void ULoadingScreenGameInstance::UpdateEnabledState(){
    if(ownedLoadingScreenDataPtr){
        if(ownedLoadingScreenDataPtr->ShowLoadingScreen()){
            ShowManualLoadingScreen();
            return;
        }else{
            HideManualLoadingScreen(); //debug disabled
            return;
        }
    }

    HideManualLoadingScreen();
}




// --- ASYNC DO: ---
void ULoadingScreenGameInstance::DoExecute(TFunction<void()> OnReadyAction){
    if(OnReadyAction){
        OnReadyAction();
    }
}

void ULoadingScreenGameInstance::DoExecute(TArray<TFunction<void()>> OnReadyActionData){
    for (int i = 0; i < OnReadyActionData.Num(); i++){
        DoExecute(OnReadyActionData[i]);
    }
}


/**
 * run with:
 * 
ULoadingScreenGameInstance::ExecuteWithLoadingScreen([]()
{
    //your code
});
 * 
 * 
 */
void ULoadingScreenGameInstance::ExecuteWithLoadingScreen(TFunction<void()> OnReadyAction)
{
    TArray<TFunction<void()>> Data;
    Data.Add(OnReadyAction);
    ExecuteWithLoadingScreenMultiple(Data);
}

void ULoadingScreenGameInstance::ExecuteWithLoadingScreenTuple(
    TFunction<void()> OnReadyActionA,
    TFunction<void()> OnReadyActionB
)
{
    TArray<TFunction<void()>> Data;
    Data.Add(OnReadyActionA);
    Data.Add(OnReadyActionB);
    ExecuteWithLoadingScreenMultiple(Data);
}

void ULoadingScreenGameInstance::ExecuteWithLoadingScreenMultiple(
    TArray<TFunction<void()>> OnReadyActionData
)
{
    if (!instancePtr)
    {
        // Falls kein GameInstance da ist, wenigstens das Lambda sofort ausführen, damit nichts crasht
        DoExecute(OnReadyActionData);
        return;
    }

    // 1. Sofort den Ladebildschirm einblenden (und das HIDE blockieren wie besprochen)
    ShowManualLoadingScreen();

    // 2. Einen kurzen Zeit-Puffer einbauen (z.B. 0.15 Sekunden), 
    // damit die Engine das Widget garantiert auf den Bildschirm malen kann!
    if (UWorld* World = instancePtr->GetWorld())
    {
        FTimerHandle TempTimerHandle;
        World->GetTimerManager().SetTimer(TempTimerHandle, [OnReadyActionData]()
        {
            // Erst NACHDEM der Ladebildschirm sichtbar geworden ist,
            // startet hier deine schwere Last / BeginPlay:
            DoExecute(OnReadyActionData);
            DebugHelper::logMessage("ULoadingScreenGameInstance::Timer Done: ON READY ACTION");
        }, 0.15f, false); // 0.15 Sekunden Verzögerung
    }
    else
    {
        DoExecute(OnReadyActionData);
    }
}