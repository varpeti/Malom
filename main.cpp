#include "env.hpp"
#include "lua_adapter.hpp"
#include "fomenu.hpp"
#include "beallitasok.hpp"
#include "jatek.hpp"
#include <iostream>


int main()
{
	srand(time(0));

	LuaAdapter lua ("main.lua");
	
	ENV env (700,600,false); // Fix méretű ablak, fix koordináták és méretek.

	initmenu(env);
	initbeallitasok(env);
	initjatek(env);

	int m=0;
	while ( (m=mainmenu(env)) != 3 )
	{
		if (m==1)
		{

		}else if(m==2)
		{
			mainbeallitasok(env);
		}
	}
	
	return 0;
}
