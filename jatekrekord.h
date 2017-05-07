#ifndef _REKORD_
#define _REKORD_

#include <vector>

struct Player
{
	unsigned int babu;
	unsigned int szin;
};

struct Mezo;

struct Szomszed
{
	Mezo *f;
	Mezo *j;
	Mezo *l;
	Mezo *b;
};

struct Mezo
{
	unsigned int id;
	unsigned int szin;
	Szomszed szom;
	Mezo() // Todo: minden mező vele egvonalba lővő mezők pointereit ismernie kell.
	{
		id=-1;
		szin=9;
		szom.f=NULL;
		szom.j=NULL;
		szom.l=NULL;
		szom.b=NULL;
	}
};

struct Rekord
{
	Player p[2];
	bool AI;
	int max_lepesszam;
	unsigned int seed;
	vector<Mezo*> palya;
};

#endif