#pragma once

#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"

#include "p2/ui/3Dui/RoomBase/WidgetRoomActorBase.h"
#include "p2/ui/3Dui/PauseActor/EPauseWidgetEvent.h"
#include "p2/ui/3Dui/PauseActor/Settings/enum/ESettingsWidgetEvent.h"

#include "PauseRoomActor.generated.h"

class APauseWidgetActor;
class UPauseWidget;

class ASettingsWidgetActor;
class USettingsWidget;

//instead of entering the room, the room will be moved to the player
UCLASS()
class P2_API APauseRoomActor : public AWidgetRoomActorBase {
    GENERATED_BODY()

private:
    static APauseRoomActor *instance;

public:
    APauseRoomActor();

    static void StaticEnter(AActor *player);
    static void StaticLeave();
    virtual bool Enter(AActor *player) override;
    virtual void Leave() override;

    

    void Notify(EPauseWidgetEvent event);
    void Notify(ESettingsWidgetEvent event);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    static void CreateInstanceIfNeeded(AActor *actor);

    void NotifyExit();
    void NotifyOpenLoadoutRoom();
    void NotifyOpenGameStartRoom();
    void ShowSettingsWidget(bool flag);

    void FindPauseMenuOnBeginPlay();
    void FindSettingsMenuOnBeginPlay();
    void SetParentReferenceForWidgets();

    UPauseWidget *GetPauseWidget();
    USettingsWidget *GetSettingsWidget();

    virtual void ClearReferencesOnEndPlay() override;

    float distanceFromPlayerToBeKept = 150.0f;

    void SetToPlayerLocation();

private:
    APauseWidgetActor *pauseWidgetActor = nullptr;
    ASettingsWidgetActor *settingsWidgetActor = nullptr;
};