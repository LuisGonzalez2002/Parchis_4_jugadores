//Librerías necesarias
#include <iostream> 
#include<time.h>
#include <stdlib.h> 
#include <stdio.h>
#include <Windows.h>
#include <string>

using namespace std;

//Constantes
const int n_jugadores = 4;
const int n_fichas = 4;
const int n_casillas = 68;

//Tipos
typedef enum {Amarillo, Azul, Rojo, Verde, Gris, Ninguno} tColor; //Diferentes colores a elegir
typedef tColor tColor1[4]; //Tipo tabla de colores
typedef int tFichas[n_fichas]; //Tipo tabla de posiciones
typedef int tZona[12]; //Tipo tabla de zonas seguras 
typedef tColor tCasillas[n_casillas];
typedef tFichas tJugadores[n_jugadores];


///////////////////// prototipos de funciones
tColor indiceAColor(int p_ind);
int colorAIndice(tColor p_color);
string colorACadena(tColor p_color);
char colorAletra(tColor p_color);
int colorAsalida(tColor p_color);
int colorAzanata(tColor p_color);
bool zonasegura(int p_casilla);
int tirardado2();
void carga_jugador(tJugadores jugadores, tColor jugador_turno, tFichas& jugador);
void iniciar(tJugadores jugadores, tCasillas calle1, tCasillas calle2, tColor& jugador_turno);
bool puente(const tCasillas calle1, const tCasillas calle2, int casilla);
int cuantasEn(const tFichas jugador, int casilla);
int primeraEn(const tFichas jugador, int casilla);
int segundaEn(const tFichas jugador, int casilla);
void saleFicha(tJugadores jugadores, tColor jugador_turno, tCasillas calle1, tCasillas calle2);
void aCasita(tJugadores jugadores, int casilla, tCasillas calle2);
bool todasEnMeta(const tFichas jugador);
bool hemoscomido2(tJugadores jugadores, int casilla, tColor jugador_turno, tCasillas calle1, tCasillas calle2);
void mandaracasa2(tJugadores jugadores, int casilla, tCasillas calle1);
void abrirPuente(tJugadores jugadores, int casilla, int casilla2, int& premio, tColor jugador_turno, int& ultimaFichaMovida, tCasillas calle1, tCasillas calle2);
bool procesa5(tJugadores& jugadores, tColor jugador_turno, int& premio, bool& pasaturno, tCasillas& calle1, tCasillas& calle2);
bool puedeMover(const tJugadores jugadores, tColor jugador_turno, int ficha, int& casilla, int tirada, tCasillas calle1, tCasillas calle2);
void mover(tJugadores jugadores, tColor jugador_turno, int ficha, int casilla, int& premio, int& ultimaFichaMovida, tCasillas calle1, tCasillas calle2);
bool procesa6(tJugadores jugadores, tColor jugador_turno, int& premio, bool& pasa_turno, int& seises, int& ultimaFichaMovida, int& tirada, tCasillas calle1, tCasillas calle2);
bool jugar(tJugadores jugadores, tColor jugador_turno, int& premio, bool& fin, int& seises, int& ultimaFichaMovida, int tirada, tCasillas calle1, tCasillas calle2);
void mostrar(const tJugadores jugadores, const tCasillas calle1, const tCasillas calle2);
void setColor(tColor color);
void posicionar();
void pinta_calles(const tJugadores jugadores, const tCasillas calle1, const tCasillas calle2);

