#include <stdlib.h>
#include <string.h>
#include <UCMD.h>
#include <stdio.h>

extern Command my_cmd_list;

//stores last used command and arguments

static char last_in[64];
char last_cmd_used;

//Commands

int led_on_cb(int argc, char* argv[]) //cmd for debugging 
{
	char* str = "Led turned on";
	int result = strcmp(argv[1],"on");
	if (result == 0)
	{
		//printmsg(str);
        printf("%s",str);
	}
	return 0;
}

//Extra Commands

int last_cmd_cb (int argc, char* argv[])
{
	if(strcmp(last_in,"") == 0)
		return UCMD_CMD_LAST_CMD_NOT_VALID;
	last_cmd_used = 1;
	return ucmd_parse(&my_cmd_list, UCMD_DEFAULT_DELIMETER, last_in);
}

int help_cb (int argc, char* argv[])
{
	for(Command* p = &my_cmd_list; p->fn != NULL; p++ )
		//printmsg((char*)p->help);
        printf("%c",(char*)p->help);
	return 0;
}

int version_cb (int argc, char* argv[])
{
	char* str = "VERSAO 1.1\r";
	//printmsg(str);
    printf("%s",str);
	return 0;
}


//---------------------------------------------------------//

///////////////////////
// Parsing functions //
///////////////////////

int ucmd_parse(Command cmd_list[], const char* delim, const char* in)
{
    if (!in || strlen(in) == 0) return 0;
    if (!cmd_list) return UCMD_CMD_NOT_FOUND;

    delim = (delim) ? delim : UCMD_DEFAULT_DELIMETER; //if delim is defined -> keep it, if not define as default

    int retval = 0;     //return value

    char* s = (char*)malloc(strlen(in) + 1);    //alocates and copies input string

	strcpy(s, in);
    
    int argc = 0;
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
            if (strcmp(p->cmd, argv[0]) == 0) {
                c = p;
				break;			//leaves if it has found cmd (avoids unncessary operations)
            }
			if(p->fn == NULL)	//failsafe condition to avoid anautorized memory access and cause a crash
				break;
        }
        if (c) {
            retval = c->fn(argc, argv);
			if(strcmp(c->cmd, "$" ) && last_cmd_used == 0 && retval == 0){
				//prevents infinite recursive loop where $ cmd calls itself
				strcpy(last_in, in);
			}			
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
void ucmd_error_handler(int error)
{
	if(error)
	{
		char* error_str;
		switch(error)
		{
			case UCMD_CMD_NOT_FOUND:
				error_str = "Command not found\r";
				break;
			
			case UCMD_NOT_ENOUGH_PARAMETERS:
				error_str = "Not enough parameters, check help for commands parameters\r";
				break;
			
			case UCMD_CMD_LIST_NOT_FOUND:
				error_str = "Command List not found\r";
				break;
			
			case UCMD_MEM_NOT_VALID:
				error_str = "Memory address not valid, check help for valid addresses\r";
				break;
			
			case UCMD_PORT_NOT_VALID:
				error_str = "GPIO port not valid, check help for valid ports\r";
				break;
			
			case UCMD_ADC_CHANNEL_NOT_VALID:
				error_str = "ADC channel not valid, check help for valid channels\r";
				break;
			
			case UCMD_CMD_LAST_CMD_LOOP:
				error_str = "Command unavailable, use another command\r";
				break;
			
			case UCMD_COUNTER_VALUE_NOT_VALID:
				error_str = "Sampling time not valid\r";
				break;
			
			case UCMD_PARAMETERS_NOT_VALID:
				error_str = "Parameters not valid, check help\r";
				break;
			case 1:
				error_str = "Undefined error\r";
				break;
		}
		//print error msg
		//printmsg(error_str);
	}
}