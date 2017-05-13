#ifndef _KIJELZO_	//ujabb definíció és fordítási hiba elkerülésére
#define _KIJELZO_

#include "ablak.hpp"

class KIJELZO : public ABLAK
{
	string puffer;
public:
	KIJELZO(double x, double y, int szine) 
		: ABLAK(x,y,33,33,650,szine*33+1,true)
	{};

	void srajzol(canvas &Tkepek, double X0, double Y0, double Xb, double Yb, double Xj, double Yj, KAMERA kamera, bool focus) const;

	void setter(istream& be);
	void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
	void getter(ostream& ki) const;
};

void KIJELZO::setter(istream& be)
{
	int szine,db;
	be >> szine;
	be >> db;
	if (db) {stringstream str; str << db; str >> puffer;} else puffer="";
	ky=szine*33+1;
}

void KIJELZO::getter(ostream& ki) const // Csak ha check boxot "játszik" akkor kell. Amíg nem szinkronizál a nem widgetben tárolt adatokkal.
{
	ki << (ky-1)/33;
}

void KIJELZO::srajzol(canvas &Tkepek, double X0, double Y0, double Xb, double Yb, double Xj, double Yj, KAMERA kamera, bool focus) const
{
	double ux,uy,usx,usy,ukx,uky;
	ux=x+X0;uy=y+Y0;usx=sx;usy=sy;ukx=kx;uky=ky;
	kamera.getKamCoords(ux,uy);
	if (ux+usx<Xb or ux>Xb+Xj or uy+usy<Yb or uy>Yb+Yj) return;
	if (ux+usx>Xb+Xj) {usx=Xb+Xj-ux;}
	if (ux<Xb) {usx+=ux-Xb; ukx-=ux; ux=Xb;}
	if (uy+usy>Yb+Yj) {usy=Yb+Yj-uy;}
	if (uy<Yb) {usy+=uy-Yb; uky-=uy; uy=Yb;}

	gout << stamp(Tkepek,ukx,uky,usx,usy,ux,uy);

	canvas c;
	c.open(462,462);
	c.transparent(true);

	c << color(255,255,255) << move_to(sx*3/8,sy*2/3) << text(puffer);
	gout << stamp(c,x,y);
}

#endif