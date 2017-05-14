#include "jatekrekord.h"
#include "kijelzo.hpp"
#include "tabla.hpp"

#include "lua.hpp"
#include <thread>

static OBJ *tabla;
static OBJ *kijelzo;

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

void getszomszedok(Rekord &rekord,int hanyadik,vector<int> &szomszedok) // vissza adja azokat a mezőket amikre szabadon léphet
{
	if (rekord.palya[hanyadik]->szom.b and rekord.palya[hanyadik]->szom.b->szin==6) szomszedok.push_back(rekord.palya[hanyadik]->szom.b->id);
	if (rekord.palya[hanyadik]->szom.f and rekord.palya[hanyadik]->szom.f->szin==6) szomszedok.push_back(rekord.palya[hanyadik]->szom.f->id);
	if (rekord.palya[hanyadik]->szom.j and rekord.palya[hanyadik]->szom.j->szin==6) szomszedok.push_back(rekord.palya[hanyadik]->szom.j->id);
	if (rekord.palya[hanyadik]->szom.l and rekord.palya[hanyadik]->szom.l->szin==6) szomszedok.push_back(rekord.palya[hanyadik]->szom.l->id);
}

bool malome(Rekord &rekord,int hanyadik,int irany=0) // visszaadja hogy malomba rakták-e
{
	int szin = rekord.palya[hanyadik]->szin;

	if (rekord.palya[hanyadik]->szom.b and rekord.palya[hanyadik]->szom.j and (!irany or irany==1 or irany==3) and rekord.palya[hanyadik]->szom.b->szin==szin and rekord.palya[hanyadik]->szom.j->szin==szin) return true; // Ha mellete ugyan olyanok
	if (rekord.palya[hanyadik]->szom.f and rekord.palya[hanyadik]->szom.l and (!irany or irany==2 or irany==4) and rekord.palya[hanyadik]->szom.f->szin==szin and rekord.palya[hanyadik]->szom.l->szin==szin) return true; // Ha felette alatta ugyan olyanok

	// Ha nem rekurzívan elmozgunk de csak egy vonalban.
	if ((!irany or irany==1) and rekord.palya[hanyadik]->szom.b and rekord.palya[hanyadik]->szom.b->szin==szin) if ( malome(rekord,gethanyadikfromid(rekord,rekord.palya[hanyadik]->szom.b->id),1) ) return true;
	if ((!irany or irany==2) and rekord.palya[hanyadik]->szom.f and rekord.palya[hanyadik]->szom.f->szin==szin) if ( malome(rekord,gethanyadikfromid(rekord,rekord.palya[hanyadik]->szom.f->id),2) ) return true;
	if ((!irany or irany==3) and rekord.palya[hanyadik]->szom.j and rekord.palya[hanyadik]->szom.j->szin==szin) if ( malome(rekord,gethanyadikfromid(rekord,rekord.palya[hanyadik]->szom.j->id),3) ) return true;
	if ((!irany or irany==4) and rekord.palya[hanyadik]->szom.l and rekord.palya[hanyadik]->szom.l->szin==szin) if ( malome(rekord,gethanyadikfromid(rekord,rekord.palya[hanyadik]->szom.l->id),4) ) return true;
	return false; 
}

bool vanenemmalom(Rekord &rekord, int szin) // Vissza adja hogy van e olyan bábú ami nem malomban van adott szinben
{
	for (int i = 0; i < 49; ++i)
	{
		if (rekord.palya[i]->szin==szin and !malome(rekord,i)) return true;
	}
	return false;
}

bool tudelepni(Rekord &rekord, int szin)
{
	for (int i = 0; i < 49; ++i)
	{
		if (rekord.palya[i]->szin==szin) 
		{
			vector<int> szomszedok;
			getszomszedok(rekord,i,szomszedok);
			if (szomszedok.size()>0) return true;
		}
	}
	return false;
}

