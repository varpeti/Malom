#include "env.hpp"
#include "lua_adapter.hpp"
#include "fomenu.hpp"
#include "jatek.hpp"
#include <iostream>


int main()
{
	srand(time(0));
	
	ENV env (700,600,false);

	initmenu(env);
	initjatek(env);

	int m=0;
	while ( (m=mainmenu(env)) != 3 )
	{
		cout << m << endl;
	}

	
	return 0;
}
