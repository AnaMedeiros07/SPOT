#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "UCMD.h"
#include <stdio.h>
#include <algorithm>

using std::string;

extern Command my_cmd_list;

//stores last used command and arguments

static char last_in[64];
char last_cmd_used;

//Commands

int LedOnCb(int argc, char* argv[]) //cmd for debugging 
{
	string str = "Led turned on";
	int result = strcmp(argv[1],"on");
	if (result == 0)
	{
        printf("%s",str.c_str());
	}
	return 0;
}

//Extra Commands

int HelpCb (int argc, char* argv[])
{
	for(Command* p = &my_cmd_list; p->fn != NULL; p++ )
		std::cout << (char*)p->help << "\n";
	return 0;
}

int VersionCb (int argc, char* argv[])
{
	string str = "VERSAO 1.1\r";
	std::cout << str << "\n";
	return 0;
}


//---------------------------------------------------------//

///////////////////////
// Parsing functions //
///////////////////////

//ucmd_parse takes strings that doesn't end on newline

int UcmdParse(Command cmd_list[], const char* delim, const string in)
{
    if (in.empty() || in.length() == 0) return 0;
    if (!cmd_list) return UCMD_CMD_NOT_FOUND;

    delim = (delim) ? delim : UCMD_DEFAULT_DELIMETER; //if delim is defined -> keep it, if not define as default

    int retval = 0;     //return value
	
    char* s = (char*)malloc(in.length() + 1);    //alocates and copies input string

	in.copy(s, in.length(), 0);
    
    long unsigned int argc = 0;
    char** argv = NULL;

    char* arg = strtok(s, delim);

    while (arg) {
        argc++;
        argv = (char**)realloc(argv, argc * sizeof(*argv));
        argv[argc - 1] = arg;
        arg = strtok(NULL, delim);
    }
		

    if (argc) {
        Command* c = NULL;
        for (Command* p = cmd_list; p->cmd; p++) {
			std::cout << argv[0] << " " << p <<"\n";
            if (strcmp(p->cmd, argv[0]) == 0) {
                c = p;
				break;			//leaves if it has found cmd (avoids unncessary operations)
            }
			if(p->fn == NULL){
				std::cout << " Dump 4 \n";
				break;
			}	//failsafe condition to avoid anautorized memory access and cause a crash
				
        }
        if (c) {
			std::cout << c->cmd << " Dump 3 \n";
            retval = c->fn(argc, argv);			
        }
        else {
            retval = UCMD_CMD_NOT_FOUND;
        }
    }
	last_cmd_used = 0;
    free(argv);
    free(s);

    return retval;
}

///////////////////////////////////////////////////
// Define actions/messages in response to errors //
///////////////////////////////////////////////////
void UcmdErrorHandler(int error)
{
	if(error)
	{
		string error_str;
		switch(error)
		{
			case UCMD_CMD_NOT_FOUND:
				error_str = "Command not found\r";
				break;
		}
	}
}

string SanitizeString(string str){
str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
return str;
}