bool lepes(bool jatekos,ENV &env,Rekord &rekord)
{	
	rekord.lastmove.hova=-1; rekord.lastmove.honnan=-1; rekord.lastmove.leutotte=-1;

	// Ha kevesebb mint három bábuja van vagy (ha a kezébe 0 bábu és játékban nem tud lépni						úgy hogy 3-nál több bábuja van),		 veszít
	if (rekord.p[jatekos].lbabu<3 or (rekord.p[jatekos].babu==0 and !tudelepni(rekord,rekord.p[jatekos].szin and !rekord.p[jatekos].lbabu==3)) ) {rekord.nyertes=!jatekos+1; return false;} 
	
	{
		stringstream str;
		str << rekord.p[jatekos].szin << " "  << rekord.p[jatekos].babu; // szín, kézben lévő bábuk száma, győzelem.
		kijelzo->setter(str);
	}

	bool felszedve = false;
	bool uthet = false;
	vector<int> szomszedok;

	while(gin >> env.ev and env.ev.keycode!=key_escape) {
		env.UpdateDrawHandle();
		if (env.ev.type==ev_mouse)
		{
			if (env.ev.button==btn_left)
			{	
				stringstream gs,ss;
				tabla->getter(gs);

				int ijsz,id,hanyadik;

				gs >> id;
				if (id==-1) continue; // A pálya lényegtelen részére kattintottak

				hanyadik = gethanyadikfromid(rekord,id);
				ijsz = rekord.palya[hanyadik]->szin;

				//	 Üres   és	 van bábuja		   és (vagy nem szedte fel vagy szomszédba rakta le vagy ugrálhat) és nem utésben van
				if (ijsz==6 and rekord.p[jatekos].babu>0 and (!felszedve or szomszede(szomszedok,id) or rekord.p[jatekos].lbabu==3) and !uthet ) 
				{
					ss << id << " " <<rekord.p[jatekos].szin;
					tabla->setter(ss); // Megjelenítés;
					rekord.palya[hanyadik]->szin = rekord.p[jatekos].szin; // Játékmenet;
					rekord.p[jatekos].babu--;
					rekord.lastmove.hova=id;
					if (malome(rekord,hanyadik)) uthet=true;
					else 	return true; // Ha nem malomba lépett átadja a kört
				}
				//		Sajátját fogta meg		és még nem szedett fel és nem ütésben van  
				else if (ijsz==rekord.p[jatekos].szin and !felszedve and !uthet) // Bábu felszedése
				{
					getszomszedok(rekord,hanyadik,szomszedok);
					if (szomszedok.size()>0 or rekord.p[jatekos].lbabu==3){ // Ha csak 3 bábuja van nem kell hogy csak szomszédba léphessen
						ss << id << " " << 6;
						tabla->setter(ss); // Megjelenés;
						rekord.palya[hanyadik]->szin = 6; // Játékmenet;
						rekord.p[jatekos].babu++;
						rekord.lastmove.honnan=id;
						felszedve=true;
					}
				}
				// Az ellenség bábuját fogta meg, és ütésben van
				else if (uthet and ijsz==rekord.p[!jatekos].szin)
				{
					// Nincs malomban vagy minden malomban van
					if (!malome(rekord,hanyadik) or !vanenemmalom(rekord,rekord.p[!jatekos].szin) )
					{
						ss << id << " " << 6;
						tabla->setter(ss); // Megjelenés;
						rekord.palya[hanyadik]->szin = 6; // Játékmenet;
						rekord.p[!jatekos].lbabu--; // Azt számolja mennyi bábuja van még játékban
						rekord.lastmove.leutotte=id;
					return true; // Ütés végével vége a körnek.
					}
				}
			}
			cout << "a: " << felszedve << uthet << endl;
		}
	}
	return false; // Kilép a menübe
}

