#include "textbox.hpp"

class MEZO : public ABLAK // A mező amin valamelyik játékos áll vagy állhat.
{
public:
	MEZO(double x, double y, int szine) 
		: ABLAK(x,y,33,33,650,szine*33+1,false)
	{};

	void setter(istream& be);
	void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
};

void MEZO::setter(istream& be) // A setterbe 0-6ig vár színt.
{
	int p;
	be >> p;
	ky=p*33+1;
}

class TABLA : public ABLAK
{
	private:
		int kattintva;
		vector<Mezo*> m;
	public:

		TABLA(vector<Mezo*> m)
			: ABLAK(119,69,462,462,683,0,true), m(m)
		{	
			for (int y = 0; y < 7; ++y) // A tábla 7*7-es de csak 21 vagy kevesebb mező van rajta.
			{
				for (int x = 0; x < 7; ++x)
				{
					if (m[x+y*7]->szin==6) {m[x+y*7]->id=objektumok.size(); objektumok.push_back( new MEZO(16+x*66,16+y*66,6) ); } // Létrehozzuk és id-t adunk hogy tudjunk rá hivatkozni.
				}
			}
			
			kattintva=0;

		}

		void srajzol(canvas &Tkepek, double X0, double Y0, double Xb, double Yb, double Xj, double Yj, KAMERA kamera, bool focus) const;
		bool supdate(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
		void setter(istream& be);
		void getter(ostream& ki) const;
};

void TABLA::srajzol(canvas &Tkepek, double X0, double Y0, double Xb, double Yb, double Xj, double Yj, KAMERA kamera, bool focus) const
{
	double ux,uy,usx,usy,ukx,uky; // Lekellett volna cserélni ezt a szépséget canvesre mielött elkezdem írni a malmot...
	ux=x+X0;uy=y+Y0;usx=sx;usy=sy;ukx=kx;uky=ky;
	kamera.getKamCoords(ux,uy);
	if (ux+usx<Xb or ux>Xb+Xj or uy+usy<Yb or uy>Yb+Yj) return;
	if (ux+usx>Xb+Xj) {usx=Xb+Xj-ux;}
	if (ux<Xb) {usx+=ux-Xb; ukx-=ux; ux=Xb;}
	if (uy+usy>Yb+Yj) {usy=Yb+Yj-uy;}
	if (uy<Yb) {usy+=uy-Yb; uky-=uy; uy=Yb;}

	gout << stamp(Tkepek,ukx,uky,usx,usy,ux,uy); // Kép

	canvas c;
	c.open(462,462); 
	c.transparent(true);

	for (int i = 0; i < 49; ++i) // Vonal rajzolás
	{
		if (m[i]->szin!=9)
		{	
			c << color(255,255,255);
			double nx,ny,mx,my;
			objektumok[m[i]->id]->getPosition(nx,ny); nx+=16.5; ny+=16.5;
			kamera.getKamCoords(nx,ny);

			if (m[i]->szom.j) { // Csak a közvetlen szömszédokból a jobbra és le
				objektumok[m[i]->szom.j->id]->getPosition(mx,my); mx+=16.5; my+=16.5;
				kamera.getKamCoords(mx,my);
				c << move_to(nx,ny) << line_to(mx,my);
			}

			if (m[i]->szom.f) {
				objektumok[m[i]->szom.f->id]->getPosition(mx,my); mx+=16.5; my+=16.5;
				kamera.getKamCoords(mx,my);
				c << move_to(nx,ny) << line_to(mx,my);
			}

		}
	}
	gout << stamp(c,x,y);

	for (int i = 0; i < objektumok.size();++i) // Mezők és a vonalak kirajzolása
	{
		objektumok[i]->srajzol(Tkepek,x+X0,y+Y0,ux,uy,usx,usy,kamera,(i+1==objektumok.size()&&focus)); // Ha utolsó elem és az ablak is az utolsó: focusban van.
	}
}

bool TABLA::supdate(event ev, double X0, double Y0, KAMERA kamera)
{
	if (ev.type==ev_mouse)
	{
		if (ev.button==btn_left)
		{
			double ux,uy; // Kamera elmozdulás miatt kellenek.
			ux=x+X0;uy=y+Y0;
			kamera.getKamCoords(ux,uy);
			for (int i = objektumok.size()-1; i >= 0; i--)
			{
				if (objektumok[i]->BenneVan(ev.pos_x-ux,ev.pos_y-uy)) {
					kattintva=i;
					return true;
				}
			}
			kattintva=-1;
		}
	}
	return false;
}

void TABLA::setter(istream& be)
{
	if (kattintva==-1) return;
	objektumok[kattintva]->setter(be);
}

void TABLA::getter(ostream& ki) const 
{
	ki << kattintva;
}