///////////////////// main
int main() {


	int tirada;
	int color0, color1, color2, color3;
	int num_seis;
	int fich_seis;
	int comido;
	int turno;
	int ficha;
	int salida;		
	bool como;	
	bool fin;
	bool saca5;
	bool proc6;
	bool pasa_turno;
	int premio;
	int seises;
	int ultimaFichaMovida;
	int cuantas_casa;
	bool siguiente;
	tColor1 color;
	tFichas pos;
	int aux_int;
	int aux_int2;

	tJugadores jugadores;
	tFichas jugador;
	tColor jugador_turno;
	tCasillas calle1;
	tCasillas calle2;


	//Inicializaciones
	fin = false;
	iniciar(jugadores, calle1, calle2, jugador_turno);
	premio = 0;
	seises = 0;
	ultimaFichaMovida = 0;

	do {

		posicionar();

		pasa_turno = false;
		saca5 = false;
		proc6 = false;
		siguiente = true;

		// tiramos el dado
		if (premio == 0) { tirada = tirardado2(); }
		else { tirada = premio; }
		cout << "Turno para el jugador " << colorACadena(jugador_turno) << "... Sale un " << tirada << endl;

		// si la tirada es distinta de 5 o 6 y todas las fichas están en casa se informa de que no se puede mover
		if (tirada != 5 && tirada != 6) {
			carga_jugador(jugadores, jugador_turno, jugador);
			cuantas_casa = cuantasEn(jugador, -1);
			if (cuantas_casa == 4) {
				pasa_turno = true;
				cout << " ************ ¡Atencion! No puede mover. Pasa el truno.  ************" << endl;
			}
		}
		//Controlar 5 y 6
		if (pasa_turno == false) {
			if (tirada == 5) {
				saca5 = procesa5(jugadores, jugador_turno, premio, pasa_turno, calle1, calle2);
			}
			else if (tirada == 6) {
				proc6 = procesa6(jugadores, jugador_turno, premio, pasa_turno, seises, ultimaFichaMovida, tirada, calle1, calle2);
			}
			if ((proc6 == false && (tirada == 6 || tirada == 7)/*&& pasa_turno == false*/) ||
				(saca5 == true && pasa_turno == false && premio != 0 && tirada == 5) || 
				(tirada != 5 && tirada != 6 && tirada != 7)) {
				siguiente = jugar(jugadores, jugador_turno, premio, fin, seises, ultimaFichaMovida, tirada, calle1, calle2);
				if (premio == 0 && seises == 0) { pasa_turno = true; }
			}
			else {
				if (proc6 == true && pasa_turno == false) { siguiente = false; }
			}
		}

		mostrar(jugadores, calle1, calle2);
		//pinta_calles(jugadores, calle1, calle2);

		//pasar de turno si es necesario
		if (siguiente == true) {
			aux_int = colorAIndice(jugador_turno);
			aux_int2 = (aux_int+1) % 4;
			jugador_turno = indiceAColor(aux_int2);
			seises = 0;
		}

		system("PAUSE");

	} while (fin == false); 

	cout << "--------------- Ha ganado el color " << colorACadena(jugador_turno) << " ----------------" << endl;

	system("PAUSE");

	return 0;
}

///////////////////// definicion de funciones
tColor indiceAColor(int p_ind)
{
	tColor color = Ninguno;

	if (p_ind == 0) { color = Amarillo; }   //Si se elige el número 1, se le asigna el color amarillo
	else if (p_ind == 1) { color = Azul; }  //Si se elige el número 2, se le asigna el color azul
	else if (p_ind == 2) { color = Rojo; }  //Si se elige el número 3, se le asigna el color rojo
	else if (p_ind == 3) { color = Verde; } //Si se elige el número 4, se le asigna el color verde

	return color;
}

int colorAIndice(tColor p_color)
{
	int indice = -1;

	if (p_color == Amarillo) { indice = 0; }   //Si se elige el número 1, se le asigna el color amarillo
	else if (p_color == Azul) { indice = 1; }  //Si se elige el número 2, se le asigna el color azul
	else if(p_color == Rojo) { indice = 2; }  //Si se elige el número 3, se le asigna el color rojo
	else if (p_color == Verde) { indice = 3; } //Si se elige el número 4, se le asigna el color verde

	return indice;
}

string colorACadena(tColor p_color)
//Convierte un color a una cadena para poder imprimirlo
{
	string cadenacolor = "";
	
	if (p_color == Rojo) { cadenacolor = "Rojo"; }              
	else if (p_color == Verde) { cadenacolor = "Verde"; }       
	else if (p_color == Amarillo) { cadenacolor = "Amarillo"; } 
	else if (p_color == Azul) { cadenacolor = "Azul"; }         

	return cadenacolor;
}
char colorAletra(tColor p_color)
{
	char letra = ' ';

	if (p_color == Rojo) { letra = 'R'; }              //Si el color es el rojo en el tablero aparecera la R
	else if (p_color == Verde) { letra = 'V'; }        //Si el color es el rojo en el tablero aparecera la V
	else if (p_color == Amarillo) { letra = 'A'; }     //Si el color es el rojo en el tablero aparecera la A
	else if (p_color == Azul) { letra = 'X'; }         //Si el color es el rojo en el tablero aparecera la X

	return letra;
}
int colorAsalida(tColor p_color)
{
	int salida = 0;

	if (p_color == Amarillo) { salida = 5; }     //Indicador de que la salida del amarillo esta en la posición 5
	else if (p_color == Azul) { salida = 22; }   //Indicador de que la salida del azul está en la posción 22
	else if (p_color == Rojo) { salida = 39; }   //Indicador de que la salida del rojo está en la posición 39
	else if (p_color == Verde) { salida = 56; }  //Indicador de que la salida del verde está en la posición 56

	return salida;
}

