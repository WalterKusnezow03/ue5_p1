#pragma once

#include "CoreMinimal.h"
#include "WorldListItem.h"
#include "customuipluginbase/baseInterface/BaseUiInterface.h"
#include "Components/PanelWidget.h"

#include "p2/ui/3Dui/GameStartRoom/TextBox/TextBoxWidget.h"
#include "p2/ui/3Dui/3DUiBase/UserUi3DBaseWidget.h"


#include "WorldListWidget.generated.h"

class AGameStartRoom;

UCLASS()
class P2_API UWorldListWidget : public UUserUi3DBaseWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    void OnBeginPlay();
    void OnEndPlay();

    void SetParent(AGameStartRoom *parentIn);
    void ResetParent();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTypeBoxWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetList();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetCardHandleRight();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetCardHandleLeft();



    //true if sucessfull and not present yet
    bool AddNewItem(FString text);


    //override
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void SetVisible(bool flag) override {};
    virtual bool markedVisible() override { return true; };
    virtual UWidget *baseLayoutPointer() override { return this; };

    virtual void dispatchUserInput(UserInput &input) override;


    UPROPERTY(EditAnywhere, Category="WidgetSetting")
    TSubclassOf<UWorldListItem> widgetClassForItem;


    void NotifyDelete(UWorldListItem *item);
    void NotifyLaunch(UWorldListItem *item);
    void NotifyTextChanged();

    bool TextBoxIsActive();

    

private:
    bool CanAddItem(FString text);

    bool dispatchClickTextbox(const FVector2D &position);
    bool dispatchClickPanel(const FVector2D &position);

    UPanelWidget *GetListAsPanel();
    void InsertPanelChild(UPanelWidget *Panel, UWidget *Child, int32 Index);


    UTextBoxWidget *GetTextBoxSearch();
    void UpdateSearchFilter();

    UWorldListItem *MakeWorldListItem();
    UWorldListItem *FindFreeItem();

    AGameStartRoom *parent = nullptr;

    int numItemsPerCard = 5;
    int currentCardIndex = 0;

    //card interface
    void ShowCardGoRight();
    void ShowCardGoLeft();
    void ShowCard(int index);
    bool IsValidCardIndex(int index);
    void ShowItemsFromTo(int start, int end);
    void ShowItemsFromTo(
        int start,
        int end,
        TArray<UWorldListItem *> &items
    );
    int NumItems();
    //all items which are valid.
    TArray<UWorldListItem *> GetAllItems();
    TArray<UWorldListItem *> GetAllItemsFiltered();

    TArray<IBaseUiInterface *> GetAllItemsForDispatch();

    TArray<FString> GetAllWorldNamesFromWidgets();
    void LoadWorldListFromStorage();
    void SaveWorldListToStorage();
    //removes the world from subdirectory
    void RemoveWorldDataFromStorage(FString name);
};
