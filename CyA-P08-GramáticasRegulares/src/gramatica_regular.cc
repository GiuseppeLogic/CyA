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
                if (gramatica.lineal_izquierda){
                    lados_derechos.push_back(destino + simbolo);
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


/**
 * @brief Comprueba y muestra por consola si la gramática tiene producciones vacías.
 * 
 * Recorre el contenedor de producciones buscando aquellas cuyo símbolo 
 * terminal consumido sea el carácter de cadena vacía ('&').
 * 
 * @return true Si existe al menos una producción vacía.
 * @return false Si la gramática no contiene producciones vacías.
 */
bool Gramatica::MostrarProduccionesVacias() const {
    bool encontradas = false;
    std::cout << "\n--- Producciones Vacías ---\n";
    
    for (const auto& produccion : producciones_) {
        if (produccion.first.second == "&") {
            std::cout << produccion.first.first << " -> &\n";
            encontradas = true;
        }
    }
    
    if (!encontradas) {
        std::cout << "La gramática no tiene producciones vacías.\n";
    }
    return encontradas;
}


/**
 * @brief Detecta y muestra los símbolos y producciones inútiles de la gramática.
 * 
 * Un símbolo es inútil si no es alcanzable desde el axioma inicial (S) o si 
 * no es generador (no puede derivar, directa o indirectamente, en una cadena 
 * de terminales puros o en la cadena vacía).
 */
void Gramatica::MostrarSimbolosInutiles() const {
    // 1. ENCONTRAR SÍMBOLOS GENERADORES
    std::set<std::string> generadores;
    
    for (const auto& prod : producciones_) {
        if (prod.first.second == "&" || prod.second == "") {
            generadores.insert(prod.first.first);
        }
    }
    
    bool cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            // Si el destino es un generador, el origen también lo es
            if (prod.second != "" && generadores.count(prod.second) > 0) {
                if (generadores.count(prod.first.first) == 0) {
                    generadores.insert(prod.first.first);
                    cambios = true;
                }
            }
        }
    }

    // 2. ENCONTRAR SÍMBOLOS ALCANZABLES
    std::set<std::string> alcanzables;
    alcanzables.insert(simbolo_arranque_);
    
    cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            if (alcanzables.count(prod.first.first) > 0 && prod.second != "") {
                if (alcanzables.count(prod.second) == 0) {
                    alcanzables.insert(prod.second);
                    cambios = true;
                }
            }
        }
    }

    // 3. DETERMINAR SÍMBOLOS INÚTILES
    std::set<std::string> inutiles;
    for (const std::string& sim : simbolos_no_terminales) {
        if (generadores.count(sim) == 0 || alcanzables.count(sim) == 0) {
            inutiles.insert(sim);
        }
    }

    // 4. IMPRIMIR RESULTADOS
    std::cout << "\n--- Símbolos Inútiles ---\n";
    if (inutiles.empty()) {
        std::cout << "No hay símbolos inútiles. La gramática está limpia.\n";
    } else {
        for (const auto& sim : inutiles) {
            std::cout << "Símbolo " << sim << " es inútil (";
            if (generadores.count(sim) == 0) {
                std::cout << "No generador";
            } else if (alcanzables.count(sim) == 0) {
                std::cout << "No alcanzable";
            }
            std::cout << ").\n";
        }
    }
}


/**
 * @brief Simula y muestra por consola la derivación paso a paso de una cadena.
 * 
 * Imprime la traza de la derivación utilizando el símbolo '=>'. Mantiene 
 * el registro de los terminales consumidos (como prefijo o sufijo según 
 * la dirección de la gramática) y la variable no terminal activa.
 * 
 * @param cadena Cadena de texto a evaluar.
 */
void Gramatica::DerivacionCadena(const std::string& cadena) const {
    std::string traza = simbolo_arranque_;
    std::string simbolo_actual = simbolo_arranque_;
    
    std::cout << "\nTraza de derivación para '" << (cadena.empty() ? "&" : cadena) << "':\n";
    std::cout << traza;
    
    if (lineal_izquierda == 0) {
        std::string prefijo = "";
        for (size_t i = 0; i < cadena.size(); ++i) {
            auto produccion = producciones_.find({simbolo_actual, std::string(1, cadena[i])});
            if (produccion != producciones_.end()) {
                simbolo_actual = produccion->second;
                prefijo += cadena[i];
                traza += " => " + prefijo + simbolo_actual;
                std::cout << " => " << prefijo << simbolo_actual;
            } else {
                std::cout << "\n[!] ERROR: Bloqueo. No hay transición para '" << cadena[i] << "'.\n";
                return;
            }
        }
        
        if (producciones_.count({simbolo_actual, "&"})) {
            std::cout << " => " << (prefijo.empty() ? "&" : prefijo) << "\n[+] Cadena ACEPTADA.\n";
        } else {
            std::cout << "\n[!] ERROR: El no terminal '" << simbolo_actual << "' no tiene producción vacía (&).\n";
        }
    } else {
        std::string sufijo = "";
        for (int i = static_cast<int>(cadena.size()) - 1; i >= 0; --i) {
            auto produccion = producciones_.find({simbolo_actual, std::string(1, cadena[i])});
            if (produccion != producciones_.end()) {
                simbolo_actual = produccion->second;
                sufijo = cadena[i] + sufijo;
                traza += " => " + simbolo_actual + sufijo;
                std::cout << " => " << simbolo_actual << sufijo;
            } else {
                std::cout << "\n[!] ERROR: Bloqueo. No hay transición para '" << cadena[i] << "'.\n";
                return;
            }
        }
        
        if (producciones_.count({simbolo_actual, "&"})) {
            std::cout << " => " << (sufijo.empty() ? "&" : sufijo) << "\n[+] Cadena ACEPTADA.\n";
        } else {
            std::cout << "\n[!] ERROR: El no terminal '" << simbolo_actual << "' no tiene producción vacía (&).\n";
        }
    }
}