int colorAzanata(tColor p_color)
{
	int salida = 0;

	if (p_color == Azul) { salida = 17; }          //Indicador de que la zanata del azul está en la posición 17
	else if (p_color == Rojo) { salida = 34; }     //Indicador de que la zanata del rojo estáa en la posición 34
	else if (p_color == Verde) { salida = 51; }    //Indicador de que la zanata del verde está en la posición 51
	else if (p_color == Amarillo) { salida = 0; }  //Indicador de que la zanata del amarillo está en la posción 0

	return salida;
}

bool zonasegura(int p_casilla) 
{
	// variables locales
	bool segur = false;
	int i = 0;
	tZona zonaseg;

	// Zonas seguras en las que no se puede comer
	
	zonaseg[0] = 0; zonaseg[1] = 5; zonaseg[2] = 12; zonaseg[3] = 22; zonaseg[4] = 29; zonaseg[5] = 34; zonaseg[6] = 39; 
	zonaseg[7] = 46; zonaseg[8] = 51; zonaseg[9] = 56; zonaseg[10] = 63, zonaseg[11] = 17;

	do
	{
		if (p_casilla == zonaseg[i]) {
			segur = true;
		}
		i = i + 1;

	} while (i < 12);

	return segur;

}

int tirardado2()
//Calcula una tirada aleatoria y devuelve su valor
{
	int num;

	srand(time(NULL));
	//num = (rand() % 6) + 1;
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
	cin >> num;
	cout << endl;

	return num;
}

void carga_jugador(tJugadores jugadores, tColor jugador_turno,  tFichas& jugador) {

	for (int i = 0; i < n_fichas; i++) {
		jugador[i] = jugadores[colorAIndice(jugador_turno)][i];
	}
}
void iniciar(tJugadores jugadores, tCasillas calle1, tCasillas calle2, tColor& jugador_turno) {
	
	int turno;

	srand(time(NULL));

	//Inicializamos fichas
	for (int i = 0; i < n_jugadores; i++) {
		for (int j = 0; j < n_fichas; j++) {
			jugadores[i][j] = -1;
		}
	}

	//Inicializamos tablero
	for (int i = 0; i < n_casillas; i++) {
		calle1[i] = Ninguno;
		calle2[i] = Ninguno;
	}

	//turno = (rand() % 4);
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><";
	cin >> turno;
	cout << endl;
	jugador_turno = indiceAColor(turno);

}

bool puente(const tCasillas calle1, const tCasillas calle2, int casilla) {

	bool aux;

	if (casilla < 0 || casilla > 67) { aux = false; }
	else {
		if (calle1[casilla] != Ninguno && calle2[casilla] == calle1[casilla]) { aux = true; }
		else { aux = false; }
	}

	return aux;
}

int cuantasEn(const tFichas jugador, int casilla) {

	int count = 0;

	for (int i = 0; i < n_fichas; i++) {
		if (jugador[i] == casilla) { count++; }
	}

	return count;
}
int primeraEn(const tFichas jugador, int casilla) {

	int primera = -1;

	for (int i = 0; i < n_fichas; i++) {
		if (jugador[i] == casilla && primera == -1) { primera = i; }
	}

	return primera;

}

int segundaEn(const tFichas jugador, int casilla) {

	int segunda = -1;

	for (int i = 0; i < n_fichas; i++) {
		if (jugador[i] == casilla) { segunda = i; }
	}

	return segunda;

}

void saleFicha(tJugadores jugadores, tColor jugador_turno, tCasillas calle1, tCasillas calle2) { 
	//Funcion para sacar la ficha de casa.

	tFichas jugador;
	int casilla;
	int ficha;

	for (int i = 0; i < n_fichas; i++) {
		jugador[i] = jugadores[colorAIndice(jugador_turno)][i];
	}

	casilla = colorAsalida(jugador_turno);
	ficha = primeraEn(jugador, -1);
	jugadores[colorAIndice(jugador_turno)][ficha] = casilla;

	//if (calle1[casilla] == Ninguno) {
	//	calle1[casilla] = jugador_turno;
	//}
	//else {
	//	calle2[casilla] = jugador_turno;
	//}
	calle2[casilla] = calle1[casilla];
	calle1[casilla] = jugador_turno;

}

