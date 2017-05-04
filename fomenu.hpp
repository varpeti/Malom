#include "menu.hpp"

static OBJ *mm;

void initmenu(ENV &env)
{
	if(!env.kepek_beolvas("sp.bmp")) {cout << "Nem talalhato a sp.bmp!\n"; exit(1);}

	std::vector<OBJ*> objs;
	objs.push_back( new MENUPONT(250,57,0,401) );
	objs.push_back( new MENUPONT(250,57,0,458) );
	objs.push_back( new MENUPONT(250,57,0,515) );

	mm = new MENU(25,100,650,57,0,0,objs); env.addObj(mm);

}

int mainmenu(ENV &env)
{
	env.ObjKiemel(mm);
	mm->setPosition(25,100);

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
				if (kv) {mm->setPosition(999,999); return kv;} // 1 Új játék // 2 infó // 3 kilépés
			}

		}

		env.UpdateDrawHandle();
	}

	return 3;
}