#ifndef __UCMD_H_
#define __UCMD_H_

//--------------------------------------//
// UCMD module for STM32F767						//
// Use in conjuction with UART3 module	//
//--------------------------------------//

#include "limits.h"
#include "stdint.h"

////////////////////////
// Define UCMD errors //
////////////////////////

#define UCMD_CMD_NOT_FOUND 				INT_MIN			
	
#define UCMD_NOT_ENOUGH_PARAMETERS 		INT_MIN+1

#define UCMD_CMD_LIST_NOT_FOUND 		INT_MIN+2

#define UCMD_MEM_NOT_VALID				INT_MIN+3

#define UCMD_PORT_NOT_VALID				INT_MIN+4

#define UCMD_ADC_CHANNEL_NOT_VALID 		INT_MIN+5

#define UCMD_CMD_LAST_CMD_LOOP			INT_MIN+6

#define UCMD_CMD_LAST_CMD_NOT_VALID 	INT_MIN+7

#define UCMD_COUNTER_VALUE_NOT_VALID	INT_MIN+8

#define UCMD_PARAMETERS_NOT_VALID		INT_MIN+9

//---------------------------------------------------------//

#define UCMD_DEFAULT_DELIMETER " "

typedef int (*Command_cb)(int, char* []);

typedef struct Command {
		const char *cmd;
		const char *help;
		Command_cb fn;
}Command;

////////////////////////////
// Declare Commands Below //
////////////////////////////

//Test Command

int led_on_cb (int, char* []);

 
//Extra Commands

int last_cmd_cb (int, char* []);

int help_cb (int, char* []);

int version_cb (int, char* []);

//---------------------------------------------------------//

//Parsing functions
int ucmd_parse(Command [], const char*, const char* in);

void ucmd_error_handler(int);

//---------------------------------------------------------//

Command my_cmd_list[] = {
    {
        "led",
        "LED || on\r",
        (Command_cb) led_on_cb,
    },
		//Extra Commands
		{
        "$",
        "$ || no arguments\r",
        (Command_cb) last_cmd_cb,
    },
		{
        "?",
        "? || no arguments\r",
        (Command_cb) help_cb,
    },
		{
        "VER",
        "VER || no arguments\r",
        (Command_cb) version_cb,
    },		
    {
        "",
        "null list terminator",
        NULL,
    },
};

#endif
