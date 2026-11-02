#pragma once

#include "CoreMinimal.h"
#include "LoadingScreenPlugin/Public/LoadingScreen/SharedContainer/LoadingScreenData.h"

class SLoadingScreenWidget : public SCompoundWidget
{

protected:  
    TSharedPtr<LoadingScreenData> LoadingScreenDataPtr;


public:
    //SLATE_BEGIN_ARGS(SLoadingScreenWidget) {}
    //SLATE_END_ARGS()

    //must be copied when deriving
    SLATE_BEGIN_ARGS(SLoadingScreenWidget) 
        : _LoadingScreenDataPtr(nullptr) // Defaultwert hier korrekt
    {}
        SLATE_ARGUMENT(TSharedPtr<LoadingScreenData>, LoadingScreenDataPtr)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs)
    {
        // 1. ZUERST den Zeiger sichern, BEVOR irgendwelche UI-Elemente darauf zugreifen!
        LoadingScreenDataPtr = InArgs._LoadingScreenDataPtr;

        int textSize = 24;

        // 2. Dann erst das UI aufbauen
        ChildSlot
        [
            // 1. SOverlay, damit wir Elemente übereinander legen können
            SNew(SOverlay)

            // Hintergrund: Ein schwarzer Kasten, der den ganzen Bildschirm füllt!
            + SOverlay::Slot()
            .HAlign(HAlign_Fill)
            .VAlign(VAlign_Fill)
            [
                SNew(SBorder)
                .BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush")) // <-- THIS LINE IS REQUIRED
                .BorderBackgroundColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)) // Black
            ]

            // Vordergrund: Der Text in der Mitte
            + SOverlay::Slot()
            .HAlign(HAlign_Center)
            .VAlign(VAlign_Center)
            [
                SNew(STextBlock)
                .Text(this, &SLoadingScreenWidget::GetLoadingText)
                .Font(FCoreStyle::GetDefaultFontStyle("Regular", textSize)) // Sehr groß!
                .ColorAndOpacity(FSlateColor(FLinearColor::White))
            ]
        ];
    }


    // --- DAS IST DEIN TICK ---
    virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override
    {
        SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

        // Hier kannst du Code ausführen, der sich während des Ladens alle paar Frames ändert!
        // Z.B. Timer hochzählen, Animationen steuern etc.
    }

    FText GetLoadingText() const
    {
        if(LoadingScreenDataPtr){
            return FText::FromString(*LoadingScreenDataPtr->GetText());
        }

        // Hier gibst du den Text zurück, der auf dem Bildschirm angezeigt werden soll
        return FText::FromString(TEXT("Lade Daten... Bitte warten."));
    }
};