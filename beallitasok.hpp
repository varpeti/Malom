#include "kivalaszto.hpp"
#include "szambeallito.hpp"
#include "jatekrekord.h"

static OBJ *beal;

class beallitasok : public ABLAK// Egyepéldányos leszármazott osztály
{
	private:
		int kattintva;
	public:
		beallitasok()
				: ABLAK(25,100,650,400,0,0,false) // Fix az játékablakméret és a csak 1 példány miatt.
		{

			objektumok.push_back( new KIVALASZTO(15,30,SZIN(200,200,100),SZIN(20,10,5),{"Red","Green","Cyan","Blue","Magenta"},5) ); //70x149 (1 mező: 31)
			objektumok.push_back( new KIVALASZTO(100,30,SZIN(200,200,100),SZIN(20,10,5),{"Yellow","Green","Cyan","Blue","Magenta"},5) );
			objektumok.push_back( new STATTEXT(15,10,SZIN(200,200,100),SZIN(20,10,5),"   P1    |    P2   ") );
			objektumok.push_back( new STATTEXT(185,10,SZIN(200,200,100),SZIN(20,10,5)," Maximum number of steps ") );
			objektumok.push_back( new SZAMBEALLITO(188,30,0,10000,SZIN(200,200,100),SZIN(20,10,5),1000,20) );
			objektumok.push_back( new ABLAK(390,333,250,57,0,515));
			//objektumok.push_back( new ABLAK(15,333,250,57,0,401)); TODO: Alkalmaz | Mégse
			kattintva=-1;
		};
	
		bool supdate(event ev, double X0, double Y0, KAMERA kamera);
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat, utólag.
		void getter(ostream& ki) const;
};

bool beallitasok::supdate(event ev, double X0, double Y0, KAMERA kamera)
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
					if (i<2){ // Listába kattintottak
						bool b = i;

						stringstream babu;

						objektumok[b]->getter(babu);
						objektumok[!b]->setter(babu); // Előző hozzáadása

						objektumok[b]->supdate(ev,x+X0,y+Y0,kamera);

						objektumok[b]->getter(babu);
						objektumok[!b]->setter(babu); // Mostani törlése a másikból

					}else // Nem a listába kattintottak
					{
						objektumok[i]->supdate(ev,x+X0,y+Y0,kamera);
					}

					return true;
				}
			}
			kattintva=-1;
		}
	}else if (ev.type==ev_key)
	{
		objektumok[4]->supdate(ev,x+X0,y+Y0,kamera); // Számbeállító átveszi a gombokat.
	}
	return false;
}

int miaszine(string s)
{
	if (s=="Red") return 0;
	else if (s=="Yellow") return 1;
	else if (s=="Green") return 2;
	else if (s=="Cyan") return 3;
	else if (s=="Blue") return 4;
	else if (s=="Magenta") return 5;

	return 0;
}

void beallitasok::getter(ostream& ki) const
{
	stringstream seged;
	objektumok[0]->getter(seged);
	string s;
	seged >> s;
	ki << kattintva << " " << miaszine(s) << " "; seged.str("");
	objektumok[1]->getter(seged);
	seged >> s;
	ki << miaszine(s) << " "; seged.str("");
	objektumok[4]->getter(seged);
	ki << seged.str() << " ";
}

void mainbeallitasok(ENV &env,Rekord &rekord)
{
	env.ObjKiemel(beal);
	beal->setPosition(25,100);

	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		if (env.ev.type==ev_mouse and env.ev.button==-btn_left)
		{
			stringstream ki;
			beal->getter(ki);
			string gomb;
			ki >> gomb;
			if (gomb=="5") { // Ha a vissza gomb lett megnyomva
				rekord.p[0].babu=8;
				rekord.p[1].babu=8;
				ki >> rekord.p[0].szin;
				ki >> rekord.p[1].szin;
				ki >> rekord.max_lepesszam;
				rekord.AI=false;
				break; // Kilépés
			}
		}
		env.UpdateDrawHandle();
	}
	beal->setPosition(999,999);
}

int initbeallitasok(ENV &env,Rekord &rekord)
{
	beal = new beallitasok(); env.addObj(beal);
	beal->setPosition(999,999);
	rekord.p[0].szin=0;
	rekord.p[1].szin=1;
	rekord.AI=false;
	rekord.max_lepesszam=1000;
	rekord.seed=time(0); cout << time(0) << endl;
}