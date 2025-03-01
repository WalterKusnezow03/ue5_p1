// Fill out your copyright notice in the Description page of Project Settings.


#include "roomProcedural.h"
#include "p2/rooms/layoutCreator/layoutMaker.h"


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
	FVector location,
	roomBoundData &currentRoom,
	int oneMeter
){
	FVector fullOffset = location; //+ currentRoom.positionInMeterSpace(oneMeter);
	SetActorLocation(fullOffset);
	
	std::vector<FVector> doorPositionsRelativeInMeters = currentRoom.relativeDoorPositionsCm();
	std::vector<FVector> windowPositionsRelativeInMeters = currentRoom.relativeWindowPositionsCm();

	createRoom(
		fullOffset,
		currentRoom.xScale(),
		currentRoom.yScale(),
		3, //3 meters height
		doorPositionsRelativeInMeters,
		windowPositionsRelativeInMeters, 
		oneMeter
	);

}




/// @brief creates the room and mesh and applies it
/// @param location location to spawn at (Bottom left corner of bounds)
/// @param scaleMetersX meters on x
/// @param scaleMetersY meters on y
/// @param scaleMetersZ meters height
/// @param doorPositions door positions 
/// @param doorWidthCm door width (should be ideally 1m as the layout, will be scaled up)
/// @param windowPositions relative positions to have windows at
/// @param windowWidthCm window width: should be just as room layout, 1m (100cm)
void AroomProcedural::createRoom(
	FVector &location, //bottom left corner
	int scaleMetersX, 
	int scaleMetersY,
	int scaleMetersZ,
	std::vector<FVector> &doorPositions, //in local space to bottom left corner
	std::vector<FVector> &windowPositions, //in local space to bottom left corner
	int onemeter
){
	int zCm = scaleMetersZ * 100;
	int windowWidthCm = onemeter;
	int doorWidthCm = onemeter;

	// erstmal den boden
	MeshData floorAndRoof;
	MeshData walls;

	//locale positionen, später verschieben
	std::vector<FVector> quad = MeshData::create2DQuadVertecies(scaleMetersX * onemeter, scaleMetersY * onemeter);
	floorAndRoof.appendDoublesided(quad[0], quad[1], quad[2], quad[3]);

	//roof
	std::vector<FVector> quadcopy = quad;
	FVector offset(0, 0, zCm);
	for (int i = 0; i < quadcopy.size(); i++){
		quadcopy[i] += offset;
	}
	floorAndRoof.appendDoublesided(quadcopy[0], quadcopy[1], quadcopy[2], quadcopy[3]);


	//dann wände --> die man mit einer methode schreibt
	//türen nach wänden sortieren
	//std::vector<FVector> corners = {bl, tl, tr, br, bl};
	std::vector<FVector> corners = {quad[0], quad[1], quad[2], quad[3], quad[0]};

	
	//debug draw doors
	/*
	for(int i = 0; i < doorPositions.size(); i++){
		FVector transformed = location + doorPositions[i];
		FVector upV = transformed + FVector(0, 0, zCm);
		DebugHelper::showLineBetween(GetWorld(), transformed, upV, FColor::Green);
	}*/

	FVector centerOfRoom = FVectorUtil::calculateCenter(quad[0], quad[1], quad[2], quad[3]);

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
			location,
			centerOfRoom
		);
		walls.appendEfficent(tmp);
	}


	//testing from here needed to spawn the room / apply mesh(es)
	floorAndRoof.calculateNormals();
	walls.calculateNormals();
	replaceMeshData(floorAndRoof, materialEnum::stoneMaterial);
	replaceMeshData(walls, materialEnum::wallMaterial);
	ReloadMeshAndApplyAllMaterials();
}





