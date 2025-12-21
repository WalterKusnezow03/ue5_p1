#include "C4Item.h"
#include "p2/weapon/CarriedItemBase/carriedItem.h"


AC4Item::AC4Item() : Super() {

}


void AC4Item::BeginPlay(){
    Super::BeginPlay();
    Type = weaponEnum::C4;
    explosiveHelper.Setup(EXPLOSION_RADIUS, DAMAGE, DAMAGE_RADIUS);
    FindComponentsOnBeginPlay();
    CreateMaterialOnBeginPlay();
    SetupTimer();
}
void AC4Item::Tick(float deltatime){
    Super::Tick(deltatime);
    TickLightStatus(deltatime);


    //todo hier: 
    //überlegen wie der zünder seine animation macht!
    //muss ja auch gestartet und angetickt werden.



}


//on shoot: spawn new c4
void AC4Item::SpawnItemAtLocation(FVector &location, FVector &normal){
    //spawn c4 here at location
    //add to children!
    if(EntityManager *e = AworldLevel::entityManager()){
        Aweapon *weapon = e->spawnAweapon(GetWorld(), weaponEnum::C4);
        if(weapon != nullptr){
            if(AC4Item *casted = Cast<AC4Item>(weapon)){
                casted->Type = weaponEnum::C4;
                casted->ResetFlagsAndProperties();
                casted->drop(); // remove from any owner.
                casted->SetParent(this);
                casted->setTeam(teamEnum::neutralTeam);
                childs.Add(casted);
                
                casted->showWeapon(true);

                casted->SetLocationAndLookDir(location, normal);
            }
        }   
    }
}

void AC4Item::SetLocationAndLookDir(FVector &location, FVector &normal){
    if(!isPickedUp()){
        SetActorLocation(location);
        FRotator r = normal.Rotation(); //x is forward, but wanted z, -90
        r.Pitch -= 90.0f;
        SetActorRotation(r);
    }
}



void AC4Item::showWeapon(bool show){
    //Debug
    ShowComponent(c4SceneComponent, show);
    return;

    //default behaiviour
    if(isPickedUp()){
        //show c4 detonator
        //hide c4 block
        ShowComponent(c4SceneComponent, false);
        ShowComponent(detonatorSceneComponent, true);
    }else{
        //hide show c4 only
        //hide detonator
        ShowComponent(c4SceneComponent, true);
        ShowComponent(detonatorSceneComponent, false);
    }
}

void AC4Item::ShowComponent(USceneComponent *comp, bool flag){
    if(comp){
        comp->SetVisibility(flag, true); // affects children (true)       
    }
}


//on right click detonate, else: place new c4 item
void AC4Item::aim(bool aimstatus){
    Super::aim(aimstatus);

    //show press anim
    if(aimstatus){
        ShowPressAnimation();
        DetonateChildren();
    }
}

void AC4Item::FindComponentsOnBeginPlay(){
    TTryAssignByName<USceneComponent>("trigger", triggerSceneComponent); //from detonator
    TTryAssignByName<USceneComponent>("detonator", detonatorSceneComponent);

    TTryAssignByName<USceneComponent>("c4scene", c4SceneComponent);
    TTryAssignByName<USceneComponent>("c4LightComponent", c4light);

    TTryAssignByName<UPointLightComponent>("PointLight", c4PointLight);
    
    //debug
    FString found = c4PointLight ? TEXT("AC4Item::c4PointLight Found!") : TEXT("AC4Item::c4PointLight NOT FOUND");
    DebugHelper::logMessage(found);

    found = c4SceneComponent ? TEXT("AC4Item::c4scene Found!") : TEXT("AC4Item::c4scene NOT FOUND");
    DebugHelper::logMessage(found);
}

void AC4Item::CreateMaterialOnBeginPlay(){
    if(materialLightPrefab){
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(materialLightPrefab, this);
        if(DynMat){
            lightMaterialInstance = DynMat;
            if(c4light){
                UStaticMeshComponent *casted = Cast<UStaticMeshComponent>(c4light);
                if(casted){
                    casted->SetMaterial(0, lightMaterialInstance);
                }
            }
        }
    }
}



void AC4Item::UpdateMaterialEmmision(float scalar){
    if(lightMaterialInstance){
        lightMaterialInstance->SetScalarParameterValue("emissionValue", scalar);
    }
}

void AC4Item::UpdatePointLightEmmision(float scalar){
    if(c4PointLight){
        float intensityUpdate = PointLightMaxIntensity * scalar;
        c4PointLight->SetIntensity(intensityUpdate);
    }
}




void AC4Item::ShowPressAnimation(){

}




void AC4Item::DetonateChildren(){
    DebugHelper::logMessage("AC4Item::DetonateChildren: ", childs.Num());
    for (int i = 0; i < childs.Num(); i++)
    {
        if(AC4Item *current = childs[i]){
            current->DetonateByParent();
        }
    }
    childs.Empty();
}


void AC4Item::DetonateByParent(){
    markedDetonateByParent = true;
    Detonate();
}

void AC4Item::DetonateByDamage(){
    markedDetonateByParent = false;
    Detonate();
}


void AC4Item::Detonate(){
    
    //MUST BE RESETTET BY ENTITY MANAGER
    if(isDetonated){
        DebugHelper::logMessage("AC4Item::is already detonated!");
        return;
    }

    //isDetonated FLAGGED IN SUPER!
    Super::Detonate(); 

    //exploded by damage, cant be exploded again.
    if(!markedDetonateByParent){
        RemoveFromParent();
    }
    
    DetonateChildren(); //detonate children (safety)
}

void AC4Item::ResetFlagsAndProperties(){
    Super::ResetFlagsAndProperties();
    isDetonated = false;
}

void AC4Item::takedamage(int d){
    markedDetonateByParent = false;
    Super::takedamage(d);
}


void AC4Item::SetupTimer(){
    lightTimer.Begin(1.0f, true);
}

void AC4Item::TickLightStatus(float deltatime){
    if(!isPickedUp() && !isDetonated){
        lightTimer.TickWithTimesUpReset(deltatime);

        float timeAsScalar = lightTimer.scalar();
        //DebugHelper::logMessage(FString::Printf(TEXT("AC4Item::TimerScalar %.2f"), timeAsScalar));
        UpdateMaterialEmmision(timeAsScalar);
        UpdatePointLightEmmision(timeAsScalar);
    }
}

void AC4Item::RemoveFromParent(){
    if(parent){
        parent->RemoveFromChildList(this);
    }
    parent = nullptr;
}

void AC4Item::RemoveFromChildList(AC4Item *item){
    if(item){
        if(childs.Contains(item)){
            childs.Remove(item);
        }
    }
}


void AC4Item::SetParent(AC4Item *parentPtr){
    parent = parentPtr;
}

