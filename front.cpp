#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "lib/cliser.h"

using namespace std;

const int numSlaves = 3;
const long long int buff_size = 1000;
vector<int> slaveSockets;
vector<string> queries;

string stringFillSize(string query){
	int size = query.size();
	string ssize = to_string(size);
	while(ssize.size()<3)ssize.insert(0,"0");
	query.insert(0, ssize);
	return query;
}

int hash(string buff){
	int sum=0;
	for(unsigned int i=0;i<buff.size();++i){
		sum += (int)buff[i];
	}
	return sum%numSlaves;
}

void readMessages(int Socket){
	char buffer[buff_size];
	string buf;

	bzero(buffer,buff_size);
	read(Socket,buffer,5);
	buf = buffer;

	if(buf.substr(0,2) == "OK") 
		cout<< "consulta numero " << buf.substr(2) << "(" << queries[atoi(buf.substr(2).c_str())] << ") ejecutada exitosamente." << endl;
}

int main(void)
{
	int n;
	char buffer[buff_size];
	string query, inputQuery, typeQuery, aux, name_node1, name_node2;
	slaveSockets.push_back(createClient("192.168.122.1", 1100));//0
	//slaveSockets.push_back(createClient("192.168.122.1", 1101));//1
	//slaveSockets.push_back(createClient("192.168.122.1", 1102));//2

	//thread por cada slave
	for(int i=0; i<slaveSockets.size(); i++)
		thread(readMessages, slaveSockets[i]).detach();

	while(true){
		cout << "gb:-$ ";
		getline(cin, inputQuery);
		if(inputQuery == "exit") break;
		//el id o codigo de la query es la posicion en el vector queries
		query = to_string(queries.size());
		while(query.size()<3)query.insert(0,"0");

		stringstream ss(inputQuery);
		ss>>aux; typeQuery=aux;//I, U, D, Q
		ss>>aux; typeQuery+=aux;//n, r
		query += typeQuery;

		ss>>name_node1; 
		name_node1 = stringFillSize(name_node1);//001 saffs (001 es el tamano de la query que enviara y luego viene la query)
		query+=name_node1;

		if(!ss.eof()){//el cuarto parametro es opcional
			ss>>name_node2; 
			name_node2 = stringFillSize(name_node2);
			query+=name_node2;
		}
		cout << query << endl;
		queries.push_back(query);

		//---------------enviar la consulta proocesada a cada slave que le corresponda
		write( slaveSockets[0], query.c_str() , query.size());//esta linea es de prueba, cuando este todo listo se descomentan las de abajo que trabaja con redundancia y todos los slaves activos



		/*
		write( slaveSockets[::hash(name_node1)], query.c_str() , query.size());
		write( slaveSockets[::hash(name_node1)+1], query.c_str() , query.size());//redundancia	

		if(query.substr(0,2) == "Ir" or query.substr(0,2) == "Dr"){	//preguntar si se tiene que enviar a dos esclavos o solo a uno
			write( slaveSockets[::hash(name_node2)], query.c_str() , query.size());
			write( slaveSockets[::hash(name_node2)+1], query.c_str() , query.size());//redundancia	
		}
		*/
	} 

	for(int i=0; i<slaveSockets.size(); i++){
		shutdown(slaveSockets[i], SHUT_RDWR);
		close(slaveSockets[i]);
	}
	return 0;
}



