#ifndef _REKORD_
#define _REKORD_

#include <vector>

struct Player
{
	unsigned int babu; // Kézben lévő bábuk száma
	unsigned int lbabu; // Játékban lévő bábuk száma
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
	unsigned int id; // Sorfolytonosan a léphető mezők 0-tól
	unsigned int szin;
	Szomszed szom;
	Mezo()
	{
		id=-1;
		szin=9;
		szom.f=NULL;
		szom.j=NULL;
		szom.l=NULL;
		szom.b=NULL;
	}
};

struct Move
{
	int hova;
	int honnan;
	int leutotte;
};

struct Rekord
{
	Player p[2];
	bool AI;
	unsigned int seed;
	vector<Mezo*> palya;
	unsigned int nyertes;
	bool Classic;
	Move lastmove;
};

#endif