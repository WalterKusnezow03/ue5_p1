#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LoadingScreenPlugin/Public/LoadingScreen/SharedContainer/LoadingScreenData.h"
#include "LoadingScreenPlugin/Public/LoadingScreen/Widget/LoadingScreenWidget.h"
#include "LoadingScreenGameInstance.generated.h"

UCLASS()
class LOADINGSCREENPLUGIN_API ULoadingScreenGameInstance : public UGameInstance
{
    GENERATED_BODY()

protected:
    static ULoadingScreenGameInstance *instancePtr;
    bool isReady = false;

protected:
    TSharedPtr<LoadingScreenData> ownedLoadingScreenDataPtr;
    TSharedPtr<LoadingScreenData> GetLoadingScreenDataPtr();
    void MakeData();


public:
    // Wird beim Spielstart einmalig aufgerufen
    virtual void Init() override;


private:
    // Speichert die aktive Instanz deines Widgets auf dem Bildschirm
    TSharedPtr<SLoadingScreenWidget> ActiveLoadingWidget;

    static bool AllowedToShowLoadingScreen();
    void AddSlateWidgetToViewPort(
        TSharedPtr<SWidget> SlateWidgetPtr,
        int order
    );

public:
    UFUNCTION(BlueprintCallable, Category = "LoadingScreen")
    static void ShowManualLoadingScreen();

    UFUNCTION(BlueprintCallable, Category = "LoadingScreen")
    static void HideManualLoadingScreen();

    static void ExecuteWithLoadingScreen(TFunction<void()> OnReadyAction);

    // Wird beim Beenden des Spiels aufgerufen
    virtual void Shutdown() override;

public:
    // static updates
    //ULoadingScreenGameInstance::

    /// @brief show a loading screen with a text - use once to initialize widget
    /// - if never initialized (during a long loading function (see terrain generator))
    /// the widget will be never updated. Callback needed here, since the world is
    /// allowed to tick once and show the widget.
    /// call this function to INITIALIZE the UI.
    /// @param tag 
    /// @param text 
    /// @param OnReadyAction function to be called once done, use [params](){...} for lambda.
    static void UpdateLoadingEntry(FString tag, FString text, TFunction<void()> OnReadyAction);

    /// if UpdateLoadingEntry (with widget show update) was called once, 
    /// this function can be used to REFRESH the UI.
    static void UpdateLoadingEntrySlateTick(
        FString tag,
        FString text
    );
    static void HideLoadingEntrySlateTick(FString tag);

private:
    /// @brief must be called manually on update of widget data
    /// the main game thread can not be interrupted
    /// during heavy processes of loading. (see terrain plugin)
    /// occasionally changing ui is the best we can do here.
    static void TickRefreshSlate();

    static void ExecuteWithLoadingScreenTuple(
        TFunction<void()> OnReadyActionA,
        TFunction<void()> OnReadyActionB
    );
    static void ExecuteWithLoadingScreenMultiple(
        TArray<TFunction<void()>> OnReadyActionData
    );

    static void DoExecute(TFunction<void()> OnReadyAction);
    static void DoExecute(TArray<TFunction<void()>> OnReadyActionData);
    
    bool GEngineStatusOk();

protected:
    void UpdateEnabledState();
};