void aCasita(tJugadores jugadores, int casilla, tCasillas calle2) {

	tFichas jugador;
	tColor colorcomido;
	int indcomido;
	int fichacomida;
	//Limpiar el tablero
	colorcomido = calle2[casilla];
	indcomido = colorAIndice(colorcomido);
	calle2[casilla] = Ninguno;

	for (int i = 0; i < n_fichas; i++) {
		jugador[i] = jugadores[indcomido][i];
	}
	//Mover a casa
	fichacomida = segundaEn(jugador, casilla);
	jugadores[indcomido][fichacomida] = -1;

}

bool todasEnMeta(const tFichas jugador) {

	bool fin = true;

	for (int i = 0; i < n_fichas; i++) {
		if (jugador[i] != 108) { fin = false; }
	}
	return fin;
}

bool hemoscomido2(tJugadores jugadores, int casilla, tColor jugador_turno, tCasillas calle1, tCasillas calle2)
//Controla si ha comido despues de mover la ficha
//Devuelve falso si no ha comido
{
	bool comido = false;

	if (zonasegura(casilla) == false && calle2[casilla] == Ninguno && calle1[casilla] != Ninguno) { comido = true; }

	return comido;
}

void mandaracasa2(tJugadores jugadores, int casilla, tCasillas calle1){

	int indice;
	tFichas jugador;
    
	indice = colorAIndice(calle1[casilla]);
	calle1[casilla] = Ninguno;
	
	for (int i = 0; i < n_fichas; i++) {
		jugador[i] = jugadores[indice][i];
	}
	
	jugadores[indice][segundaEn(jugador, casilla)] = -1;

}
void abrirPuente(tJugadores jugadores, int casilla, int casilla2, int& premio, tColor jugador_turno, int& ultimaFichaMovida, tCasillas calle1, tCasillas calle2){

	tFichas jugador;
	int ficha;

	carga_jugador(jugadores, jugador_turno, jugador);

	// localizar la ficha a mover
	ficha = segundaEn(jugador, casilla);
	// informar al usuario
	cout << " ************ ¡Atencion! Ha abierto en puente de la casilla "<< casilla <<".  ************" << endl;
	// movemos la ficha
	mover(jugadores, jugador_turno, ficha, casilla2, premio, ultimaFichaMovida, calle1, calle2);
	// registramos ultima ficha movida
	ultimaFichaMovida = ficha;

}

bool procesa5(tJugadores& jugadores, tColor jugador_turno, int& premio, bool& pasaturno, tCasillas& calle1, tCasillas& calle2) {
	// devuelve = true si ha sacado ficha y false si no
	// premio = 20 si come al sacar ficha
	// pasaturno = true si no come al salir y falso si come

	tFichas jugador;
	int ficha;
	int salida;
	bool resultado;

	premio = 0;

	//Comorobamos si queda alguna ficha en casa
	for (int i = 0; i < n_fichas; i++) {
		jugador[i] = jugadores[colorAIndice(jugador_turno)][i];
	}
	ficha = primeraEn(jugador, -1);

	if (ficha == -1) { resultado = false;   pasaturno = true; } //no hay ninguna ficha por sacar
	else {
		//Comprobar cuantas fichas hay en la casilla de salida
		salida = colorAsalida(jugador_turno);
		//Si hay 0 o 1 sacamos una ficha
		if (calle2[salida] == Ninguno) {
			saleFicha(jugadores, jugador_turno, calle1, calle2);
			resultado = true;
			pasaturno = true;
		}
		else {
			//Si hay dos fichas de nuestro color no se puede sacar ficha, por lo que hay que contar 5 con alguna de las que estan fuera
			if (calle1[salida] == jugador_turno && calle2[salida] == jugador_turno) { resultado = false;  pasaturno = false;}
			else {
				//Si hay dos fichas iguales de otro color comemos ficha, sacamos la nuestra y ganamos premio
				if (calle1[salida] != jugador_turno && calle2[salida] == calle1[salida]) {
					//comersalida2(jugadores, salida, calle2);
					aCasita(jugadores, salida, calle2);
					saleFicha(jugadores, jugador_turno, calle1, calle2);
					premio = 20;
					resultado = true;
					pasaturno = false;
				}
				else {
					//Si hay 2 y una es nuestra, la del otro color la comemos, sacamos la nuestra y ganamos premio
					if (calle1[salida] == jugador_turno) {
						//comersalida2(jugadores, salida, calle2);
						aCasita(jugadores, salida, calle2);
						saleFicha(jugadores, jugador_turno, calle1, calle2);
						premio = 20;
						resultado = true;
						pasaturno = false;
					}
					else {
						//Si hay 2 de colores distintos, comemos la ultima que haya llegado, sacamos la nuestra y ganamos premio.
						//comersalida2(jugadores, salida, calle2);
						aCasita(jugadores, salida, calle2);
						saleFicha(jugadores, jugador_turno, calle1, calle2);
						premio = 20;
						resultado = true;
						pasaturno = false;
					}
				}
			}
			
		}
	}

	return resultado;

}

