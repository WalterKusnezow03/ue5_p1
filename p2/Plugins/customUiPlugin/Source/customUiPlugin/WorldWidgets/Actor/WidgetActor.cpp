#include "WidgetActor.h"

#include "customUiPlugin/WorldWidgets/UUserWidgetBase/UserWidgetBase.h"

#include "DebugPlugin/DebugHelper.h"

AWidgetActor::AWidgetActor(){
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    RootComponent = WidgetComponent; //->SetupAttachment(RootComponent);

    

    internalWidget = nullptr;
}

void AWidgetActor::BeginPlay(){
    Super::BeginPlay();
    Init();

    if(CreateDefaultWidgetOnBeginPlay){
        //UUserWidgetBase *ptr = NewObject<UUserWidgetBase>(this);
        
        UUserWidgetBase* ptr = CreateWidget<UUserWidgetBase>(GetWorld(), UUserWidgetBase::StaticClass());
        ptr->Init();
        ptr->SetLabelText("Text A");
        ReplaceWidget(ptr);
        DebugHelper::logMessage("AWidgetActor:: created default widget");
    }
}


void AWidgetActor::Init(){

    //WidgetComponent->RegisterComponent();

    // Optional: damit es zur Kamera schaut
    //WidgetComponent->SetDrawAtDesiredSize(true);



    // WidgetComponent Einstellungen
    WidgetComponent->SetDrawSize(FVector2D(500.f, 500.f));
    WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
    WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
    WidgetComponent->SetTwoSided(true);
    WidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
    WidgetComponent->SetVisibility(true);
    WidgetComponent->SetHiddenInGame(false);

    WidgetComponent->SetWorldRotation(FRotator(0, 180, 0)); // wenn Kamera in +X schaut



    //start?
    WidgetComponent->UpdateWidget();
}

void AWidgetActor::Tick(float deltatime){
    Super::Tick(deltatime);
}

//virtual void SetWidget(UUserWidget* Widget);
void AWidgetActor::ReplaceWidget(UUserWidget *widget){
    if(widget && WidgetComponent){

        WidgetComponent->SetWidget(widget);
        internalWidget = widget;
    }
}