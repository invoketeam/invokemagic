#pragma warning (disable : 4786)

#include "xRand.h"



xRand::xRand(void) 
{
	high = 1;
	low = 1 ^ 0x49616E42; 		
}//ctor



void 
xRand::setSeed(int seed)
{
	high = seed;
	low = seed ^ 0x49616E42;
}//setseed


int 
xRand::getRandInt(void)
{
	high = (high >> 2) + (high << 2);
	high += low;
	low += high;
	
	return (high % 32768);
}//getrand



float 
xRand::getRand(void)
{
	high = (high >> 2) + (high << 2);
	high += low;
	low += high;
	
	float rand2;
	rand2 = ((float)(high % 32768)) / 32768.0f;
	if (rand2 < 0.0f ) { rand2 = -rand2; }
	if (rand2 > 1.0f ) { rand2 = 1.0f; }

	return rand2;
}//getrand