void LuaThreadAI(bool *futhat,bool *lephet,Rekord *rekord) // Külön szál ami a a kommunikációs csatorna a lua és a c++ között
{
	lua_State* L = luaL_newstate();
	luaL_loadfile(L, "main.lua");
	luaL_openlibs(L);
	lua_pcall(L, 0, 0, 0);

	lua_newtable(L); //palya
		for (int i = 0; i < rekord->palya.size(); ++i)
		{
			if (rekord->palya[i]->szin!=9)
			{
				lua_newtable(L); //mezo
					lua_pushnumber(L,rekord->palya[i]->id+1); 
					lua_setfield(L, -2, "id");

					lua_pushnumber(L,rekord->palya[i]->szin); 
					lua_setfield(L, -2, "szin");

					if (rekord->palya[i]->szom.b) lua_pushnumber(L,rekord->palya[i]->szom.b->id+1); 
						else lua_pushnumber(L,0);
					lua_setfield(L,-2,"b");
					if (rekord->palya[i]->szom.f) lua_pushnumber(L,rekord->palya[i]->szom.f->id+1); 
						else lua_pushnumber(L,0);
					lua_setfield(L,-2,"f");
					if (rekord->palya[i]->szom.j) lua_pushnumber(L,rekord->palya[i]->szom.j->id+1); 
						else lua_pushnumber(L,0);
					lua_setfield(L,-2,"j");
					if (rekord->palya[i]->szom.l) lua_pushnumber(L,rekord->palya[i]->szom.l->id+1); 
						else lua_pushnumber(L,0);
					lua_setfield(L,-2,"l");
				lua_rawseti(L, -2, rekord->palya[i]->id+1);
			}
		}
	lua_setglobal(L, "palya");

	lua_pushnumber(L,rekord->p[1].babu);
	lua_setglobal(L, "db");

	while(*futhat)
	{
		if (*lephet) {

			// Ha veszít																													// Az emberi játékos nyer
			if (rekord->p[1].lbabu<3 or (rekord->p[1].babu==0 and !tudelepni(*rekord,rekord->p[1].szin and !rekord->p[1].lbabu==3)) ) {rekord->nyertes=1; *lephet=false; return;} 
			
			lua_pushnumber(L, rekord->lastmove.hova);
			lua_setglobal(L,"hova");
			lua_pushnumber(L, rekord->lastmove.honnan);
			lua_setglobal(L,"honnan");
			lua_pushnumber(L, rekord->lastmove.leutotte);
			lua_setglobal(L,"leutotte");

			lua_getglobal(L,"lephet");
			if(lua_isfunction(L, -1) )
			{
				lua_pcall(L,0,0,0); // Átadom mit lépet a játékos, és visszakapom hogy az AI mit lép

				lua_getglobal(L,"hova");
				rekord->lastmove.hova = lua_tonumber(L,-1);
				lua_getglobal(L,"honnan");
				rekord->lastmove.honnan = lua_tonumber(L,-1);
				lua_getglobal(L,"leutotte");
				rekord->lastmove.leutotte = lua_tonumber(L,-1);
				
				if (rekord->lastmove.hova>-1) {
					stringstream ss;
					ss << rekord->lastmove.hova << " " << rekord->p[1].szin;
					tabla->setter(ss);
					rekord->palya[gethanyadikfromid(*rekord,rekord->lastmove.hova)]->szin=rekord->p[1].szin;
					rekord->p[1].babu--;
				}
				if (rekord->lastmove.honnan>-1) {
					stringstream ss;
					ss << rekord->lastmove.honnan << " " << 6;
					tabla->setter(ss);
					rekord->palya[gethanyadikfromid(*rekord,rekord->lastmove.honnan)]->szin=6; 
					rekord->p[1].babu++;
				}
				if (rekord->lastmove.leutotte>-1) {
					stringstream ss;
					ss << rekord->lastmove.leutotte << " " << 6;
					tabla->setter(ss);
					rekord->palya[gethanyadikfromid(*rekord,rekord->lastmove.leutotte)]->szin=6; 
					rekord->p[0].lbabu--;
				}

			}else cout << "FV" << endl;

			*lephet=false;
		}else //!*lephet
		{
			lua_getglobal(L,"szamol");
			if(lua_isfunction(L, -1) )
			{
				lua_pcall(L,0,0,0);
			}
		}
	} // futhat
}


