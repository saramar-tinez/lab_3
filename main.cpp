#include <iostream>
#include <string>
#include <fstream>

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

void ejercicio_RLE();
void ejercicio_LZ78();
void ejercicio_encriptacion();
void ejercicio_integracion();


// 5.1 COMPRESION RLE

string comprimir_RLE(string texto) {
    string resultado = "";
    int n = texto.length();
    int i = 0;

    while (i < n) {
        char letra = texto[i];
        int contador = 1;

        while (i + contador < n && texto[i + contador] == letra)
            contador++;

        resultado += to_string(contador) + letra;
        i += contador;
    }
    return resultado;
}

string descomprimir_RLE(string comprimido) {
    string resultado = "";
    int n = comprimido.length();
    int i = 0;

    while (i < n) {
        string num = "";

        while (i < n && isdigit(comprimido[i])) {
            num += comprimido[i];
            i++;
        }
        if (i >= n || num.empty()) break;

        char letra = comprimido[i];
        i++;
        int veces = stoi(num);

        for (int j = 0; j < veces; j++)
            resultado += letra;
    }
    return resultado;
}

void ejercicio_RLE() {
    try {
        string texto, comprimido, descomprimido;

        cout << "5.1 Compresion RLE" << endl;
        cout << "Ingresa el texto: ";
        cin >> texto;

        if (texto.empty()) throw runtime_error("El texto no puede estar vacio.");

