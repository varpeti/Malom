#include "env.hpp"
#include "szambeallito.hpp"
#include "kivalaszto.hpp"


#include "textbox.hpp"

using namespace genv;

OBJ *mm;

class FOMENU : public ABLAK
{
	private:
		int kattintva;
	public:

		FOMENU()
			: ABLAK(25,100,650,400,0,0,true)
		{	
			objektumok.push_back( new ABLAK(200,1*57,250,57,200,1*57,false) );
			objektumok.push_back( new ABLAK(200,3*57,250,57,200,3*57,false) );
			objektumok.push_back( new ABLAK(200,5*57,250,57,200,5*57,false) );
			kattintva=0;

		}

		bool supdate(event ev, double X0, double Y0, KAMERA kamera); 
		void addObj(OBJ *obj) {}; // Nem lehet hozzáadni újabb objektumokoat.
		void getter(ostream& ki) const;
};

bool FOMENU::supdate(event ev, double X0, double Y0, KAMERA kamera)
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

void FOMENU::getter(ostream& ki) const 
{
	ki << kattintva;
}

void initmenu(ENV &env)
{
	if(!env.kepek_beolvas("sp.bmp")) cout << "Nem talalhato a sp.bmp!";

	mm = new FOMENU(); env.addObj(mm);
}

int mainmenu(ENV &env)
{
	gin.timer(20);

	while(gin >> env.ev and env.ev.keycode!=key_escape) {

		if (env.ev.type==ev_timer){
			
		} 
		else if (env.ev.type==ev_key)
		{

		}
		else if (env.ev.type==ev_mouse)
		{
			if (env.ev.button==-btn_left) // Gomb felengedésnél nézzük meg hogy melyik lett lenyomva.
			{
				stringstream str;
				mm->getter(str);
				int kv;
				str >> kv;
				if (kv) return kv; // 1 Új játék // 2 infó // 3 kilépés
			}

		}

		env.UpdateDrawHandle();
	}

	return 3;

}