#pragma once

#include "CoreMinimal.h"

class CUSTOMUIPLUGINBASE_API UserInput{
public:
    UserInput();
    ~UserInput();

    void UpdateMouseClickState();
    void UpdateKeyBoardData(APlayerController *playerController);

    bool DispatchClick();
    bool PressedBackspace();
    bool PressedEnter();

    ///invalid once UpdateKeyBoardData() is called.
    const TArray<TCHAR> &GetPressedKeys() const;
    FString GetPressedKeysAsString() const;

    bool HasAnyKeyboardInput();

private:
    bool bMouseLeftIsPressed = false;
    bool dispatchClickNeeded = false;

    void CheckCharacters(APlayerController *playerController);
    void CheckNumbers(APlayerController *playerController);

    TCHAR MakeCase(APlayerController *playerController, TCHAR &c);
    bool IsPressed(APlayerController *playerController, TCHAR &c);
    void Reset();

    TArray<TCHAR> pressed;
    bool pressedEnter = false;
    bool pressedBackspace = false;

};
