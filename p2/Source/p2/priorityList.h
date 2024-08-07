// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathFinder.h"

/**
 * 
 */
class P2_API priorityList
{
public:
	priorityList();
	~priorityList();

	void add(PathFinder::Node *node);
	PathFinder::Node *popLowestFx();

	bool hasNodes();

	

private:
	

	class knoten{
		public:
			knoten *next;
			knoten *prev;
			priorityList *parent;

			knoten(PathFinder::Node *n, priorityList *p);
			~knoten();

			PathFinder::Node *nutzlast;

			void add(PathFinder::Node *n, priorityList *p);
			void append(PathFinder::Node *n, priorityList *p);
			void insert(PathFinder::Node *n, priorityList *p);
			knoten *unlinklast();
	};

	void updateFirst(knoten *oldFirst, knoten *newFirst);
	void updateLast(knoten *oldLast, knoten *newLast);

	

	knoten *last;
	knoten *first;
};
