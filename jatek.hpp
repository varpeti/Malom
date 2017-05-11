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
	void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
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
		vector<Mezo*> m;
	public:

		TABLA(vector<Mezo*> m)
			: ABLAK(83,33,462,462,683,0,true), m(m)
		{	
			for (int y = 0; y < 7; ++y) // A tábla 7*7-es de csak 24 mező van rajta.
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
	if (kattintva==-1) return;
	stringstream s;
	objektumok[kattintva]->getter(s);
	ki << s.str() << " " << kattintva;
}

int gethanyadikfromid(Rekord &rekord,int id)
{
	for (int i = 0; i < rekord.palya.size(); ++i)
	{
		if (id==rekord.palya[i]->id) return i;
	}
	return -1;
}

bool szomszede(vector<int> &szomszedok,int id)
{
	for (int i = 0; i < szomszedok.size(); ++i)
	{
		if (szomszedok[i]==id) return true;
	}
	return false;
}

void getszomszedok(Rekord &rekord,int i,vector<int> &szomszedok)
{
	int id = gethanyadikfromid(rekord,i);
	if (rekord.palya[id]->szom.b) szomszedok.push_back(rekord.palya[id]->szom.b->id);
	if (rekord.palya[id]->szom.f) szomszedok.push_back(rekord.palya[id]->szom.f->id);
	if (rekord.palya[id]->szom.j) szomszedok.push_back(rekord.palya[id]->szom.j->id);
	if (rekord.palya[id]->szom.l) szomszedok.push_back(rekord.palya[id]->szom.l->id);
}

bool lepes(bool jatekos,ENV &env,Rekord &rekord)
{
	bool felszedve = false;
	vector<int> szomszedok;
	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		env.UpdateDrawHandle();
		if (env.ev.type==ev_mouse)
		{
			if (env.ev.button==btn_left)
			{	
				stringstream gs,ss;
				tabla->getter(gs);
				int ijsz, kat;
				gs >> ijsz;
				gs >> kat; cout << kat << endl;
				//     Üres   és     van bábuja           és vagy nem szedte fel vagy szomszédba rakta le
				if (ijsz==6 and rekord.p[jatekos].babu>0 and  (!felszedve or szomszede(szomszedok,kat)) ) 
				{
					ss << rekord.p[jatekos].szin;
					tabla->setter(ss);
					rekord.p[jatekos].babu--;
					return true; // Átadja a kört
				}
				else if (ijsz==rekord.p[jatekos].szin and !felszedve) // Bábu felszedése
				{
					getszomszedok(rekord,kat,szomszedok);
					if (szomszedok.size()>0){
						ss << 6;
						tabla->setter(ss);
						rekord.p[jatekos].babu++;
						felszedve=true;
					}
				}
			}else if (env.ev.button==-btn_left)
			{

			}
		}
	}
	return false; // Kilép a menübe
}

void mainjatek(ENV &env,Rekord &rekord)
{	
	env.ObjKiemel(tabla);
	tabla->setPosition(83,33);

	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		if (!lepes(0,env,rekord)) break;
		if (!lepes(1,env,rekord)) break;
	}


	tabla->setPosition(999,999);
}

bool okxy(vector<Mezo*> m,int xx,int yy)
{
	unsigned char x=0,y=0;
	for (int i = 0; i < 7; ++i)
	{
		if (m[i+yy*7]->szin!=9) x++;
		if (m[xx+i*7]->szin!=9) y++;
	}
	return (x<3 and y<3);
}

void vonalaz(vector<Mezo*> m,int xx,int yy) // rekurzívan végigmegy az egész táblán és feltölti ki-kinek a szomszédja.
{	
	if (m[xx+yy*7]->szin!=6) {vonalaz(m,xx+1,yy); return;} // Ha olyannal kezdődik ami nem léphető mező akkor elindul jobbra keresni egyet.

	for (int x = xx-1; x >= 0 and !m[xx+yy*7]->szom.b; --x) // Balra
		if (m[x+yy*7]->szin==6) {m[xx+yy*7]->szom.b=m[x+yy*7]; vonalaz(m,x,yy);}

	for (int x = xx+1; x <  7 and !m[xx+yy*7]->szom.j; ++x) // Jobbra
		if (m[x+yy*7]->szin==6) {m[xx+yy*7]->szom.j=m[x+yy*7]; vonalaz(m,x,yy);}

	for (int y = yy-1; y >= 0 and !m[xx+yy*7]->szom.f; --y) // Fel
		if (m[xx+y*7]->szin==6) {m[xx+yy*7]->szom.f=m[xx+y*7]; vonalaz(m,xx,y);}

	for (int y = yy+1; y <  7 and !m[xx+yy*7]->szom.l; ++y) // Le
		if (m[xx+y*7]->szin==6) {m[xx+yy*7]->szom.l=m[xx+y*7]; vonalaz(m,xx,y);}

}

void initjatek(ENV &env,Rekord &rekord)
{

	srand(rekord.seed);

	for (int i = 0; i < 49; ++i)
	{
		rekord.palya.push_back(new Mezo);
	}

	int mezoszam=0, lepes=0;
	while(mezoszam<21 and lepes<999) // Ez egy fair pálya generáló, ha ezer lépésből megvan.
	{
		int xx = rand()%7;
		int yy = rand()%7;
		if ( rekord.palya[xx+yy*7]->szin==9 and okxy(rekord.palya,xx,yy)) {rekord.palya[xx+yy*7]->szin=6; mezoszam++;}
		lepes++;
	}

	int i=0;
	while(mezoszam<21 and i<49) // Ha nem lett meg ezerből, sorba megy és berakja oda ahol hiányzik. 
	{
		if ( rekord.palya[i]->szin==9 and okxy(rekord.palya,i%7,i/7)) {rekord.palya[i]->szin=6; mezoszam++;}
		i++;
	}

	vonalaz(rekord.palya,0,0); // Ha nem léphető mező akkor elöbb keres egyet.

	rekord.p[0].babu= (rekord.p[0].babu==8 ? 8 : 7); // easter egg jutalma
	rekord.p[1].babu=7;

	tabla = new TABLA(rekord.palya); env.addObj(tabla);
	tabla->setPosition(999,999);
}

void endjatek(ENV &env, Rekord &rekord)
{
	env.delObj(tabla);

	while (rekord.palya.size()>0)
	{
		delete rekord.palya[rekord.palya.size()-1];
		rekord.palya.pop_back();
	}
}
