#include "zmake.h"
#include <stdio.h>
#include <string.h>
#include "zcmake.h"

Cir_Debug_Module_Define(main);

#include "zmfile.h"

using namespace zm;
int main (int argc, char *argv[])
{
	CDBG_Init();
	CENTER();
	zmake zm(argc, argv);

	if (zm.doOpts() == 0)
	{
		zm.doConf();
	}

	CLEAVE();
	CDBG_Fini();
	return 0;
}

