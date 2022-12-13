#ifndef __UCMD_H_
#define __UCMD_H_

//-------------------//
//    UCMD module	 //
//-------------------//


/*

//----------------------------- How to use -----------------------------//

In cmd_list.cpp add custom commands with the structure
{
    "command"
    "command help tooltip"
    (Command_cb) 'function callback'
}
custom commands should always be declared before the null
command terminator

-----------------------------------------------------------------------------

To process commands put UcmdParse() where it is necessary

UcmdParse(Command_cb &my_cmd_list, char delimiter, string input)

my_cmd_list is defined in cmd_list.cpp

delimiter is declared to seperate the words in input string, by default
it is ""

input is a string with the command and arguments, this string should not
contain special caracters in the end

-----------------------------------------------------------------------------

To define errors add it below and add the corresponding action to take in the
UcmdErrorHandler function

*/


#include "limits.h"
#include "stdint.h"
#include <iostream>
#include <string.h>

using std::string;

////////////////////////
// Define UCMD errors //
////////////////////////

#define UCMD_CMD_NOT_FOUND 				INT_MIN		

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

int LedOnCb (int, char* []);

 
//Extra Commands

int LastCmdCb (int, char* []);

int HelpCb (int, char* []);

int VersionCb (int, char* []);

//---------------------------------------------------------//

//Parsing functions

int UcmdParse(Command [], const char*, const string in);

void UcmdErrorHandler(int);

string SanitizeString(string);

//---------------------------------------------------------//

#endif
