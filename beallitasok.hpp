#include "kivalaszto.hpp"
#include "szambeallito.hpp"

static OBJ *beal;

class beallitasok : public ABLAK// Egyepéldányos leszármazott osztály
{
	private:
		int kattintva;
	public:
		beallitasok()
				: ABLAK(25,100,650,400,0,0,true) // Fix az játékablakméret és a csak 1 példány miatt.
		{

			objektumok.push_back( new KIVALASZTO(15,30,SZIN(200,200,100),SZIN(20,10,5),{"Red","Green","Cyan","Blue","Magenta"},5) ); //70x149 (1 mező: 31)
			objektumok.push_back( new KIVALASZTO(100,30,SZIN(200,200,100),SZIN(20,10,5),{"Yellow","Green","Cyan","Blue","Magenta"},5) );
			objektumok.push_back( new STATTEXT(15,10,SZIN(200,200,100),SZIN(20,10,5),"   P1    |    P2   ") );
			objektumok.push_back( new STATTEXT(185,10,SZIN(200,200,100),SZIN(20,10,5)," Maximum number of steps ") );
			objektumok.push_back( new SZAMBEALLITO(188,30,0,9e99,SZIN(200,200,100),SZIN(20,10,5),1000,20) );
			objektumok.push_back( new ABLAK(390,333,250,57,0,515));
			kattintva=1;
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
		}
	}else if (ev.type==ev_key)
	{
		objektumok[4]->supdate(ev,x+X0,y+Y0,kamera); // Számbeállító átveszi a gombokat.
	}
	return false;
}

void beallitasok::getter(ostream& ki) const
{
	ki << kattintva;
}

int mainbeallitasok(ENV &env)
{
	gin.timer(20);

	stringstream gomb;
	while(gin >> env.ev and env.ev.keycode!=key_escape and gomb.str()!="5" ) {
		gomb.str("");
		beal->getter(gomb);
		env.UpdateDrawHandle();
	}

	return 3;
}

int initbeallitasok(ENV &env)
{
	beal = new beallitasok(); env.addObj(beal);
}