void mainjatek(ENV &env,Rekord &rekord)
{	
	env.ObjKiemel(tabla);
	tabla->setPosition(119,69);
	kijelzo->setPosition(334,18);

	thread *LuaAI=NULL;
	bool futhat=true;
	bool lephet=false;

	if (rekord.AI) { // Ha van AI meghívom külön threaden a Lua kódot
		LuaAI = new thread(LuaThreadAI,&futhat,&lephet,&rekord);
	}

	while(gin >> env.ev and env.ev.keycode!=key_escape and rekord.nyertes==0) { // A játék
		if (!lepes(0,env,rekord)) break;
		if (!rekord.AI) {if (!lepes(1,env,rekord)) break;} // 2. Játékos lép
		else {lephet=true; while(lephet);} // Megvárja az AI lépését
	}

	tabla->setPosition(999,999);

	if (rekord.nyertes) {
		OBJ * victory = new ABLAK(225,272,250,57,250,401,true); env.addObj(victory);
		stringstream str;
		str << rekord.p[rekord.nyertes-1].szin << " " << 0; // szín, 0
		kijelzo->setter(str);
		kijelzo->setPosition(420,284);
		env.ObjKiemel(kijelzo);
		while(gin >> env.ev and env.ev.type!=ev_key) {env.UpdateDrawHandle();}
		env.delObj(victory);
	}

	if (LuaAI) // Ha van LuaAI thread, akkor terminálom
	{
		futhat=false;
		if (LuaAI->joinable()) LuaAI->join();
		delete LuaAI;
	}

}

bool okxy(vector<Mezo*> m,int xx,int yy) // 3-nál kevesebb van ebben a sorban vagy oszlopban
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

	if (!rekord.Classic){
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
	
		// BUG: Létezhet olyan szituáció hogy 1 izolált pont van a pályán (nagyon kicsi esély), és ez az izolált pont az első sorban van (Hihetetlenül kicsi esély).
		// Ekkor a program nem vonalaz, nem jelöli be a szomszédokat.
		// Ezzel a sorral lehet megoldani a problémát, csak az esetek 100-Epszilon százalékában felesleges.
		vonalaz(rekord.palya,0,1);

		rekord.p[0].babu=7;
		rekord.p[1].babu=7;

	}else // Classikus 24 mezős pálya
	{	//	  0123456
		//  0 0--0--0
		//  1 |0-0-0|
		//  2 ||000||
		//  3 000 000
		//  4 ||000||
		//  5 |0-0-0|
		//  6 0--0--0

		for (int i = 0; i < 49;) // Ez a cuki szabály írja le a pályát :D
		{
			rekord.palya[i]->szin=6;
			if ( i==(5+1*7) or i==(4+4*7) ) i+=4;
			else if ( i<(6+0*7) or i==(4+2*7) or i==(6+3*7) or i>=(0+6*7) ) i+=3;
			else if ( i<(5+1*7) or i==(2+3*7) or (i>=(1+5*7) and i<(0+6*7)) ) i+=2;
			else i+=1;
		}

		vonalaz(rekord.palya,0,0);

		rekord.palya[3+2*7]->szom.l=NULL; // Nem kellenek ezek a szomszédok.
		rekord.palya[2+3*7]->szom.j=NULL;
		rekord.palya[4+3*7]->szom.b=NULL;
		rekord.palya[3+4*7]->szom.f=NULL;


		rekord.p[0].babu=9;
		rekord.p[1].babu=9;
	}

	rekord.p[0].lbabu=rekord.p[0].babu;
	rekord.p[1].lbabu=rekord.p[1].babu;

	rekord.nyertes=0;

	tabla = new TABLA(rekord.palya); env.addObj(tabla);
	tabla->setPosition(999,999);

	kijelzo = new KIJELZO(10,10,6); env.addObj(kijelzo);
	kijelzo->setPosition(999,999);
}

void endjatek(ENV &env, Rekord &rekord)
{
	env.delObj(tabla);
	env.delObj(kijelzo);

	while (rekord.palya.size()>0)
	{
		delete rekord.palya[rekord.palya.size()-1];
		rekord.palya.pop_back();
	}

}
