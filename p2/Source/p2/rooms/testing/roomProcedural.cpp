// Fill out your copyright notice in the Description page of Project Settings.


#include "roomProcedural.h"

// Sets default values
AroomProcedural::AroomProcedural()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AroomProcedural::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AroomProcedural::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AroomProcedural::createRoom(
	FVector &location, //bottom left corner
	int scaleMetersX, 
	int scaleMetersY,
	int scaleMetersZ,
	std::vector<FVector> &doorPositions, //in local space to bottom left corner
	int doorWidthCm,
	std::vector<FVector> &windowPositions, //in local space to bottom left corner
	int windowWidthCm	
){
	int zCm = scaleMetersZ * 100;

	//erstmal den boden
	MeshData floorAndRoof;
	MeshData walls;

	//locale positionen, später verschieben
	FVector bl = FVector(0, 0, 0);
	FVector tl = bl + FVector(0, scaleMetersY * 100, 0);
	FVector tr = tl + FVector(scaleMetersX * 100, 0, 0);
	FVector br = bl + FVector(scaleMetersX * 100, 0, 0);
	createTwoSidedQuad(bl, tl, tr, br, floorAndRoof);

	//roof
	FVector offset(0, 0, zCm);
	FVector bl1 = bl + offset;
	FVector tl1 = tl + offset;
	FVector tr1 = tr + offset;
	FVector br1 = br + offset;
	createTwoSidedQuad(bl1, tl1, tr1, br1, floorAndRoof);


	//dann wände --> die man mit einer methode schreibt
	//türen nach wänden sortieren
	std::vector<FVector> corners = {bl, tl, tr, br, bl};

	

	for (int i = 1; i < corners.size(); i++)
	{
		FVector &from = corners[i - 1];
		FVector &to = corners[i];
		MeshData tmp = createWall(
			from, 
			to, 
			doorPositions,
			windowPositions, 
			doorWidthCm, 
			zCm,
			location
		);
		walls.append(tmp);
	}


	//testing from here needed to spawn the room / apply mesh(es)
	bool createNormals = true;
	updateMesh(floorAndRoof, createNormals, 0);
	updateMesh(walls, createNormals, 1);

	if (assetManager *e = assetManager::instance())
	{
        //floor
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::stoneMaterial), 0); //layer 0

		//walls
        ApplyMaterial(Mesh, e->findMaterial(materialEnum::wallMaterial), 1); //layer 0
	}




}



