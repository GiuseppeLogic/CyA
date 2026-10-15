// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 6: Simulador de DFAs
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 15/10/2026
// Archivo: automata_finito.cc
// Descripción: Implementación de los métodos de la clase Automata, incluyendo
//              la sobrecarga del operador de extracción (>>) para la lectura y
//              validación del fichero .dfa, y la simulación paso a paso de cadenas.

#include "automata_finito.h"

/**
 * @brief Sobrecarga del operador de extracción para la clase Automata.
 * 
 * Lee la especificación de un DFA desde un flujo de entrada asegurando que
 * las líneas vacías o con espacios no desfasen el contador de líneas.
 */
std::istream& operator>>(std::istream& inp, Automata& automata) {
    std::string linea;
    int contador_linea{1};
    int contador_estados{0};

    automata.automata_invalido_ = false;
    automata.alfabeto_.clear();
    automata.estados_aceptacion_.clear();
    automata.funcion_transicion_.clear();

    while (std::getline(inp, linea)) {
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }

        if (linea.empty()) {
            continue;
        }

        if (linea.length() >= 2 && linea.substr(0, 2) == "//") {
            continue;
        }

        std::stringstream string_stream{linea};

        switch (contador_linea) {
            case 1: { 
                std::string token;
                while (string_stream >> token) {
                    for (const char c : token) {
                        automata.alfabeto_.insert(std::string(1, c));
                    }
                }
                break;
            }
            case 2:
                string_stream >> automata.cantidad_estados_;
                break;
            case 3:
                string_stream >> automata.estado_arranque_;
                break; 
            default:
                break;
        }

        if (contador_linea > 3) {
            int estado, aceptacion;
            std::string simbolo_entrada;
            int estado_siguiente;
            
            if (!(string_stream >> estado >> aceptacion)) {
                std::cout << "ERROR: Formato incorrecto en la línea de estado " 
                          << contador_linea << ".\n";
                automata.automata_invalido_ = true;
                return inp;
            }

            if (aceptacion == 1) {
                automata.estados_aceptacion_.insert(estado);
            } else if (aceptacion != 0) {
                std::cout << "ERROR: El indicador de aceptación debe ser 0 o 1.\n";
                automata.automata_invalido_ = true;
                return inp;
            }

            while (string_stream >> simbolo_entrada >> estado_siguiente) {

                if (!automata.alfabeto_.count(simbolo_entrada)) {
                    std::cout << "ERROR: El símbolo '" << simbolo_entrada << "' no está contenido en el alfabeto.\n";
                    automata.automata_invalido_ = true;
                    return inp;
                }

                if (estado_siguiente < 0 || estado_siguiente >= automata.cantidad_estados_) {
                    std::cout << "ERROR: El estado destino " << estado_siguiente 
                              << " no es un estado válido del autómata.\n";
                    automata.automata_invalido_ = true;
                    return inp;
                }

                std::pair<int, std::string> par_estado_simbolo{estado, simbolo_entrada};
                automata.funcion_transicion_.insert({par_estado_simbolo, estado_siguiente});
            }
            
            ++contador_estados;
        }
        ++contador_linea;
    } 

    if (automata.cantidad_estados_ != contador_estados) {
        std::cout << "ERROR: La cantidad de estados leídos (" << contador_estados 
                  << ") no coincide con los especificados en el fichero (" 
                  << automata.cantidad_estados_ << ").\n";
        automata.automata_invalido_ = true;
    }

    return inp;
}

/**
 * @brief Simula la ejecución del DFA para una cadena de entrada y muestra la traza.
 * 
 * Procesa la cadena símbolo a símbolo, mostrando por pantalla una tabla con
 * el estado actual, el símbolo consumido y el estado siguiente. Si la cadena
 * es el símbolo especial '&', se evalúa la cadena vacía. Si una transición
 * no está definida, el autómata transita a un estado de absorción/muerte (-1).
 * 
 * @param cadena Cadena de caracteres a evaluar por el autómata.
 */
void Automata::ComprobarCadena(const std::string& cadena) const {
    std::cout << std::left 
              << std::setw(15) << "Est. Actual" 
              << std::setw(12) << "Entrada" 
              << std::setw(12) << "Nuevo Est." << "\n";

    int estado_actual{estado_arranque_};

    if (cadena == "&") {
        if (estados_aceptacion_.count(estado_arranque_)) {
            std::cout << "\nCadena de entrada ACEPTADA.\n";
        } else {
            std::cout << "\nCadena de entrada NO ACEPTADA.\n";
        }
        return;
    }

    for (const char entrada : cadena) {
        std::string simbolo(1, entrada);

        if (!alfabeto_.count(simbolo)) {
            std::cout << "ERROR: El símbolo de entrada '" << entrada 
                      << "' no pertenece al alfabeto.\n";
            std::cout << "\nCadena de entrada NO ACEPTADA.\n";
            return;
        }

        int estado_siguiente{-1}; 
        std::pair<int, std::string> clave{estado_actual, simbolo};

        if (estado_actual != -1 && funcion_transicion_.count(clave) > 0) {
            estado_siguiente = funcion_transicion_.at(clave);
        }

        std::cout << std::left 
                  << std::setw(15) << (estado_actual == -1 ? "Muerte (-1)" : std::to_string(estado_actual))
                  << std::setw(12) << entrada
                  << std::setw(12) << (estado_siguiente == -1 ? "Muerte (-1)" : std::to_string(estado_siguiente)) 
                  << "\n";

        estado_actual = estado_siguiente;
    }

    bool aceptada = (estado_actual != -1) && (estados_aceptacion_.count(estado_actual) > 0);

    if (aceptada) {
        std::cout << "\nCadena de entrada ACEPTADA.\n";
    } else {
        std::cout << "\nCadena de entrada NO ACEPTADA.\n";
    }
}

/**
 * @brief Muestra el modo correcto de ejecución del programa.
 * 
 * Imprime por pantalla la sintaxis necesaria para invocar el ejecutable 
 * con los parámetros requeridos por la línea de comandos, indicando el 
 * ejecutable Automata y sus dos ficheros de entrada.
 */
void MostrarUso() {
    std::cout << "Modo de empleo: ./Automata input.dfa input.txt\n"
              << "Pruebe './Automata --help' para más información.\n";
}

/**
 * @brief Muestra la ayuda detallada del programa.
 * 
 * Explica el propósito del simulador de DFA, los parámetros requeridos 
 * (fichero de especificación .dfa y fichero de cadenas .txt) y su significado.
 */
void MostrarAyuda() {
    std::cout << "Simulador de Autómatas Finitos Deterministas (DFA).\n\n"
              << "USO:\n"
              << "  ./Automata <input.dfa> <input.txt>\n\n"
              << "PARÁMETROS:\n"
              << "  <input.dfa>  Fichero de texto con la especificación de un DFA.\n"
              << "  <input.txt>  Fichero de texto con una serie de cadenas (una por línea) sobre el alfabeto del autómata a evaluar.\n";
}

