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
 * @brief Imprime el alfabeto del autómata en formato de conjunto (ej. "{a,b,c}").
 * 
 * @param out Flujo de salida (ej. std::cout).
 * @param alfabeto Conjunto de símbolos a imprimir.
 * @return std::ostream& Referencia al flujo de salida.
 */
std::ostream& operator<<(std::ostream& out, const std::set<std::string>& alfabeto){
    out << "{";
    auto it = alfabeto.begin();
    while (it != alfabeto.end()) {
        out << *it;
        ++it;
        if (it != alfabeto.end()) {
            out << ",";
        }
    }
    out << "}\n";
    return out;
}

/**
 * @brief Imprime un vector de estados en formato de conjunto (ej. "{0,1,2}").
 * 
 * @param out Flujo de salida (ej. std::cout).
 * @param estados Vector con los identificadores de los estados.
 * @return std::ostream& Referencia al flujo de salida.
 */
std::ostream& operator<<(std::ostream& out, const std::vector<int>& estados){
    out << "(";
    for (size_t i = 0; i < estados.size(); ++i) {
        out << estados[i];
        if (i < estados.size() - 1) {
            out << ",";
        }
    }
    out << ")\n";
    return out;
}


/**
 * @brief Sobrecarga del operador de inserción (<<) para imprimir un par de estados.
 * 
 * Formatea un par de enteros (std::pair<int, int>) simulando la notación de un par
 * ordenado o conjunto de dos elementos (ej. "{0,1}") seguido de un salto de línea.
 * 
 * @param out Flujo de salida donde se insertará el texto (ej. std::cout).
 * @param par_estados Par de enteros que representa dos identificadores de estados.
 * @return std::ostream& Referencia al propio flujo de salida para permitir encadenamiento.
 */
std::ostream& operator<<(std::ostream& out, const std::pair<int,int>& par_estados){
    out << "{" << par_estados.first << "," << par_estados.second << "}" << "\n";
    return out;
}

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
 * @brief Sobrecarga del operador de inserción (<<) para representar el Autómata.
 * 
 * Imprime una cabecera estructurada con la información del autómata (alfabeto y
 * cantidad de estados) seguida de una tabla formateada y alineada que muestra
 * la función de transición de forma legible.
 * 
 * @param out Flujo de salida donde se insertará el texto.
 * @param automata Objeto constante de la clase Automata que se va a imprimir.
 * @return std::ostream& Referencia al flujo de salida para encadenar operaciones.
 */
std::ostream& operator<<(std::ostream& out, const Automata& automata){
    out << "\n==========================================\n";
    out << "    REPRESENTACIÓN DEL AUTÓMATA (DFA)     \n";
    out << "==========================================\n";
    
    out << "Alfabeto: " << automata.alfabeto_;
    out << "Cantidad de estados: " << automata.cantidad_estados_ << "\n";
    auto estados_muerte = automata.EstadosAbsorcion(); 
    out << "Estados de muerte: ";

    if (estados_muerte.empty()) {
        out << "No tiene estados de absorción.\n";
    } else {
        out << estados_muerte << "\n"; 
    }

    auto estados_bucle = automata.EstadosBucle(); 
    out << "Estados con bucles propios: ";

    if (estados_bucle.empty()) {
        out << "No tiene estados con bucles propios.";
    } else {
        out << estados_bucle; 
    }

    auto estados_equivalentes = automata.EsMinimo();

    out << "¿ES MÍNIMO?: " << ((estados_equivalentes.size() == 0) ? "SÍ" : "NO") << "\n";
    out << "Lista de estados equivalentes:\n";
    if (estados_equivalentes.size() == 0){
        out << "Todos los estados son independientes.\n\n";
    }else{
        for (const auto& par : estados_equivalentes){
            out << par;
        }
        out << "\n";
    }
    

    out << "Función de Transición (δ):\n";
    out << "------------------------------------------------------------------\n";
    out << std::left << std::setw(15) << "Estado Origen" 
        << " | " << std::setw(10) << "Símbolo" 
        << " | " << std::setw(15) << "Estado Destino"
        << " | "  << "Estado Aceptación\n";
    out << "------------------------------------------------------------------\n";
    
    for (const auto& par_transicion : automata.funcion_transicion_){
        out << std::left << std::setw(15) << par_transicion.first.first   
            << " | " << std::setw(10) << par_transicion.first.second  
            << " | " << std::setw(15) << par_transicion.second
            << " | " << ( (automata.estados_aceptacion_.count(par_transicion.first.first) > 0) ? "SÍ" : "NO" ) << "\n";    
    }
    
    out << "\n==========================================\n";
    return out;
}

/**
 * @brief Obtiene el conjunto de estados de muerte (absorción) del autómata.
 * 
 * Un estado se considera de muerte o sumidero si cumple dos propiedades estrictas:
 * 1. Para todo símbolo del alfabeto, su transición tiene como destino a sí mismo.
 * 2. No pertenece al conjunto de estados de aceptación del autómata.
 * 
 * @return std::vector<int> Vector con los identificadores de los estados de muerte encontrados.
 */
std::vector<int> Automata::EstadosAbsorcion() const {
    std::vector<int> estados_absorcion;

    for (int estado{0}; estado != cantidad_estados_; ++estado) {
        if (estados_aceptacion_.count(estado) > 0) {
            continue; 
        }

        bool absorcion{true};
        
        for (const std::string& simbolo : alfabeto_) {
            auto iterador_transicion = funcion_transicion_.find({estado, simbolo});
            
            if (iterador_transicion == funcion_transicion_.end() || iterador_transicion->second != estado) {
                absorcion = false;
                break; 
            }
        }

        if (absorcion) {
            estados_absorcion.emplace_back(estado);
        }
    }    
    return estados_absorcion;
}