MeshData AroomProcedural::createWall(
	FVector from, 
	FVector to, 
	std::vector<FVector> &doors, //doors in cm local
	std::vector<FVector> &windows, //windows in cm local
	int doorWidthCm,
	int scaleZCm,
	FVector &locationOffset
){
	float epsilon = 10;

	std::vector<FVector> oneDimWall;
	oneDimWall.push_back(from);

	//will save the 1d window representation
	std::vector<FVector> oneDimWindows;

	std::vector<FVector> doorsFiltered;
	filterForVectorsBetween(
		from,
		to,
		doors,
		doorsFiltered //output
	);

	std::vector<FVector> windowsFiltered;
	filterForVectorsBetween(
		from,
		to,
		windows,
		windowsFiltered //output
	);

	//debug
	if(windows.size() > 0){
		FString compareWindows = FString::Printf(
			TEXT("windows %d, filtered %d"), 
			(int)windows.size(), 
			(int)windowsFiltered.size()
		);
		DebugHelper::logMessage(compareWindows);

	}
	


	//merge windows into doors to create gaps consistency
	for (int i = 0; i < windowsFiltered.size(); i++){
		doorsFiltered.push_back(windowsFiltered[i]);
	}


	// create gaps for wall
	FVector direction_to = (to - from).GetSafeNormal();
	direction_to.Z = 0;
	direction_to *= doorWidthCm;

	//iterate over filtered doors, make them "2" anchored
	//when the wall will be created, each second pair will be filled
	for (int i = 0; i < doorsFiltered.size(); i++){
		FVector &current = doorsFiltered[i];
		FVector A = current - direction_to;
		FVector B = current + direction_to;
		oneDimWall.push_back(A);
		oneDimWall.push_back(B);
	}
	//last part added here, wall "complete", lower ceiling 
	oneDimWall.push_back(to);

	//COPY WINDOW POSITIONS
	for (int i = 0; i < windowsFiltered.size(); i++){
		FVector &current = windowsFiltered[i];
		FVector A = current - direction_to;
		FVector B = current + direction_to;
		oneDimWindows.push_back(A);
		oneDimWindows.push_back(B);
	}


	FVector toTop(0, 0, std::abs(scaleZCm));

	//CREATE WALL WITH GAPS
	std::vector<TTouple<FVector, FVector>> twoDimWall;
	for (int i = 0; i < oneDimWall.size(); i++){
		FVector bottom = oneDimWall[i];
		FVector top = bottom + toTop;

		TTouple<FVector, FVector> touple(bottom, top);
		twoDimWall.push_back(touple);
	}

	//PREPARE WINDOWS FOR WINDOW CREATION
	std::vector<TTouple<FVector, FVector>> twoDimWindows;
	for (int i = 0; i < oneDimWindows.size(); i++){
		FVector bottom = oneDimWindows[i];
		FVector top = bottom + toTop;

		TTouple<FVector, FVector> touple(bottom, top);
		twoDimWindows.push_back(touple);
	}





	MeshData output;
	
	//weil jede hinzugefügte tür 2 teile hinzufügt, wird nur jede 2 vertecies eine wand erstellt,
	//wand, wenn lücke, lücke, ansonsten wand. Quasi.
	for (int i = 1; i < twoDimWall.size(); i += 2){
		TTouple<FVector, FVector> &first = twoDimWall[i - 1];
		TTouple<FVector, FVector> &second = twoDimWall[i];
		FVector a = first.first(); //bottom
		FVector b = first.last(); //top
		FVector c = second.last(); //top1
		FVector d = second.first(); //bottom1

		createTwoSidedQuad(a, b, c, d, output);
	}


	//spawn windows seperately
	spawnWindowMeshFromBounds(
		twoDimWindows,
		locationOffset
	);

	//return walls
	return output;
}




/// @brief creates a new mesh from touples representing the vertical edges of an window
/// @param windowTouples 
/// @param offset 
void AroomProcedural::spawnWindowMeshFromBounds(
	std::vector<TTouple<FVector, FVector>> &windowTouples,
	FVector &offset
){

	assetManager *assetManagerPointer = assetManager::instance();
	EntityManager *e = worldLevel::entityManager();
	if(assetManagerPointer != nullptr && e != nullptr){
		for (int i = 1; i < windowTouples.size(); i += 2){

			AcustomMeshActor *newActor = e->spawnAcustomMeshActor(GetWorld(), offset);
			if(newActor != nullptr){
				
				TTouple<FVector, FVector> &first = windowTouples[i - 1];
				TTouple<FVector, FVector> &second = windowTouples[i];
				FVector a = first.first(); //bottom
				FVector b = first.last(); //top
				FVector c = second.last(); //top1
				FVector d = second.first(); //bottom1
				//newActor->SetActorLocation(offset);
				
				//fix offset for mesh, move center to window, move local coors to "0,0,0"

				FVector center = (d + a) / 2;
				FVector fromOffset = offset + center;
				
				a -= center; //nur an den ursprung mit dem offset weil a bis d local space vom raum sind
				b -= center;
				c -= center;
				d -= center;
				
				newActor->SetActorLocation(fromOffset);

				FVector zeroVec(0, 0, 0);
				DebugHelper::showLineBetween(GetWorld(), zeroVec, fromOffset, FColor::Red);

				// create mesh
				newActor->createTwoSidedQuad(
					a, b, c, d,
					assetManagerPointer->findMaterial(materialEnum::glassMaterial),
					true
				);

				//set splitting on death to true
				bool splitGlass = true;
				newActor->setMaterialBehaiviour(materialEnum::glassMaterial, splitGlass);
			}
		}
		
	}

}