bool puedeMover(tJugadores jugadores, tColor jugador_turno, int ficha, int& casilla, int tirada, tCasillas calle1, tCasillas calle2) {

	//devuelve = true si se puede mover y false si no
	//casilla = casilla de destino si se puede mover

	bool resultado;
	int puentes = 0;
	int aux;
	int i;
	int casilla_aux;
	int indice;
	tFichas jugador;

	indice = colorAIndice(jugador_turno);
	carga_jugador(jugadores, jugador_turno, jugador);
	
	casilla = jugadores[indice][ficha];

	if (casilla == -1 || casilla == 108) { resultado = false; }
	else {
		//Verificar si se puede mover
		i = 1;
		casilla_aux = casilla;
		if (casilla_aux == colorAzanata(jugador_turno)) { casilla_aux = 101; }
		else if (casilla_aux > 100) { casilla_aux = casilla_aux + 1; }
		else { casilla_aux = (casilla_aux + 1) % 68; }

		while (i < tirada) {
			if ((puente( calle1, calle2, casilla_aux) == true && zonasegura(casilla_aux))|| casilla_aux > 108 ) { puentes++; } 

			if (casilla_aux == colorAzanata(jugador_turno)) { casilla_aux = 101; }
			else if (casilla_aux > 100) { casilla_aux = casilla_aux + 1; }
			else { casilla_aux = (casilla_aux + 1) % 68; } 
			i = i + 1;
		} 
		
		if (puentes == 0) {
			if (casilla_aux < 100) {
				if (calle2[casilla_aux] == Ninguno) { resultado = true; }
				else { resultado = false; }
			}
			else {
				if (cuantasEn(jugador, casilla_aux) < 2) { resultado = true; }
				else { resultado = false; }
			}
		}
		else { resultado = false; }

		//Contar la tirada
		if (resultado == true) {
			casilla = casilla_aux ;		
		}
	}

	return resultado;
}

void mover(tJugadores jugadores, tColor jugador_turno, int ficha, int casilla, int& premio, int& ultimaFichaMovida, tCasillas calle1, tCasillas calle2) {
	
	int indice;
	int casilla_ori;
	tColor color_comido;

	// quitar la ficha de la ubicacion actual
	indice = colorAIndice(jugador_turno);
	casilla_ori = jugadores[indice][ficha];
	if (calle2[casilla_ori] == jugador_turno) {	calle2[casilla_ori] = Ninguno; }
	else {
		calle1[casilla_ori] = calle2[casilla_ori];
		calle2[casilla_ori] = Ninguno;
	}

	// asignamos la nueva casilla
	jugadores[indice][ficha] = casilla;
	ultimaFichaMovida = ficha;

	// Premio meta
	premio = 0;
	if (casilla == 108) { 
		premio = 10; 
		cout << " ************ ¡Atencion! Ha llegado a meta con una ficha. Tiene un movimiento extra de tirada 10.  ************" << endl;
	}
	// movemos a la nueva casilla en calle
	else if (casilla < 100) {
		if (calle1[casilla] == Ninguno) { calle1[casilla] = jugador_turno; }
		else if (calle1[casilla] == jugador_turno) { calle2[casilla] = jugador_turno; }
		else if (zonasegura(casilla) == true) { calle2[casilla] = jugador_turno; }
		else {
			calle2[casilla] = calle1[casilla];
			calle1[casilla] = jugador_turno;
			aCasita(jugadores, casilla, calle2);
			premio = 20;
			cout << " ************ ¡Atencion! Ha comido una ficha "<< colorACadena(calle2[casilla]) <<". Tiene un movimiento extra de tirada 20.  ************" << endl;
		}
	}

}

