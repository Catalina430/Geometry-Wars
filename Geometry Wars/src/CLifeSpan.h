#pragma once
class CLifeSpan
{
public:
								
								
	int remaining{ 0 };			//total = 100,remaining 100	->alive for 30 frames->remaining 70 
	int total{ 0 };				//(ie.substract 1 every frame for int remaining )
								//Amount of lifespan remaining on the entity
	CLifeSpan(int total)		//Initial amount of lifespan
		: remaining(total), total(total) {}
};

