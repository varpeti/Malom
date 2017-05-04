#include "textbox.hpp"
#include "jatekrekord.h"

static OBJ *tabla;

class MEZO : public ABLAK // A mező amin valamelyik játékos áll vagy állhat.
{
public:
	MEZO(double x, double y, int szine) 
		: ABLAK(x,y,33,33,650,szine*33+1,false) 
	{};

	void setter(istream& be);
	void getter(ostream& ki) const;
};

void MEZO::setter(istream& be) // A setterbe 0-6ig vár színt.
{
	int p;
	be >> p;
	ky=p*33+1;
}

void MEZO::getter(ostream& ki) const 
{
	ki << ((ky-1)/33);
}

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
					if (t[x+y*7]) objektumok.push_back( new MEZO(16+x*66,16+y*66,6) );
				}
			}
			
			kattintva=0;

		}

		bool supdate(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
		void setter(istream& be);
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
	if (kattintva==-1) return;
	stringstream s;
	objektumok[kattintva]->getter(s);
	ki << s.str();
}

bool bjjj;

void mainjatek(ENV &env,Rekord &rekord)
{	
	env.ObjKiemel(tabla);
	tabla->setPosition(83,33);

	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		env.UpdateDrawHandle();
		if (env.ev.type==ev_mouse)
		{
			if (env.ev.button==btn_left)
			{	
				bjjj=!bjjj;
				int jatekos = bjjj;
				stringstream gs,ss;
				tabla->getter(gs);
				int ijsz;
				gs >> ijsz;
				if (ijsz==6) 
				{
					ss << rekord.p[jatekos].szin;
					tabla->setter(ss);
				}
				else if (ijsz==rekord.p[jatekos].szin)
				{
					ss << 6;
					tabla->setter(ss);
				}
			}else if (env.ev.button==-btn_left)
			{

			}
		}
	}

	tabla->setPosition(999,999);
}

bool okxy(bool a[49],int xx,int yy)
{
	unsigned char x=0,y=0;
	for (int i = 0; i < 7; ++i)
	{
		if (a[i+yy*7]) x++;
		if (a[xx+i*7]) y++;
	}
	return (x<3 and y<3);
}

void initjatek(ENV &env,Rekord &rekord)
{

	/*bool a[49] = {	1,0,0,1,0,0,1,	// A tábla
					0,1,0,1,0,1,0,
					0,0,1,1,1,0,0,
					1,1,1,0,1,1,1,
					0,0,1,1,1,0,0,
					0,1,0,1,0,1,0,
					1,0,0,1,0,0,1	};*/

	srand(rekord.seed);

	bool a[49] = {	0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,
					0,0,0,0,0,0,0};

	int mezoszam=0, lepes=0;
	while(mezoszam<21 and lepes<999) // Ez egy fair pálya generáló, ha ezer lépésből megvan.
	{
		int xx = rand()%7;
		int yy = rand()%7;
		if (!a[xx+yy*7] and okxy(a,xx,yy)) {a[xx+yy*7]=1; mezoszam++;}
		lepes++;
	}

	int i=0;
	while(mezoszam<21) // Ha nem lett meg ezerből, sorba megy és berakja oda ahol hiányzik.
	{
		if (!a[i] and okxy(a,i%7,i/7)) {a[i]=1; mezoszam++;}
		i++;
	}


	tabla = new TABLA(a); env.addObj(tabla);
	tabla->setPosition(999,999);
}
