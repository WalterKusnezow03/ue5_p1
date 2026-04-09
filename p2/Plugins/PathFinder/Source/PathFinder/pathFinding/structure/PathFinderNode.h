#pragma once

/*
class Node
	{
		public:
			static const int noneFx = -1;

			/// @brief will tell if the node is closed (on the closed list) or not
			bool closedFlag;
			/// @brief came from neighbor
			APathFinder::Node *camefrom = nullptr;
			float fx;
			float gx;
			FVector pos;

			
			

			Node(FVector posIn); //no id: setup needed
			Node(int id, FVector posIn); //id from storage

			Node(Node &other);
			Node &operator=(Node &other);

			~Node();
			void reset();
			void updateCameFrom(float gxIn, float hxEnd, Node &came);
			void close();
			bool isClosed();

			float oldfx;

			void setConvexNeighborA(Node *n);
			void setConvexNeighborB(Node *n);
			void addTangentialNeighbor(Node *n);

			//convex neighbors A and B
			APathFinder::Node *nA = nullptr;
			APathFinder::Node *nB = nullptr;

			bool hasNeighbors(); //convex hull neighbors
			bool hasAnyNeighbors(); //any visible neighbors 

			std::vector<Node *> visible_tangential_Neighbors;

			// new: hull index
			int hullindex = -1;
			bool sameHull(Node *other);

			void show(UWorld *world);

			//storage interface helpers
			void setId(int id);
			int getId();

			//only returns valid ids
			TArray<int> NeighborsById();

			//returns -1 if not valid
			int IdConvexNeighborA();
			int IdConvexNeighborB();

		private:
			FCriticalSection CriticalSection;

			//id system for saving
			int id = -1; //is invalid by default, not tracked in storage.
	};
    */