// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramática Regular a partir de un DFA
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 29/10/2026
// Archivo: gramatica_regular.cc
// Descripción: Implementación de los métodos de la clase Gramatica, incluyendo
//              la sobrecarga del operador de inserción (<<) para formatear y
//              exportar la especificación de la gramática a un fichero .gra.

#include "gramatica_regular.h"

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
 * @brief Sobrecarga del operador de inserción (<<) para imprimir la gramática.
 * 
 * Muestra las reglas de producción agrupadas por cada símbolo no terminal 
 * separadas por '|' (ej. "S -> aS|bA"), coincidiendo con la notación formal[cite: 3].
 * 
 * @param out Flujo de salida (ej. std::cout).
 * @param gramatica Objeto de tipo Gramatica a representar.
 * @return std::ostream& Referencia al propio flujo de salida.
 */
std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica) {
    out << "\n";
    out << "==========================================\n";
    out << "    GRAMÁTICA REGULAR EQUIVALENTE          \n";
    out << "==========================================\n";

    for (const std::string& no_terminal : gramatica.simbolos_no_terminales) {
        std::vector<std::string> lados_derechos;

        for (const auto& produccion : gramatica.producciones_) {
            std::string origen = produccion.first.first;
            std::string simbolo = produccion.first.second;
            std::string destino = produccion.second;

            if (origen == no_terminal) {
                if (destino == "ε" || simbolo == "NA") {
                    lados_derechos.push_back("ε");
                } else {
                    lados_derechos.push_back(simbolo + destino);
                }
            }
        }

        if (!lados_derechos.empty()) {
            out << no_terminal << " -> ";
            for (size_t i = 0; i < lados_derechos.size(); ++i) {
                out << lados_derechos[i];
                if (i < lados_derechos.size() - 1) {
                    out << "|";
                }
            }
            out << "\n";
        }
    }
    out << "==========================================\n\n";

    return out;
}


/**
 * @brief Sobrecarga del operador de inserción de flujo para exportar la gramática.
 * 
 * Este método formatea y escribe los atributos de la gramática (alfabeto, 
 * símbolos no terminales, símbolo de arranque y reglas de producción) en un 
 * archivo de salida, siguiendo la estructura estricta de líneas separadas 
 * especificada para los ficheros de extensión .gra.
 * 
 * @param out Flujo de salida de archivo (std::ofstream) donde se escribirá la especificación.
 * @param gramatica Objeto constante de tipo Gramatica cuyos datos se van a exportar.
 * @return std::ofstream& Referencia al flujo de salida modificado, permitiendo el encadenamiento de operaciones de inserción.
 */
std::ofstream& operator<<(std::ofstream& out, const Gramatica& gramatica){
    out << gramatica.alfabeto_.size() << "\n";
    for (const std::string& simbolo_terminal : gramatica.alfabeto_){
        out << simbolo_terminal << "\n";
    }
    
    out << gramatica.simbolos_no_terminales.size() << "\n"; 
    for (const std::string& simbolo_no_terminal : gramatica.simbolos_no_terminales){
        out << simbolo_no_terminal << "\n";
    }
    
    out << gramatica.simbolo_arranque_ << "\n"; 
    
    out << gramatica.producciones_.size() << "\n"; 
    for (const auto& produccion : gramatica.producciones_){
        if(gramatica.lineal_izquierda){
             out << produccion.first.first << " -> " << produccion.second<< produccion.first.second  << "\n";
        }else{
            out << produccion.first.first << " -> " << produccion.first.second << produccion.second << "\n";
        }
       
    }
    
    return out;
}

/**
 * @brief Muestra el modo correcto de ejecución del programa.
 * 
 * Imprime por pantalla la sintaxis necesaria para invocar el ejecutable 
 * con los parámetros requeridos por la línea de comandos, indicando el 
 * ejecutable dfa2grammar y sus ficheros de entrada (.dfa) y salida (.gra).
 */
void MostrarUso() {
    std::cout << "Modo de empleo: ./dfa2grammar input.dfa output.gra\n"
              << "Pruebe 'dfa2grammar --help' para más información.\n";
}

/**
 * @brief Muestra la ayuda detallada del programa.
 * 
 * Explica el propósito del conversor de DFA a Gramática Regular, 
 * los parámetros requeridos (fichero de especificación .dfa de entrada y 
 * fichero de salida .gra), y proporciona un ejemplo práctico de ejecución.
 */
