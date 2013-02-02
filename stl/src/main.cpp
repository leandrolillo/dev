#include <iostream>
#include <list>

#include<map>

using namespace std;


#define String std::string
#define boolean unsigned char
#define null NULL

int main() {
	String *cadena = new String();
	cadena->assign("lalala");

	map<const unsigned int, String> *mapa = new map<const unsigned int, String>();
	mapa[1] = "lalala";

	delete mapa;
	delete cadena;


	list <char *> lista;

	lista.push_back(new char('-'));
	lista.push_back(new char('a'));
	lista.push_back(new char('b'));
	lista.push_back(new char('c'));
	lista.push_back(new char('d'));
	lista.push_back(new char('e'));
	lista.push_back(new char('f'));
	lista.push_back(new char('g'));
	lista.push_back(new char('h'));
	lista.push_back(new char('-'));

	char *pChar;

	list<char *>::iterator it = lista.begin(), temp;
	do {
		if(*(*it) == 'f') {
			temp = it;
			it++;

			pChar = *temp;
			delete pChar;

			lista.erase(temp);
		}
		else {
			cout << *(*it);
			it++;
		}
	} while(it != lista.end());

	cout << endl;

	for(it = lista.begin(); it != lista.end(); it++) {
		cout << *(*it);
	}
	

	/*for(list<char>::iterator it = lista.begin(); it != lista.end(); it++) {
		cout << *it;

		if(*it == 'f') {
			it++;
			lista.remove('f');
			continue;
		}
	}
	*/
	cout << endl;

	return 0;
}
