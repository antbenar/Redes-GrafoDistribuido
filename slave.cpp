#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include <map> 
#include <iterator> 
#include "lib/cliser.h"

using namespace std;

multimap <string, string> grafo;
const long long int buff_size = 1000;

void leer_de(int SocketFD){
    char buffer[1000];
    string buf, codigo;

    while(true){
        bzero(buffer,1000);
        read(SocketFD, buffer, 3);
        codigo = buffer;

        cout << "-Procesando consulta " << codigo << endl;

        bzero(buffer,1000);
        read(SocketFD, buffer, 2);
        buf = buffer;

        if(buf == "In" ){  //Insert node
            bzero(buffer,1000);
            read(SocketFD, buffer, 3);
            int sizeMessage = atoi(buffer);
            bzero(buffer,1000);
            read(SocketFD, buffer, sizeMessage);
            string node1 = buffer;

            grafo.insert({node1, ""});
            
            cout << "--Consulta " << codigo << " procesada en In" << endl;
            write(SocketFD,("OK"+codigo).c_str(),5);// OK001, se insertó exitosamente
        }
        else if(buf == "Ir" ){  //Insert node

        }
        else {
            cout << "--No se pudo procesar la consulta " << codigo << endl;
            write(SocketFD,("NO"+codigo).c_str(),5);// NO001, hubo algun error en la escritura del mensaje
        }
    }
}

int main(void)
{
    int port = 1100;
    /*
    cout << "ingrese el numero de puerto del slave: ";
    cin >> port;
    */
	int Socket = createServer(port);
    for(;;){
        int ConnectFD = accept(Socket, NULL, NULL);
        if(ConnectFD < 0)continue;
        cout << "front conectado a Slave" << endl;
        thread(leer_de, ConnectFD).detach();
    }
    shutdown(Socket,SHUT_RDWR);
    close(Socket);
    return 0;
}