void MostrarAyuda() {
    std::cout << "=================================================================\n"
              << " Conversor de DFA a Gramática Regular    \n"
              << "=================================================================\n\n"
              << "USO:\n"
              << "  ./dfa2grammar <input.dfa> <output.gra>\n\n"
              << "PARÁMETROS:\n"
              << "  <input.dfa>  Fichero de texto con la especificación de un DFA.\n"
              << "  <output.gra> Fichero de texto donde se guardará la gramática regular generada.\n\n"
              << "EJEMPLOS DE EJECUCIÓN:\n"
              << "  1. Conversión estándar:\n"
              << "     ./dfa2grammar mi_automata.dfa mi_gramatica.gra\n"
              << "=================================================================\n";
}


/**
 * @brief Extrae y formatea las producciones asociadas a un símbolo no terminal.
 * 
 * Busca todas las reglas de producción en las que el símbolo no terminal 
 * indicado actúa como origen (parte izquierda). Agrupa los lados derechos 
 * correspondientes y los formatea en una única cadena separada por el 
 * símbolo '|'. Tiene en cuenta si la gramática está configurada como lineal 
 * por la izquierda o por la derecha para ordenar correctamente los terminales 
 * y no terminales en la salida.
 * 
 * @param simbolo_no_terminal Cadena de texto con el identificador del no terminal a buscar (ej. "A").
 * @return std::string Cadena formateada con todas las producciones del símbolo (ej. "A -> aB | b | &"), 
 *         o una cadena vacía si el símbolo no es válido o no pertenece a la gramática.
 */
std::string Gramatica::MostrarProdNoTerminal(const std::string& simbolo_no_terminal) const {
    std::string producciones{""};
    
    if (simbolo_no_terminal.empty()) {
        std::cout << "\nERROR: No se ha ingresado un símbolo no terminal." << "\n";
        return producciones;
    }
    
    bool existe_simbolo{false};
    for (const std::string& simbolo : simbolos_no_terminales) {
        if (simbolo_no_terminal == simbolo) {
            existe_simbolo = true;
        }
    }   

    if (!existe_simbolo) {
        std::cout << "\nERROR: No existe el símbolo no terminal ingresado." << "\n";
        return producciones;
    }

    std::vector<std::string> lados_derecho;
    for (const auto& produccion : producciones_) {
        std::string origen{produccion.first.first};
        if (origen != simbolo_no_terminal) {
            continue;
        }

        std::string simbolo{produccion.first.second};
        std::string destino{produccion.second};
        if (lineal_izquierda) {
            lados_derecho.emplace_back(std::string{destino + simbolo});
        } else {
            lados_derecho.emplace_back(std::string{simbolo + destino});
        }
    }

    producciones = simbolo_no_terminal + " -> ";
    for (size_t i{0}; i != lados_derecho.size(); ++i) {
        producciones += lados_derecho[i];
        if (i < lados_derecho.size() - 1) {
            producciones += " | ";
        }
    }

    return producciones;
}


/**
 * @brief Comprueba si una cadena pertenece al lenguaje generado por la gramática.
 * 
 * Simula el proceso de derivación iterando sobre los caracteres de la cadena.
 * Si la gramática es lineal por la derecha, lee la cadena en su orden natural 
 * (de izquierda a derecha). Si es lineal por la izquierda, procesa la cadena 
 * en orden inverso (de derecha a izquierda).
 * 
 * @param cadena La cadena de texto (std::string) a evaluar.
 * @return true Si la gramática genera la cadena y culmina en una producción vacía (&).
 * @return false Si no hay reglas de producción para la cadena o no termina en aceptación.
 */
bool Gramatica::ComprobarCadena(const std::string& cadena) const {
    if (lineal_izquierda == 0) {
        std::string simbolo_no_terminal{"S"};
        for (size_t i{0}; i != cadena.size(); ++i) {
            auto produccion{producciones_.find({simbolo_no_terminal, std::string(1, cadena[i])})};

            if (produccion != producciones_.end()) {
                simbolo_no_terminal = produccion->second;
            } else {
                return false;
            }
        }
        auto produccion_final{producciones_.find({simbolo_no_terminal, "&"})};

        if (produccion_final == producciones_.end()) {
            return false;
        }
    } else {
        std::string simbolo_no_terminal{"S"};
        
        for (int i = static_cast<int>(cadena.size()) - 1; i >= 0; --i) {
            auto produccion{producciones_.find({simbolo_no_terminal, std::string(1, cadena[i])})};

            if (produccion != producciones_.end()) {
                simbolo_no_terminal = produccion->second;
            } else {
                return false;
            }
        }
        auto produccion_final{producciones_.find({simbolo_no_terminal, "&"})};

        if (produccion_final == producciones_.end()) {
            return false;
        }
    }

    return true;
}
