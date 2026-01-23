#pragma once

#include "CoreMinimal.h"

#include "WorldListItem.generated.h"

class UWorldListWidget;

UCLASS()
class P2_API UWorldListItem : public UUserWidget, public IBaseUiInterface{
    GENERATED_BODY()

public:
    void ClearParent();
    void SetParent(UWorldListWidget *parentIn);

    void SetText(FString text);
    bool ContainsText(FString text);
    FString GetText();

    void NotifyDelete();
    void NotifyLaunch();

    bool IsFree();
    void SetVisibleIfNotFree(bool flag);

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextBase();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetTextWidget();

    UFUNCTION(BlueprintImplementableEvent, Category = "WidgetSetting")
    UWidget *GetDeleteButton();


    //override
    virtual void Tick(float DeltaTime) override;
    virtual bool dispatchClick(const FVector2D &position) override;
    virtual bool dispatchHover(const FVector2D &position) override;
    virtual void SetVisible(bool flag) override;
    virtual bool markedVisible() override;
    virtual UWidget *baseLayoutPointer() override { return this; };

    void MarkVisibleBySearch(FString nameIn);
    bool IsMarkedHiddenBySearch();
    

protected:
    UWorldListWidget *parent = nullptr;
    bool isMarkedFree = false;
    bool isMarkedVisible = true;
    bool markedVisibleBySearch = false;

    UTextBlock *TextBlock();
};