bool procesa6(tJugadores jugadores, tColor jugador_turno, int& premio, bool& pasa_turno, int& seises, int& ultimaFichaMovida, int& tirada, tCasillas calle1, tCasillas calle2) {
	// devuelve = true si se ha jugado ficha y false si no
	// premio = no cambia
	// pasaturno = devuelve true si llega al tercer 6 sino false
	// seises = incrementa en 1 el valor recibido
	// ultimaFichaMovida = devuelve la ficha movida si ha abierto un puente automáticamente
	// tirada = devuelve 7 si todas fichas fuera de casa sino 6

	tFichas jugador;
	tColor aux_color;
	bool resultado = false;
	int puentes, indPuente;
	int aux_casilla;

	//Actualizar la tirada a 7 si no hay fichas en casa
	carga_jugador(jugadores, jugador_turno, jugador);
	if (cuantasEn(jugador, -1) == 0) {
		tirada = 7;
		cout << " ************ ¡Atencion! Tiene todas las fichas fuera de casa. Cuenta 7.  ************" << endl;
	}

	//Si es el tercer 6 y no esta en la zanata entonces mandamos a casa la ultima ficha movida
	seises = seises + 1;
	if (seises == 3) {
		if (jugador[ultimaFichaMovida] < 100) {
			if (calle1[jugador[ultimaFichaMovida]] == jugador_turno) { 
				aux_color = calle2[jugador[ultimaFichaMovida]];
				calle2[jugador[ultimaFichaMovida]] = calle1[jugador[ultimaFichaMovida]];
				calle1[jugador[ultimaFichaMovida]] = aux_color;
			}
			aCasita(jugadores, jugador[ultimaFichaMovida], calle2);
			cout << " ************ ¡Atencion! 3 seises consecutivos. La ultima ficha movida se envia a casa.  ************" << endl;
		}
		pasa_turno = true;
		resultado = true;
	}
	else {
		//Contar cuantos puentes hay de nuestro color
		puentes = 0;
		indPuente = -1;
		for (int i = 0; i < n_casillas; i++) {
			if (puente(calle1,calle2,i) == true 
				&& calle1[i] == jugador_turno
				&& puedeMover(jugadores, jugador_turno, primeraEn(jugador,i), aux_casilla, tirada, calle1, calle2) == true) {
				puentes = puentes + 1;
				indPuente = i;
			}
		}
		//Si no hay ninguno sigue tirando
		if (puentes == 0) { 
			resultado = false;
		}
		else if (puentes == 1) {
			//Si hay 1 se realiza el movimiento obligatorio y sigue tirando
			mover(jugadores, jugador_turno, primeraEn(jugador, indPuente), indPuente + tirada, premio, ultimaFichaMovida, calle1, calle2);
			resultado = true;
		}
		else if (puentes == 2) {
			//Si hay 2 no se hace nada en la funcion
			resultado = false;
		}
		pasa_turno = false;
	}

	return resultado;

}

