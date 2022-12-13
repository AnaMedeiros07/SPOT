#ifndef __CSERVER_H_
#define __CSERVER_H_

#include <string.h>
#include <iostream>
#include <UCMD.h>

using std::string;

extern Command my_cmd_list;

class CServer
{
    private:

    public:

    CServer(void);

    ~CServer(void);

    bool Updatelocalhouse(double, double, double, double);

    bool ProcessRequest(string);

    bool SendValuesOutOfRange(void); //definir o tipo de dados limits e trocar void

    bool SendImage(void); //trocar void pelo tipo da imagem

    void ConnectServer(void);

    void EndConnection(void);
};

#endif