/**
 * @brief Comprueba si el lenguaje generado por la gramática es infinito.
 * 
 * Determina si la gramática genera un número ilimitado de palabras. 
 * Para que un lenguaje regular sea infinito, debe existir un ciclo útil en sus 
 * reglas. Esto se traduce matemáticamente en la existencia de un símbolo no 
 * terminal que sea alcanzable desde el inicio, que sea capaz de generar la cadena
 * vacía (cerrar la palabra) y que pueda alcanzarse a sí mismo en uno o más pasos.
 * 
 * @return true Si el lenguaje es infinito.
 * @return false Si el lenguaje es finito o está vacío.
 */
bool Gramatica::EsLenguajeInfinito() const {
    // 1. Calcular conjunto de símbolos GENERADORES (pueden terminar la palabra)
    std::set<std::string> generadores;
    for (const auto& prod : producciones_) {
        if (prod.first.second == "&" || prod.second == "") {
            generadores.insert(prod.first.first);
        }
    }
    bool cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            if (prod.second != "" && generadores.count(prod.second) > 0) {
                if (generadores.insert(prod.first.first).second) {
                    cambios = true;
                }
            }
        }
    }

    // 2. Calcular conjunto de símbolos ALCANZABLES (desde S)
    std::set<std::string> alcanzables;
    alcanzables.insert(simbolo_arranque_);
    cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            if (alcanzables.count(prod.first.first) > 0 && prod.second != "") {
                if (alcanzables.insert(prod.second).second) {
                    cambios = true;
                }
            }
        }
    }

    // 3. Buscar un CICLO entre los símbolos que son tanto alcanzables como generadores
    for (const std::string& candidato : simbolos_no_terminales) {
        if (alcanzables.count(candidato) > 0 && generadores.count(candidato) > 0) {
            
            std::set<std::string> rama_alcanzable;
            
            for (const auto& prod : producciones_) {
                if (prod.first.first == candidato && prod.second != "") {
                    rama_alcanzable.insert(prod.second);
                }
            }
            
            bool propagando = true;
            while (propagando) {
                propagando = false;
                for (const auto& prod : producciones_) {
                    if (rama_alcanzable.count(prod.first.first) > 0 && prod.second != "") {
                        if (rama_alcanzable.insert(prod.second).second) {
                            propagando = true;
                        }
                    }
                }
            }
            
            if (rama_alcanzable.count(candidato) > 0) {
                return true; 
            }
        }
    }
    
    return false;
}


/**
 * @brief Simplifica la gramática eliminando los símbolos y producciones inútiles.
 * 
 * Calcula los conjuntos de símbolos generadores (que pueden derivar en cadenas 
 * terminales) y alcanzables (desde el axioma inicial). Todo símbolo que no 
 * pertenezca a la intersección de ambos conjuntos se considera inútil y es 
 * purgado tanto de la lista de no terminales como del mapa de producciones.
 */
void Gramatica::EliminarSimbolosInutiles() {
    // 1. ENCONTRAR SÍMBOLOS GENERADORES
    std::set<std::string> generadores;
    for (const auto& prod : producciones_) {
        if (prod.first.second == "&" || prod.second == "") {
            generadores.insert(prod.first.first);
        }
    }
    
    bool cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            if (prod.second != "" && generadores.count(prod.second) > 0) {
                if (generadores.insert(prod.first.first).second) {
                    cambios = true;
                }
            }
        }
    }

    // 2. ENCONTRAR SÍMBOLOS ALCANZABLES
    std::set<std::string> alcanzables;
    alcanzables.insert(simbolo_arranque_);
    
    cambios = true;
    while (cambios) {
        cambios = false;
        for (const auto& prod : producciones_) {
            if (alcanzables.count(prod.first.first) > 0 && prod.second != "") {
                if (alcanzables.insert(prod.second).second) {
                    cambios = true;
                }
            }
        }
    }

    // 3. IDENTIFICAR LOS INÚTILES
    std::set<std::string> inutiles;
    for (const std::string& sim : simbolos_no_terminales) {
        if (generadores.count(sim) == 0 || alcanzables.count(sim) == 0) {
            inutiles.insert(sim);
        }
    }

    if (inutiles.empty()) {
        std::cout << "\nNo se encontraron símbolos inútiles para eliminar.\n";
        return;
    }

    // 4. PURGAR DEL VECTOR DE NO TERMINALES
    std::vector<std::string> nuevos_simbolos;
    for (const std::string& sim : simbolos_no_terminales) {
        if (inutiles.count(sim) == 0) {
            nuevos_simbolos.push_back(sim);
        }
    }
    simbolos_no_terminales = nuevos_simbolos;

    // 5. PURGAR DEL MAPA DE PRODUCCIONES
    // Usamos un iterador manual porque borrar elementos en un contenedor 
    // mientras se itera sobre él con un for-each normal provoca cuelgues.
    for (auto it = producciones_.begin(); it != producciones_.end(); ) {
        std::string origen = it->first.first;
        std::string destino = it->second;

        // Si el origen es inútil, o si el destino es un no terminal inútil
        if (inutiles.count(origen) > 0 || (destino != "" && inutiles.count(destino) > 0)) {
            it = producciones_.erase(it); // erase devuelve el iterador al siguiente elemento válido
        } else {
            ++it;
        }
    }

    std::cout << "\n[+] Simplificación completada. Se eliminaron " 
              << inutiles.size() << " símbolos inútiles.\n";
}