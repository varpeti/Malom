#include "textbox.hpp"

static OBJ *tabla;

class MEZO : public ABLAK // A mező amin valamelyik játékos áll vagy állhat.
{
public:
	MEZO(double x, double y, int szine) 
		: ABLAK(x,y,33,33,650,szine*33+1,false) {};
};

class TABLA : public ABLAK
{
	private:
		int kattintva;
	public:

		TABLA(bool t[49])
			: ABLAK(83,33,462,462,683,0,true)
		{	
			for (int y = 0; y < 8; ++y) // A tábla 7*7-es de csak 24 mező van rajta.
			{
				for (int x = 0; x < 8; ++x)
				{
					if (t[x+y*7]) objektumok.push_back( new MEZO(16+x*66,16+y*66,rand()%7) );
				}
			}
			
			kattintva=0;

		}

		bool supdate(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
		void getter(ostream& ki) const;
};

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
					kattintva=i+1;
					return true;
				}
			}
			kattintva=0;
		}
	}
	return false;
}

void TABLA::getter(ostream& ki) const 
{
	ki << kattintva;
}

void initjatek(ENV &env)
{
	bool a[49] = {	1,0,0,1,0,0,1,	// A tábla
					0,1,0,1,0,1,0,
					0,0,1,1,1,0,0,
					1,1,1,0,1,1,1,
					0,0,1,1,1,0,0,
					0,1,0,1,0,1,0,
					1,0,0,1,0,0,1	};

	tabla = new TABLA(a); env.addObj(tabla);
}
