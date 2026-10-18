// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 6: Simulador de DFAs
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 15/10/2026
// Archivo: main_simulador_automatas.cc
// Descripción: Contiene la función principal (main) del programa, encargada 
//              de procesar los argumentos de la línea de comandos, validar 
//              la apertura de los ficheros e invocar la simulación del DFA.


#include "automata_finito.h"

int main(int argc, char* argv[]) {
    bool opcion_complementario{false};

    if (argc == 2 && std::string{argv[1]} == "--help") {
        MostrarAyuda();
        return 0;
    }

    if (argc == 4 && std::string{argv[3]} == "-c"){
        opcion_complementario = true;
    }

    if (argc != 3 && argc != 4) {
        MostrarUso();
        return 1;
    }

    std::string direccion_dfa{argv[1]};
    std::string direccion_entrada{argv[2]};

    std::ifstream inp_dfa(direccion_dfa);
    if (!inp_dfa.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el fichero de especificación del DFA: " 
                  << direccion_dfa << "\n";
        return 1;
    }

    std::ifstream inp_entrada(direccion_entrada);
    if (!inp_entrada.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el fichero de cadenas de entrada: " 
                  << direccion_entrada << "\n";
        return 1;
    }

    Automata automata;
    inp_dfa >> automata;

    if (automata.getter_invalido() || ! automata.EsCompatible()) {
        std::cerr << "ERROR: El autómata es inválido o no compatible. Se finaliza la ejecución del programa.\n";
        return 1;
    }

    if (opcion_complementario){
        automata.HacerComplementario();
        std::cout << "Evaluando lenguaje complementario...\n";
    }

    std::cout << automata;

    std::string cadena;
    while (std::getline(inp_entrada, cadena)) {
        if (!cadena.empty() && cadena.back() == '\r') {
            cadena.pop_back();
        }

        std::cout << "\n----------------------------------------\n";
        std::cout << "Evaluando cadena: \"" << cadena << "\"\n";
        std::cout << "----------------------------------------\n";
        
        automata.ComprobarCadena(cadena);
    }

    return 0;
}