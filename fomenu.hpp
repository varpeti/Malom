#include "menu.hpp"
#include "kivalaszto.hpp"

using namespace genv;

OBJ *mm;
OBJ *jm;

void initmenu(ENV &env)
{
	if(!env.kepek_beolvas("sp.bmp")) {cout << "Nem talalhato a sp.bmp!\n"; exit(1);}

	std::vector<OBJ*> objs;
	objs.push_back( new MENUPONT(250,57,200,1*57) );
	objs.push_back( new MENUPONT(250,57,200,3*57) );
	objs.push_back( new MENUPONT(250,57,200,5*57) );

	mm = new MENU(25,100,650,57,0,0,objs); env.addObj(mm);

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

int initbeallmenu()
{
	OBJ *ablak = new ABLAK(25,100,650,400,0,0,true);
}