MeshData AroomProcedural::createWall(
	FVector from, 
	FVector to, 
	std::vector<FVector> &doors, //doors in cm local
	std::vector<FVector> &windows, //windows in cm local
	int doorWidthCm,
	int scaleZCm,
	FVector &locationOffset,
	FVector &centerOfRoom
){
	//sort vectors for consistent door placement, otherwise overlap issues occur
	//very important!
	FVector connect = to - from;
	if(connect.X < 0 || connect.Y < 0){
		FVector copy = to;
		to = from;
		from = copy;
	}
	

	std::vector<FVector> oneDimWall;
	oneDimWall.push_back(from); //FIRST WALL POSITION

	//will save the 1d window representation
	std::vector<FVector> oneDimWindows;

	std::vector<FVector> doorsFiltered;
	filterForVectorsBetween(
		from,
		to,
		doorWidthCm,
		doors,
		doorsFiltered //output
	);

	std::vector<FVector> windowsFiltered;
	filterForVectorsBetween(
		from,
		to,
		doorWidthCm,
		windows,
		windowsFiltered //output
	);

	
	

	
	// create gaps for wall
	FVector direction_to = (to - from).GetSafeNormal() * doorWidthCm;
	direction_to.Z = 0;

	//
	for (int i = 0; i < windowsFiltered.size(); i++){
		//doorsFiltered.push_back(windowsFiltered[i]);
		FVector &current = windowsFiltered[i];
		FVector A = current;
		FVector B = current + direction_to;
		oneDimWall.push_back(A);
		oneDimWall.push_back(B);
	}
	//sortVectorsBetween(from, to, doorsFiltered);



	//iterate over filtered doors, make them "2" anchored
	//when the wall will be created, each second pair will be filled
	int doorWidthIncrease = 2;
	for (int i = 0; i < doorsFiltered.size(); i++)
	{
		FVector &current = doorsFiltered[i];
		FVector A = current;
		FVector B = current + direction_to * doorWidthIncrease; //türen breiter machen
		oneDimWall.push_back(A);
		oneDimWall.push_back(B);
	}
	//last part added here, wall "complete", lower ceiling 
	oneDimWall.push_back(to); //FINAL POSITION
	sortVectorsBetween(from, to, oneDimWall);

	

	FVector toTop(0, 0, std::abs(scaleZCm)); //direction to top
	//CREATE WALL WITH GAPS
	std::vector<TTouple<FVector, FVector>> twoDimWall;
	for (int i = 0; i < oneDimWall.size(); i++){
		FVector bottom = oneDimWall[i];
		FVector top = bottom + toTop;

		TTouple<FVector, FVector> touple(bottom, top);
		twoDimWall.push_back(touple);

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

		//draw wall
		DebugHelper::showLineBetween(
			GetWorld(),
			a + locationOffset,
			c + locationOffset,
			FColor::Green
		);
		DebugHelper::showLineBetween(
			GetWorld(),
			b + locationOffset,
			d + locationOffset,
			FColor::Green
		);

		//debug draw door
		if(i + 1 < twoDimWall.size()){
			TTouple<FVector, FVector> &next = twoDimWall[i + 1];
			FVector e = next.first(); //bottom
			FVector f = next.last(); //top
			DebugHelper::showLineBetween(
				GetWorld(),
				c + locationOffset,
				d + locationOffset,
				FColor::Yellow
			);
			DebugHelper::showLineBetween(
				GetWorld(),
				e + locationOffset,
				f + locationOffset,
				FColor::Yellow
			);

		}


		//flat wall for now
		//createTwoSidedQuad(a, b, c, d, output);

		//new extruded wall	
		//testdir
		FVector sideDir = (d - a); //AB = B - A

		//um einen vektor um 90 grad zu drehen, einen comp negieren,
		//und x und y vertauschen
		FVector orthogonalDir(
			sideDir.Y * -1,
			sideDir.X,
			sideDir.Z
		);

		//nach innen drehen
		FVector aToCenter = centerOfRoom - a; //AB = B - A
		float dotProduct = (orthogonalDir.X * aToCenter.X) * (orthogonalDir.Y * aToCenter.Y);
		if(dotProduct < 0.0f){ //orthogonal zeigt weg von raum, drehen nach innen
			orthogonalDir *= -1;
		}

		int widthCm = 20;
		output.appendCube(
			a,
			b,
			c,
			d,
			orthogonalDir.GetSafeNormal() * widthCm
		);

	}


	// --- CREATE WINDOWS ---
	
	//COPY WINDOW POSITIONS
	for (int i = 0; i < windowsFiltered.size(); i++){
		FVector &current = windowsFiltered[i];
		FVector A = current;
		FVector B = current + direction_to;
		oneDimWindows.push_back(A);
		oneDimWindows.push_back(B);
	}

	//PREPARE WINDOWS FOR WINDOW CREATION
	std::vector<TTouple<FVector, FVector>> twoDimWindows;
	for (int i = 0; i < oneDimWindows.size(); i++){
		FVector bottom = oneDimWindows[i];
		FVector top = bottom + toTop;

		TTouple<FVector, FVector> touple(bottom, top);
		twoDimWindows.push_back(touple);
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
				
				//fix offset for mesh, move pivot to bottom center of window, move local coors to "0,0,0"

				FVector center = (d + a) / 2;
				FVector fromOffset = offset + center;
				
				a -= center; //nur an den ursprung mit dem offset weil a bis d local space vom raum sind
				b -= center;
				c -= center;
				d -= center;
				
				newActor->SetActorLocation(fromOffset); //apply offset

				FVector zeroVec(0, 0, 0);
				fromOffset += FVector(0, 0, 10);
				//DebugHelper::showLineBetween(GetWorld(), zeroVec, fromOffset, FColor::Red);

				// create mesh

				MeshData &glassMesh = newActor->findMeshDataReference(
					materialEnum::glassMaterial,
					ELod::lodNear,
					true
				);
				glassMesh.appendDoublesided(a, b, c, d);
				newActor->ReloadMeshAndApplyAllMaterials();
				/*
				newActor->createTwoSidedQuad(
					a, b, c, d,
					materialEnum::glassMaterial
				);*/

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
	int minDistance,
	std::vector<FVector> &positionsToFilter,
	std::vector<FVector> &output
){
	FVector zeroVec(0, 0, 0);
	FVector AB = (B - A).GetSafeNormal();
	FVector BA = (A - B).GetSafeNormal();

	minDistance = std::abs(minDistance);

	float distanceBetweenAB = FVector::Dist(A, B);

	//minDistance *= 0.9f;

	
	for (int i = 0; i < positionsToFilter.size(); i++){
		FVector &current = positionsToFilter[i];
		FVector AC = (current - A); //for distance
		FVector ACdir = AC.GetSafeNormal(); //direction

		//vector from A to B and A to wall will be paralell if the wall is in interest
		float dot = AB.X * ACdir.X + AB.Y * ACdir.Y;
		
		if(dot >= 0.99f){

			FVector BC = (current - B);
			float distanceToStartPoint = lenghtOf(AC);
			float distanceToEndPoint = lenghtOf(BC);

			if(
				distanceToStartPoint >= minDistance && //distanze minimal
				distanceToEndPoint >= minDistance &&
				distanceToStartPoint < distanceBetweenAB && //distanz kleiner als maximal sodass innen
				distanceToEndPoint < distanceBetweenAB
			){
				output.push_back(current);
			}

		}
	}


	
}

float AroomProcedural::lenghtOf(FVector &vec){
	FVector zeroVec(0, 0, 0);
	return std::abs(FVector::Dist(zeroVec, vec));
}


void AroomProcedural::sortVectorsBetween(FVector &A, FVector &B, std::vector<FVector> &output){
	FVector AB = B - A;

	bool vectorIsXDirection = AB.X != 0; //ansonsten Z
	bool isNegativeToPositive = true;
	if(vectorIsXDirection){
		isNegativeToPositive = AB.X > 0; //from 0 to 8 for example direction
	}else{
		isNegativeToPositive = AB.Y > 0; //from 0 to 8 for example direction
	}


	std::sort(
		output.begin(), output.end(),
		[vectorIsXDirection, isNegativeToPositive]
		(const FVector &a, const FVector &b) {
		if(vectorIsXDirection){
			return a.X < b.X;
		}else{
			return a.Y < b.Y;
		}
	});

	//debug print sorted vector
	DebugHelper::logMessage("debug vector sorted: ");
	for (int i = 0; i < output.size(); i++)
	{
		int number = vectorIsXDirection ? output.at(i).X : output.at(i).Y;
		FString s = FString::Printf(TEXT("debug vector sorted %d"), number);
		DebugHelper::logMessage(s); // sieht gut aus
	}
}















/**
 * static method section
 */

/// @brief spawns a vector of roomBound data into the world
/// @param worldIn world to spawn in
/// @param location location to spawn at
/// @param vec vector of rooms
void AroomProcedural::spawnRooms(UWorld* worldIn, FVector location, std::vector<roomBoundData> &vec){
	if(worldIn == nullptr){
		return;
	}

	//process all rooms to be created
	for (int i = 0; i < vec.size(); i++){
		roomBoundData &currentRoom = vec.at(i);
		//create proper offset in xpos and ypos as needed

		FVector fullOffset = location + currentRoom.positionInMeterSpace(100);
		AroomProcedural *newRoom = spawnRoom(worldIn, fullOffset);
		if(newRoom != nullptr){

			newRoom->createRoom(
				fullOffset,
				currentRoom,
				100
			);

		}

	}
}


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










/**
 * 
 * 
 * 
 * ----- new testing section fvector shape / tree alike generation 
 * planned here
 * 
 * 
 * 
 */
void AroomProcedural::generate(UWorld *world, int sizeXMeters, int sizeYMeters, FVector location){
	std::vector<TTouple<int, int>> sizesPossible;
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 2, sizeYMeters / 2));
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 3, sizeYMeters / 3));
    sizesPossible.push_back(TTouple<int, int>(sizeXMeters / 4, sizeYMeters / 4));

    std::vector<roomBoundData> outputRooms;
    layoutMaker l;
    l.makeLayout(20, 20, sizesPossible, outputRooms);
    AroomProcedural::spawnRooms(world, location, outputRooms);
	
}