/**
 * @brief Identifica los estados que poseen al menos un bucle hacia sí mismos.
 * 
 * Un estado tiene un bucle interno (o self-loop) si existe alguna transición
 * que, al consumir un símbolo del alfabeto, tenga como destino final el
 * mismo estado origen.
 * 
 * @return std::vector<int> Vector con los identificadores únicos de los 
 * estados que poseen bucles internos, ordenados ascendentemente.
 */
std::vector<int> Automata::EstadosBucle() const {
    std::set<int> estados_unicos; 

    for (const auto& par_transicion : funcion_transicion_) {
        int estado_origen = par_transicion.first.first;
        int estado_destino = par_transicion.second;

        if (estado_origen == estado_destino) {
            estados_unicos.insert(estado_origen);
        }
    }

    std::vector<int> estados_bucle(estados_unicos.begin(), estados_unicos.end());
    
    return estados_bucle;
}


/**
 * @brief Comprueba si el autómata DFA es completo (compatible).
 * 
 * Un DFA es completo si y solo si todos sus estados tienen exactamente una 
 * transición definida para cada uno de los símbolos del alfabeto.
 * Al usar std::map (sin claves duplicadas), esto equivale a que el total de 
 * transiciones sea el producto de la cantidad de estados por la del alfabeto.
 * 
 * @return true Si el autómata es completo.
 * @return false Si falta al menos una transición en algún estado.
 */
bool Automata::EsCompatible() const {
    size_t transiciones_esperadas = cantidad_estados_ * alfabeto_.size();
    
    return funcion_transicion_.size() == transiciones_esperadas;
}

/**
 * @brief Transforma el DFA actual en uno que reconoce el lenguaje complementario.
 * 
 * Basado en el teorema de clausura bajo complementación, el DFA complementario 
 * mantiene los mismos estados (Q) y transiciones (δ), pero su conjunto de 
 * estados finales (F) pasa a ser la diferencia de conjuntos Q - F.
 */
void Automata::HacerComplementario() {
    std::set<int> nuevos_estados_aceptacion;

    for (int estado = 0; estado < cantidad_estados_; ++estado) {
        
        if (estados_aceptacion_.count(estado) == 0) {
            nuevos_estados_aceptacion.insert(estado);
        }
    }

    estados_aceptacion_ = nuevos_estados_aceptacion;
}


/**
 * @brief Comprueba si dos estados dados del autómata son equivalentes a nivel directo.
 * 
 * Dos estados se consideran equivalentes si cumplen dos condiciones indispensables:
 * 1. Ambos comparten la misma naturaleza respecto a la aceptación (ambos son 
 *    estados finales o ambos son estados no finales).
 * 2. Para cada símbolo del alfabeto, sus transiciones apuntan exactamente 
 *    al mismo estado destino.
 * 
 * @param estado_1 Identificador del primer estado a comparar.
 * @param estado_2 Identificador del segundo estado a comparar.
 * @return true Si ambos estados son equivalentes.
 * @return false Si difieren en su condición de aceptación o en alguna transición.
 */
bool Automata::EstadosEquivalentes(int estado_1, int estado_2) const{
    bool estado_1_aceptacion{estados_aceptacion_.count(estado_1) > 0};
    bool estado_2_aceptacion{estados_aceptacion_.count(estado_2) > 0};
    
    if (estado_1_aceptacion !=  estado_2_aceptacion){
        return false;
    }

    for (const std::string& simbolo : alfabeto_) {
        auto trans_estado_1 = funcion_transicion_.find({estado_1, simbolo});
        auto trans_estado_2 = funcion_transicion_.find({estado_2, simbolo});

        int dest_estado_1 = (trans_estado_1 != funcion_transicion_.end()) ? trans_estado_1->second : -1;
        int dest_estado_2 = (trans_estado_2 != funcion_transicion_.end()) ? trans_estado_2->second : -1;

        if (dest_estado_1 != dest_estado_2) {
            return false; 
        }
    }

    return true;
}

/**
 * @brief Encuentra todos los pares de estados equivalentes del autómata.
 * 
 * Recorre sistemáticamente todos los pares únicos de estados posibles 
 * (evitando duplicados y auto-comparaciones) mediante dos bucles anidados.
 * Utiliza el método de comprobación directa para evaluar si cada par es 
 * equivalente, almacenando aquellos que lo cumplan en un vector de pares.
 * 
 * @return std::vector<std::pair<int,int>> Vector que contiene todos los pares 
 * de estados equivalentes encontrados en el autómata.
 */
std::vector<std::pair<int,int>> Automata::EsMinimo() const{
    std::vector<std::pair<int,int>> estados_equivalentes;

    for (int estado_1{0}; estado_1 < cantidad_estados_ - 1; ++estado_1){
        for (int estado_2{estado_1 + 1}; estado_2 < cantidad_estados_; ++estado_2){
            if (EstadosEquivalentes(estado_1, estado_2)){
                estados_equivalentes.push_back({estado_1, estado_2}); 
            }
        }
    }

    return estados_equivalentes;
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
              << "  ./Automata <input.dfa> <input.txt> [-c]\n\n"
              << "PARÁMETROS:\n"
              << "  <input.dfa>  Fichero de texto con la especificación de un DFA.\n"
              << "  <input.txt>  Fichero de texto con una serie de cadenas (una por línea) sobre el alfabeto del autómata a evaluar.\n"
              << "  [-c]         Opción que simula el lenguaje complementario reconocido por el DFA.\n";

}

