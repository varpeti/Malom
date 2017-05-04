#ifndef _REKORD_
#define _REKORD_

struct Player
{
	unsigned int babu;
	unsigned int szin;
};

struct Rekord
{
	Player p[2];
	bool AI;
	int max_lepesszam;
	unsigned int seed;
};

#endif