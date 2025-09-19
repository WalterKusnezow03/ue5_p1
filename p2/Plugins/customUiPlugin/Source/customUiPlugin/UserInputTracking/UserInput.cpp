#include "UserInput.h"

UserInput::UserInput(){

}

UserInput::~UserInput(){

}

//mouse

void UserInput::UpdateMouseClickState(){
    //click dispatch!
    TSet<FKey> PressedButtons = FSlateApplication::Get().GetPressedMouseButtons();
    if (PressedButtons.Contains(EKeys::LeftMouseButton)) {
        if(!bMouseLeftIsPressed){
            // Linksklick ist aktuell gedrückt und war es vorher nicht
            dispatchClickNeeded = true;
            DebugHelper::showScreenMessage("UPlayerUiBase: SLATE CLICK REGISTERED", FColor::Red);
        }
        bMouseLeftIsPressed = true;
    }else{
        bMouseLeftIsPressed = false;
    }
}

bool UserInput::DispatchClick(){
    bool copy = dispatchClickNeeded;
    dispatchClickNeeded = false;
    return copy;
}





//keys
bool UserInput::HasAnyKeyboardInput(){
    bool anyInteraction = pressedBackspace || pressedEnter || pressed.Num() > 0;
    return anyInteraction;
}

bool UserInput::PressedBackspace(){
    bool copy = pressedBackspace;
    pressedBackspace = false;
    return copy;
}

bool UserInput::PressedEnter(){
    bool copy = pressedEnter;
    pressedEnter = false;
    return copy;
}

const TArray<TCHAR> &UserInput::GetPressedKeys() const{
    return pressed;
}

void UserInput::UpdateKeyBoardData(APlayerController *playerController)
{
    Reset();
    if (!playerController)
    {
        return;
    }

    //Keys
    CheckCharacters(playerController);
    CheckNumbers(playerController);

    // Backspace
    if (playerController->WasInputKeyJustPressed(EKeys::BackSpace))
    {
        pressedBackspace = true;
    }

    // Enter
    if (playerController->WasInputKeyJustPressed(EKeys::Enter))
    {
        pressedEnter = true;
    }
}

void UserInput::Reset(){
    pressed.Empty();
}


bool UserInput::IsPressed(APlayerController *playerController, TCHAR &c){
    if(!playerController){
        return false;
    }
    FKey Key(*FString::Chr(c)); // EKeys::A ... EKeys::Z geht auch
    return playerController->WasInputKeyJustPressed(Key);
    //return playerController->IsInputKeyDown(Key);
}

void UserInput::CheckCharacters(APlayerController *playerController){
    if(!playerController){
        return;
    }
    for (TCHAR c = 'A'; c <= 'Z'; ++c)
    {
        if(IsPressed(playerController, c)){
            TCHAR caseSet = MakeCase(playerController, c);
            pressed.Add(caseSet);
        }
    }
    // SpaceBar
    if (playerController->WasInputKeyJustPressed(EKeys::SpaceBar))
    {
        pressed.Add(' ');
    }
}

void UserInput::CheckNumbers(APlayerController *playerController){
    if(!playerController){
        return;
    }

    TArray<FKey> NumberKeys = {
        EKeys::Zero, EKeys::One, EKeys::Two, EKeys::Three, EKeys::Four,
        EKeys::Five, EKeys::Six, EKeys::Seven, EKeys::Eight, EKeys::Nine
    };
    for (int32 i = 0; i <= 9; i++)
    {
        if (playerController->WasInputKeyJustPressed(NumberKeys[i]))
        {
            TCHAR Digit = '0' + i; //dont ask me why, this is correct.
            pressed.Add(Digit); // Char anhängen
        }
    }
}





TCHAR UserInput::MakeCase(APlayerController *playerController, TCHAR &c){
    FKey Key(*FString::Chr(c)); // EKeys::A ... EKeys::Z geht auch
    if (playerController->WasInputKeyJustPressed(Key))
    {
        bool bShift = 
        playerController->IsInputKeyDown(EKeys::LeftShift) || 
        playerController->IsInputKeyDown(EKeys::RightShift);

        TCHAR character = bShift ? c : FChar::ToLower(c);
        return character;
    }
    return c;
}