bool jugar(tJugadores jugadores, tColor jugador_turno, int& premio, bool& fin, int& seises, int& ultimaFichaMovida, int tirada, tCasillas calle1, tCasillas calle2) {
	// devuelve = true si se debe pasar el turno y falso sino
	// premio = valor del premio si se ha comido o llegado a meta
	// fin = true si el jugador quiere salir del programa o si todos en meta
	// seises = numero de seises consecutivos
	// ultimaFichaMovida = indice de la última ficha movida en ese color

	int indice;
	int cuantas;
	int casilla_ori;
	int casilla;
	int casilla_dest;
	int ficha;
	bool puede;
	bool ok;
	bool resultado;

	fin = false;

	// Comprobar cuantas fichas puede mover el jugador
	indice = colorAIndice(jugador_turno);
	cuantas = 0;
	for (int i = 0; i < n_fichas; i++) {
		casilla_dest = jugadores[indice][i];
		puede = puedeMover(jugadores, jugador_turno, i, casilla_dest, tirada, calle1, calle2);
		if (puede == true && jugadores[indice][i] != 108) {
			cuantas = cuantas + 1;
			ficha = i;
			casilla = casilla_dest;
		}
	}

	// Cuando no se puede mover, se informa
	if (cuantas == 0) {
		if ((tirada == 6 || tirada == 7) && seises < 3) { resultado = false; }
		else { 
			resultado = true; 
			cout << " ************ ¡Atencion! No puede mover ninguna ficha. Pasa turno.  ************" << endl;
		}

	}
	// Cuando solo hay una para mover, se informa y se mueve
	else if (cuantas == 1) {
		cout << " ************ ¡Atencion! Solo puede mover una ficha. Se mueve esta.  ************" << endl;
		cout << " ************ Se mueve la ficha " << ficha << " a la casilla " << casilla << ".  ************" << endl;
		mover(jugadores, jugador_turno, ficha, casilla, premio, ultimaFichaMovida, calle1, calle2);
		if ( premio == 0 && tirada != 6 && tirada != 7) { resultado = true; }
		else { resultado = false; }
	}
	// Cuando hay mas de una para mover, se listan las opciones y se mueve
	else if (cuantas > 1) {
		// Listar fichas posibles
		cout << "Por favor, elige la ficha que quieres mover..." << endl;
		for (int i = 0; i < n_fichas; i++) {
			casilla_dest = jugadores[indice][i];
			puede = puedeMover(jugadores, jugador_turno, i, casilla_dest, tirada, calle1, calle2);
			if (puede == true && jugadores[indice][i] != 108) {
				cout << i + 1 << ": De la casilla " << jugadores[indice][i] << " a la casilla " << casilla_dest << endl;
			}
		}
		// Elegir ficha
		cout << "Ficha(0 para salir) : ";
		cin >> ficha;
		cout << endl;
		// Si elige 0 salir del programa
		if (ficha == 0) {
			fin = true;
			resultado = true;
		}
		else {
			ok = false;
			do {
				casilla_dest = jugadores[indice][ficha - 1];
				if (puedeMover(jugadores, jugador_turno, ficha - 1, casilla_dest, tirada, calle1, calle2) == true && jugadores[indice][ficha - 1] != 108) {
					ok = true;
				}
				else {
					cout << "Por favor, elige la ficha que quieres mover..." << endl;
					cout << "Ficha(0 para salir) : ";
					cin >> ficha;
					cout << endl;
					if (ficha == 0) {
						fin = true;
						ok = true;
					}
				}

			} while (ok == false);
		}
		// Mover ficha
		if (fin == false) {
			casilla_ori = jugadores[indice][ficha - 1];
			puedeMover(jugadores, jugador_turno, ficha - 1, casilla_ori, tirada, calle1, calle2);
			mover(jugadores, jugador_turno, ficha - 1, casilla_ori, premio, ultimaFichaMovida, calle1, calle2);
			if (premio == 0 && tirada != 6 && tirada != 7) { resultado = true; }
			else { resultado = false; }
		}
	}
	return resultado;
}

