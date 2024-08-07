// Fill out your copyright notice in the Description page of Project Settings.

#include "PathFinder.h"
#include "priorityList.h"

priorityList::priorityList()
{
    first = nullptr;
    last = nullptr;
}

priorityList::~priorityList()
{
    knoten *f = first;
    while(f != nullptr){
        knoten *copy = f->next;
        delete (f);
        f = copy;
    }
}

priorityList::knoten::knoten(PathFinder::Node *n, priorityList *p){
    this->nutzlast = n;
    this->parent = p;
    prev = nullptr;
    next = nullptr;
}

priorityList::knoten::~knoten()
{

}



//outer methods

bool priorityList::hasNodes(){
    return (this->last != nullptr);
}

/// @brief returns the lowest fx pathfinder node
/// @return returns the lowest fx pathfinder node
PathFinder::Node *priorityList::popLowestFx(){

    if(last != nullptr){
        PathFinder::Node *copy = last->nutzlast;

        priorityList::knoten *prevCopy = last->unlinklast();
        delete last;
        if(prevCopy == nullptr){
            last = nullptr;
            first = nullptr; //reset all!
        }else{
            last = prevCopy;
        }

        return copy;
    }

    return nullptr;
}

/// @brief adds a path finder node to the priority queue
/// @param n 
void priorityList::add(PathFinder::Node *n){
    if(first != nullptr){
        first->add(n, this);
    }else{
        first = new knoten(n, this);
        last = first;
    }
}


void priorityList::updateFirst(knoten *oldFirst, knoten *newFirst){
    if(oldFirst == first){
        first = newFirst;
    }
}

void priorityList::updateLast(knoten *oldLast, knoten *newLast){
    if(oldLast == last){
        last = newLast;
    }
}





//inner class methods

/// @brief unlinks the last node and returns the new last node
/// @return new last to replace the last node
priorityList::knoten* priorityList::knoten::unlinklast(){
    if(prev != nullptr){
        prev->next = nullptr;
        return prev;
    }
    return nullptr;
}

void priorityList::knoten::add(PathFinder::Node *node, priorityList *p){
    bool deleteThis = false;
    if (node != nullptr)
    {

        //add functionality for updating when re open?
        if(node == this->nutzlast && node->fx < nutzlast->fx){
            deleteThis = true;
            if (next != nullptr)
            {
                next->prev = prev;
            }
            if(prev != nullptr){
                prev->next = next;
            }
        }



        float fx = node->fx;
        if(fx < nutzlast->fx){
            if(next != nullptr){
                next->add(node, p);
            }else{
                append(node, p); //nach mir einfügen
            }
        }else{
            insert(node, p);
        }
    }

    if(deleteThis){
        delete this;
    }
}

//insert back of own node OK
void priorityList::knoten::append(PathFinder::Node *n, priorityList *p){
    priorityList::knoten *knotenNeu = new knoten(n, p);

    if(next != nullptr){
        knotenNeu->next = next;
        next->prev = knotenNeu;
    }
    knotenNeu->prev = this;
    next = knotenNeu;

    parent->updateLast(this, knotenNeu);
}

//append infornt of own node
void priorityList::knoten::insert(PathFinder::Node *n, priorityList *p){
    priorityList::knoten *knotenNeu = new knoten(n, p);

    if(prev != nullptr){
        prev->next = knotenNeu;
        knotenNeu->prev = prev;
    }
    knotenNeu->next = this;
    prev = knotenNeu;
    parent->updateFirst(this, knotenNeu);
}



