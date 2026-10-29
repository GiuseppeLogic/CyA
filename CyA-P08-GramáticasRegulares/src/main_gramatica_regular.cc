// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramática Regular a partir de un DFA
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 29/10/2026
// Archivo: main_gramatica_regular.cc
// Descripción: Contiene la función principal (main) del programa dfa2grammar, 
//              encargada de procesar los argumentos de la línea de comandos, 
//              validar los ficheros, instanciar el autómata y generar la gramática.

#include <iostream>
#include <fstream>
#include <string>

#include "gramatica_regular.h"


int main(int argc, char* argv[]) {

    if (argc == 2 && std::string{argv[1]} == "--help") {
        MostrarAyuda();
        return 0;
    }

    if (argc != 3) {
        MostrarUso();
        return 1;
    }

    std::string direccion_dfa{argv[1]};
    std::string direccion_gramatica{argv[2]};

    std::ifstream inp_dfa(direccion_dfa);
    if (!inp_dfa.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el fichero de especificación del DFA: " 
                  << direccion_dfa << "\n";
        return 1;
    }
    inp_dfa.close();

    std::ofstream out_gramatica(direccion_gramatica);
    if (!out_gramatica.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el fichero de la gramática: " 
                  << direccion_gramatica << "\n";
        return 1;
    }

    Automata automata(direccion_dfa);

    Gramatica gramatica = automata.ConvertToGrammar();
    
    out_gramatica << gramatica;

    return 0;
}