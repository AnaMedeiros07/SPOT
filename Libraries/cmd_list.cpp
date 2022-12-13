#include "UCMD.h"
#include "stdlib.h"

//if cmds stop working copy this to main.c and delete "&" in ucmd_parse()

Command my_cmd_list[] = {
    {
        "led",
        "LED || on\r",
        (Command_cb) LedOnCb,
    },
	{
        "?",
        "? || no arguments\r",
        (Command_cb) HelpCb,
    },
		{
        "VER",
        "VER || no arguments\r",
        (Command_cb) VersionCb,
    },		
    {
        "",
        "null list terminator",
        NULL,
    },
};