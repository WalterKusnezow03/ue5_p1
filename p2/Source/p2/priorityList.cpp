// Fill out your copyright notice in the Description page of Project Settings.

#include "PathFinder.h"
#include "priorityList.h"

priorityList::priorityList()
{
    
}

priorityList::~priorityList()
{
    
}





//outer methods

bool priorityList::hasNodes(){
    return this->nodes.size() > 0;
}

/// @brief returns the lowest fx pathfinder node
/// @return returns the lowest fx pathfinder node
PathFinder::Node *priorityList::popLowestFx(){

    if(nodes.size() > 0){
        PathFinder::Node *lowest = nodes.back();
        //remove from list
        nodes.erase(nodes.begin() + nodes.size() - 1);
        return lowest;
    }

    return nullptr;
}

/// @brief adds a path finder node to the priority queue
/// @param n 
void priorityList::add(PathFinder::Node *n){
    if(n != nullptr){
        float fxToAdd = n->fx;

        int i = 0;
        while(i < nodes.size()){
            if(nodes.at(i) != nullptr){
                float fxRead = nodes.at(i)->fx;
                if(fxToAdd > fxRead){
                    nodes.insert(nodes.begin() + i, n);
                    return; // Exit function after insertion
                }
            }else{
                nodes.erase(nodes.begin() + i);
                i--;
            }

            i++;
        }
        // If we reached the end of the vector, add the node to the end
        nodes.push_back(n);
    }
    
}