/// @brief will filter positions from the positions to filter vector
/// and add the positions which meet the requirement into the filtered list
/// @param A start
/// @param B end
/// @param positionsToFilter any vectors
/// @param output vectors between a and b / paralell to AB
void AroomProcedural::filterForVectorsBetween(
	FVector &A,
	FVector &B,
	std::vector<FVector> &positionsToFilter,
	std::vector<FVector> &output
){
	FVector zeroVec(0, 0, 0);
	FVector AB = (B - A).GetSafeNormal();
	FVector BA = (A - B).GetSafeNormal();

	int minDistance = 150;

	//todo: hinzufügen wenn AB distanz < min distanz, wie man dann mit türen und fenstern umgeht!

	for (int i = 0; i < positionsToFilter.size(); i++){
		FVector &current = positionsToFilter[i];
		FVector AC = (current - A); //for distance
		FVector ACdir = AC.GetSafeNormal(); //direction

		//vector from A to B and A to wall will be paralell if the wall is in interest
		float dot = AB.X * ACdir.X + AB.Y * ACdir.Y;
		
		if(dot >= 0.99f){

			DebugHelper::logMessage(TEXT("DEBUG_VECTOR paralell?"), AB, AC);

			//also needs to have an min distance from corners of half window / door width
			FVector BC = (current - B);
			if(
				FVector::Dist(zeroVec, AC) >= minDistance &&  //distance for safety not create weird windows at corners
				FVector::Dist(zeroVec, BC) >= minDistance
			){
				output.push_back(current);
			}

			//output.push_back(current);
		}


		//OBWOHL MATHE MATISCH KORREKT DENNOCH FEHLER ANFÄLLIG!

		//es reicht auch aus beide skalar produkte zu nehmen, wenn eins negativ (anti paralell) ist
		//dann overshootet die position
		//man braucht AB und BA jeweils um die richtung zu ändern
		/*
		//reverse dot product, must be near -1 for paralell in reverse dir
		float dotReversed = BA.X * ACdir.X + BA.Y * ACdir.Y;
		if(
			dot >= 0.99f &&
			dotReversed <= -0.99f //<= more towards -1
		){
			output.push_back(current);
		}
		*/
	}
}





/**
 * static method section
 */



AroomProcedural* AroomProcedural::spawnRoom(UWorld *world, FVector location){
	if(world == nullptr){
		return nullptr;
	}

	FActorSpawnParameters params;
	AroomProcedural *spawned = world->SpawnActor<AroomProcedural>(
		AroomProcedural::StaticClass(),
		location,
		FRotator::ZeroRotator,
		params
	);
	return spawned;
}



void AroomProcedural::spawnRooms(UWorld* world, FVector location, std::vector<roomBounds> &vec){
	if(world == nullptr){
		return;
	}

	int heightZDefault = 3;
	int doorAndWindowWidth = 100;

	//process all rooms to be created
	for (int i = 0; i < vec.size(); i++){
		roomBounds &currentRoom = vec.at(i);
		//create proper offset in xpos and ypos as needed

		FVector additionOffset(currentRoom.xpos() * 100, currentRoom.ypos() * 100, 0);
		FVector fullOffset = location + additionOffset;
		AroomProcedural *newRoom = spawnRoom(world, fullOffset);
		if(newRoom != nullptr){
			std::vector<FVector> doorPositionsRelativeInMeters = currentRoom.relativeDoorPositionsCm();
			std::vector<FVector> windowPositionsRelativeInMeters = currentRoom.relativeWindowPositionsCm();

			newRoom->createRoom(
				fullOffset,
				currentRoom.xscale(),
				currentRoom.yscale(),
				heightZDefault,
				doorPositionsRelativeInMeters,
				doorAndWindowWidth, 
				windowPositionsRelativeInMeters, 
				doorAndWindowWidth
			);
		}

	}
}