void mostrar(const tJugadores jugadores, const tCasillas calle1, const tCasillas calle2) {
	int casilla, ficha;
	tColor jug;

	//cout << "\x1b[2J\x1b[H"; // Se situa en la esquina superior izquierda
	//setColor(Gris);
	cout << endl;

	// Filas con la numeraci�n de las casillas...
	for (int i = 0; i < n_casillas; i++)
		cout << i / 10;
	cout << endl;
	for (int i = 0; i < n_casillas; i++)
		cout << i % 10;
	cout << endl;

	// Borde superior...
	for (int i = 0; i < n_casillas; i++)
		cout << '>';
	cout << endl;

	// Primera fila de posiciones de fichas...
	for (int i = 0; i < n_casillas; i++) {
		setColor(calle2[i]);
		if (calle2[i] != Ninguno)
			cout << segundaEn(jugadores[calle2[i]], i) + 1;
		else
			cout << ' ';
		setColor(Gris);
	}
	cout << endl;

	// "Mediana"   
	for (int i = 0; i < n_casillas; i++)
		if (zonasegura(i))
			cout << 'o';
		else
			cout << '-';
	cout << endl;


	// Segunda fila de posiciones de fichas...
	for (int i = 0; i < n_casillas; i++) {
		setColor(calle1[i]);
		if (calle1[i] != Ninguno)
			cout << primeraEn(jugadores[calle1[i]], i) + 1;
		else
			cout << ' ';
		setColor(Gris);
	}
	cout << endl;

	jug = Amarillo;
	// Borde inferior...
	for (int i = 0; i < n_casillas; i++)
		if (i == colorAzanata(jug)) {
			setColor(jug);
			cout << "V";
			setColor(Gris);
		}
		else if (i == colorAsalida(jug)) {
			setColor(jug);
			cout << "^";
			setColor(Gris);
			jug = tColor(int(jug) + 1);
		}
		else
			cout << '>';
	cout << endl;

	// Metas y casas...
	for (int i = 0; i < n_fichas; i++) {
		casilla = 0;
		jug = Amarillo;
		setColor(jug);
		while (casilla < n_casillas) {
			if (casilla == colorAzanata(jug)) {
				ficha = primeraEn(jugadores[jug], 101 + i);
				if (ficha != -1) {
					cout << ficha + 1;
					if (cuantasEn(jugadores[jug], 101 + i) > 1) {
						ficha = segundaEn(jugadores[jug], 101 + i);
						if (ficha != -1) {
							cout << ficha + 1;
						}
						else
							cout << "V";
					}
					else
						cout << "V";
				}
				else
					cout << "VV";
				casilla++;
			}
			else if (casilla == colorAsalida(jug)) {
				if (jugadores[jug][i] == -1) // En casa
					cout << i + 1;
				else
					cout << "^";
				jug = tColor(int(jug) + 1);
				setColor(jug);
			}
			else
				cout << ' ';
			casilla++;
		}
		cout << endl;
	}

	// Resto de metas...
	for (int i = 105; i <= 107; i++) {
		casilla = 0;
		jug = Amarillo;
		setColor(jug);
		while (casilla < n_casillas) {
			if (casilla == colorAzanata(jug)) {
				ficha = primeraEn(jugadores[jug], i);
				if (ficha != -1) {
					cout << ficha + 1;
					if (cuantasEn(jugadores[jug], i) > 1) {
						ficha = segundaEn(jugadores[jug], i);
						if (ficha != -1) {
							cout << ficha + 1;
						}
						else
							cout << "V";
					}
					else
						cout << "V";
				}
				else
					cout << "VV";
				casilla++;
				jug = tColor(int(jug) + 1);
				setColor(jug);
			}
			else
				cout << ' ';
			casilla++;
		}
		cout << endl;
	}

	casilla = 0;
	jug = Amarillo;
	setColor(jug);
	while (casilla < n_casillas) {
		cout << ((jugadores[jug][0] == 108) ? '1' : '.');
		cout << ((jugadores[jug][1] == 108) ? '2' : '.');
		jug = tColor(int(jug) + 1);
		setColor(jug);
		cout << "               ";
		casilla += 17;
	}
	cout << endl;
	casilla = 0;
	jug = Amarillo;
	setColor(jug);
	while (casilla < n_casillas) {
		cout << ((jugadores[jug][2] == 108) ? '3' : '.');
		cout << ((jugadores[jug][3] == 108) ? '4' : '.');
		jug = tColor(int(jug) + 1);
		setColor(jug);
		cout << "               ";
		casilla += 17;
	}
	cout << endl << endl;
	setColor(Gris);
}

void setColor(tColor color) {
	switch (color) {
	case Azul:
		cout << "\x1b[34;107m";
		break;
	case Verde:
		cout << "\x1b[32;107m";
		break;
	case Rojo:
		cout << "\x1b[31;107m";
		break;
	case Amarillo:
		cout << "\x1b[33;107m";
		break;
	case Gris:
	case Ninguno:
		cout << "\x1b[90;107m";
		break;
	}
}

void pausa() {
	cout << "Pulsa Intro para continuar...";
	//cin.ignore(numeric_limits<streamsize>::max(), '\n');
	system("PAUSE");
}

void iniciaColores() {
#ifdef _WIN32
	for (DWORD stream : {STD_OUTPUT_HANDLE, STD_ERROR_HANDLE}) {
		DWORD mode;
		HANDLE handle = GetStdHandle(stream);

		if (GetConsoleMode(handle, &mode)) {
			mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			SetConsoleMode(handle, mode);
		}
	}
#endif
}

void pinta_calles(const tJugadores jugadores, const tCasillas calle1, const tCasillas calle2) {
	for (int i = 0; i < n_casillas; i++)
		cout << calle1[i];
	cout << endl;
	for (int i = 0; i < n_casillas; i++)
		cout << calle2[i];
	cout << endl;
	cout << endl;
}

void posicionar() {
	cout << "\x1b[2J\x1b[H"; // Se situa en la esquina superior izquierda
	setColor(Gris);
	cout << endl;
}