#include "env.hpp"
#include "lua_adapter.hpp"
#include "fomenu.hpp"
#include "beallitasok.hpp"
#include "jatek.hpp"
#include "jatekrekord.h"
#include <iostream>


int main()
{
	LuaAdapter lua ("main.lua");
	
	ENV env (700,600,false); // Fix méretű ablak, fix koordináták és méretek.
	gin.timer(20);

	Rekord rekord;

	initmenu(env);
	initbeallitasok(env,rekord);

	int m=0;
	while ( (m=mainmenu(env)) != 3 )
	{
		if (m==1)
		{
			initjatek(env,rekord);
			mainjatek(env,rekord);
			endjatek(env,rekord);
		}else if(m==2)
		{
			mainbeallitasok(env,rekord);
		}
	}
	
	return 0;
}
