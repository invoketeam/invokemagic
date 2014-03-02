#pragma once



class xRand
{
public:
	int high;
	int low;
public:
	xRand();

	void setSeed(int seed);

	int getRandInt();

	float getRand();

};//xrand