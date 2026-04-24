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
        comprimido    = comprimir_RLE(texto);
        descomprimido = descomprimir_RLE(comprimido);

        cout << "Texto original:      " << texto << endl;
        cout << "Texto comprimido:    " << comprimido << endl;
        cout << "Texto descomprimido: " << descomprimido << endl;

        if (texto == descomprimido)
            cout << "Verificacion exitosa." << endl;
        else
            cout << "Error: el texto no coincide." << endl;

    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}


// 5.2 COMPRESION LZ78

struct EntradaDiccionario {
    int  prefijo;
    char caracter;
};

struct Par {
    int  indice;
    char caracter;
};

void reconstruir_frase(EntradaDiccionario* dicc, int idx, char* buffer, int& len) {
    if (idx == 0) { len = 0; return; }
    reconstruir_frase(dicc, dicc[idx - 1].prefijo, buffer, len);
    buffer[len++] = dicc[idx - 1].caracter;
}

Par* comprimir_LZ78(const char* texto, int n, EntradaDiccionario*& dicc, int& tam_dicc, int& num_pares) {
    int cap_dicc  = 256;
    int cap_pares = 256;

    dicc       = new EntradaDiccionario[cap_dicc];
    Par* pares = new Par[cap_pares];
    tam_dicc   = 0;
    num_pares  = 0;

    int i = 0;
    while (i < n) {
        int prefijo = 0;
        int match   = 0;

        while (i + match < n) {
            char c = texto[i + match];
            bool encontrado = false;

            for (int d = 0; d < tam_dicc; d++) {
                if (dicc[d].prefijo == prefijo && dicc[d].caracter == c) {
                    prefijo = d + 1;
                    match++;
                    encontrado = true;
                    break;
                }
            }
            if (!encontrado) break;
        }

        char c_nuevo = (i + match < n) ? texto[i + match] : '\0';

        if (num_pares >= cap_pares) {
            cap_pares *= 2;
            Par* tmp = new Par[cap_pares];
            for (int k = 0; k < num_pares; k++) tmp[k] = pares[k];
            delete[] pares;
            pares = tmp;
        }
        pares[num_pares++] = {prefijo, c_nuevo};

        if (tam_dicc >= cap_dicc) {
            cap_dicc *= 2;
            EntradaDiccionario* tmp = new EntradaDiccionario[cap_dicc];
            for (int k = 0; k < tam_dicc; k++) tmp[k] = dicc[k];
            delete[] dicc;
            dicc = tmp;
        }
        dicc[tam_dicc++] = {prefijo, c_nuevo};

        i += match + 1;
    }
    return pares;
}

char* descomprimir_LZ78(Par* pares, int num_pares, int& len_resultado) {
    int cap_dicc = 256;
    EntradaDiccionario* dicc = new EntradaDiccionario[cap_dicc];
    int tam_dicc = 0;

    int cap_res = 1024;
    char* resultado = new char[cap_res];
    len_resultado = 0;

    char buffer[512];

    for (int p = 0; p < num_pares; p++) {
        int  idx = pares[p].indice;
        char c   = pares[p].caracter;

        int len_buf = 0;
        reconstruir_frase(dicc, idx, buffer, len_buf);
        buffer[len_buf++] = c;

        while (len_resultado + len_buf >= cap_res) {
            cap_res *= 2;
            char* tmp = new char[cap_res];
            for (int k = 0; k < len_resultado; k++) tmp[k] = resultado[k];
            delete[] resultado;
            resultado = tmp;
        }

        for (int k = 0; k < len_buf; k++) resultado[len_resultado++] = buffer[k];

        if (tam_dicc >= cap_dicc) {
            cap_dicc *= 2;
            EntradaDiccionario* tmp = new EntradaDiccionario[cap_dicc];
            for (int k = 0; k < tam_dicc; k++) tmp[k] = dicc[k];
            delete[] dicc;
            dicc = tmp;
        }
        dicc[tam_dicc++] = {idx, c};
    }

    resultado[len_resultado] = '\0';
    delete[] dicc;
    return resultado;
}

void ejercicio_LZ78() {
    try {
        cout << "5.2 Compresion LZ78" << endl;
        cout << "Ingresa el texto: ";
        string entrada;
        cin >> entrada;

        if (entrada.empty()) throw runtime_error("El texto no puede estar vacio.");

        const char* texto = entrada.c_str();
        int n = entrada.length();

        EntradaDiccionario* dicc = nullptr;
        int tam_dicc  = 0;
        int num_pares = 0;

        Par* pares = comprimir_LZ78(texto, n, dicc, tam_dicc, num_pares);

        cout << "\nPares generados:" << endl;
        for (int i = 0; i < num_pares; i++)
            cout << "  (" << pares[i].indice << ", " << pares[i].caracter << ")" << endl;

        cout << "\nDiccionario:" << endl;
        for (int i = 0; i < tam_dicc; i++)
            cout << "  " << (i+1) << " -> prefijo:" << dicc[i].prefijo << " caracter:'" << dicc[i].caracter << "'" << endl;

        int len_resultado = 0;
        char* resultado = descomprimir_LZ78(pares, num_pares, len_resultado);

        cout << "\nTexto original:      " << texto << endl;
        cout << "Texto descomprimido: " << resultado << endl;

        if (entrada == string(resultado))
            cout << "Verificacion exitosa." << endl;
        else
            cout << "Error: el texto no coincide." << endl;

        delete[] pares;
        delete[] dicc;
        delete[] resultado;

    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}


