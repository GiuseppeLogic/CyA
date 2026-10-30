// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramática Regular a partir de un DFA
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 29/10/2026
// Archivo: automata_determinista.cc
// Descripción: Implementación de los métodos de la clase Automata, incluyendo
//              la lectura de la especificación desde un fichero .dfa.

#include "gramatica_regular.h"

/**
 * @brief Sobrecarga del operador de igualdad (==) para comparar dos autómatas.
 * 
 * Dos autómatas finitos deterministas (DFA) se consideran matemáticamente 
 * equivalentes si, una vez minimizados, comparten exactamente la misma estructura.
 * La función realiza copias locales para minimizar sin alterar los objetos originales,
 * comparando posteriormente todos sus componentes internos.
 * 
 * @param automata_1 Referencia constante al primer autómata a comparar.
 * @param automata_2 Referencia constante al segundo autómata a comparar.
 */
bool operator==(const Automata& automata_1, const Automata& automata_2) {
    Automata copia_1 = automata_1;
    Automata copia_2 = automata_2;

    copia_1.MinimizarDFA();
    copia_2.MinimizarDFA();
    
    return (copia_1.cantidad_estados_ == copia_2.cantidad_estados_) &&
           (copia_1.estado_arranque_ == copia_2.estado_arranque_) &&
           (copia_1.alfabeto_ == copia_2.alfabeto_) &&
           (copia_1.estados_aceptacion_ == copia_2.estados_aceptacion_) &&
           (copia_1.funcion_transicion_ == copia_2.funcion_transicion_);
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
 * @brief Constructor de la clase Automata a partir de un fichero de texto.
 * 
 * Intenta abrir el fichero especificado y utiliza el operador de extracción
 * sobrecargado para inicializar los estados, alfabeto y transiciones.
 * 
 * @param nombre_fichero Ruta en formato string del fichero .dfa a procesar.
*/
Automata::Automata(const std::string& nombre_fichero) {
    automata_invalido_ = false;
    cantidad_estados_ = 0;
    estado_arranque_ = 0;

    std::ifstream archivo(nombre_fichero);
    
    if (!archivo.is_open()) {
        std::cerr << "ERROR: No se pudo abrir el fichero de especificación del DFA: " 
                  << nombre_fichero << "\n";
        automata_invalido_ = true;
        return;
    }

    archivo >> *this;
    
    archivo.close();
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
 * @brief Comprueba si dos estados dados son equivalentes a nivel directo o de auto-bucle.
 * 
 * @param estado_1 Identificador del primer estado.
 * @param estado_2 Identificador del segundo estado.
 * @return true Si tienen el mismo comportamiento inmediato.
 */
bool Automata::EstadosEquivalentes(int estado_1, int estado_2) const {
    bool estado_1_aceptacion = (estados_aceptacion_.count(estado_1) > 0);
    bool estado_2_aceptacion = (estados_aceptacion_.count(estado_2) > 0);
    
    if (estado_1_aceptacion != estado_2_aceptacion){
        return false;
    }

    for (const std::string& simbolo : alfabeto_) {
        auto trans_1 = funcion_transicion_.find({estado_1, simbolo});
        auto trans_2 = funcion_transicion_.find({estado_2, simbolo});

        int dest_1 = (trans_1 != funcion_transicion_.end()) ? trans_1->second : -1;
        int dest_2 = (trans_2 != funcion_transicion_.end()) ? trans_2->second : -1;

        if (dest_1 != dest_2) {
            bool bucle_propio = (dest_1 == estado_1 && dest_2 == estado_2);
            
            bool bucle_cruzado = (dest_1 == estado_2 && dest_2 == estado_1);

            if (!bucle_propio && !bucle_cruzado) {
                return false; 
            }
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
 * @brief Minimiza el Autómata Finito Determinista (DFA) fusionando estados equivalentes.
 * 
 * El algoritmo utiliza el método EsMinimo() para encontrar todos los pares de 
 * estados equivalentes. Luego, agrupa estos estados asignándoles un único estado 
 * "representante". Finalmente, reconstruye la función de transición, el conjunto 
 * de estados de aceptación y el estado de arranque eliminando los estados redundantes 
 * y reasignando identificadores contiguos (0, 1, 2...) a los estados resultantes.
 */
void Automata::MinimizarDFA() {
    // 1. Obtener los pares de estados equivalentes
    std::vector<std::pair<int,int>> equivalentes = EsMinimo();

    // Si el vector está vacío, el DFA ya es mínimo, no hay nada que hacer
    if (equivalentes.empty()) {
        return;
    }

    // 2. Crear un arreglo de "representantes" para cada estado
    // Inicialmente, cada estado se representa a sí mismo
    std::vector<int> representante(cantidad_estados_);
    for (int i = 0; i < cantidad_estados_; ++i) {
        representante[i] = i;
    }

    // Fusionar los estados equivalentes
    for (const auto& par : equivalentes) {
        int p = par.first;
        int q = par.second;
        
        // Nos aseguramos de quedarnos con el identificador más pequeño como representante
        if (p > q) std::swap(p, q);
        
        int rep_p = representante[p];
        
        // Todos los estados que estaban siendo representados por 'q'
        // ahora pasan a ser representados por el representante de 'p'.
        for (int i = 0; i < cantidad_estados_; ++i) {
            if (representante[i] == q) {
                representante[i] = rep_p;
            }
        }
    }

    // 3. Crear nuevos identificadores contiguos (0, 1, 2...) para los estados únicos
    std::map<int, int> nuevo_id;
    int id_actual = 0;
    for (int i = 0; i < cantidad_estados_; ++i) {
        if (representante[i] == i) { // Si el estado es un representante oficial
            nuevo_id[i] = id_actual++;
        }
    }

    // 4. Construir las nuevas estructuras del autómata minimizado
    int nueva_cantidad_estados = id_actual;
    int nuevo_estado_arranque = nuevo_id[representante[estado_arranque_]];
    std::set<int> nuevos_estados_aceptacion;
    std::map<std::pair<int, std::string>, int> nueva_funcion_transicion;

    // Actualizar estados de aceptación
    for (int estado_final : estados_aceptacion_) {
        nuevos_estados_aceptacion.insert(nuevo_id[representante[estado_final]]);
    }

    // Actualizar función de transición
    for (const auto& par_transicion : funcion_transicion_) {
        int origen_original = par_transicion.first.first;
        std::string simbolo = par_transicion.first.second;
        int destino_original = par_transicion.second;

        // Redirigir el origen y el destino hacia sus nuevos identificadores representativos
        int nuevo_origen = nuevo_id[representante[origen_original]];
        int nuevo_destino = nuevo_id[representante[destino_original]];

        nueva_funcion_transicion[{nuevo_origen, simbolo}] = nuevo_destino;
    }

    // 5. Reemplazar los atributos del autómata con los minimizados
    cantidad_estados_ = nueva_cantidad_estados;
    estado_arranque_ = nuevo_estado_arranque;
    estados_aceptacion_ = nuevos_estados_aceptacion;
    funcion_transicion_ = nueva_funcion_transicion;
}


/**
 * @brief Convierte el autómata finito (DFA) actual en una Gramática Regular.
 * 
 * Implementa el algoritmo de transformación donde cada estado del DFA 
 * se convierte en un símbolo no terminal de la gramática. Las transiciones 
 * pasan a ser reglas de producción lineales por la derecha, y los estados 
 * de aceptación generan producciones vacías representadas por el símbolo '&'.
 * 
 * @return Gramatica Objeto instanciado de la clase Gramatica equivalente al DFA.
 */
Gramatica Automata::ConvertToGrammar() const {
    Gramatica gramatica;
    
    gramatica.alfabeto_ = this->alfabeto_; 
    
    gramatica.simbolos_no_terminales.clear();
    gramatica.producciones_.clear();

    int cantidad_estados = this->getter_cantidad_estados();
    int estado_arranque = this->getter_estado_arranque();

    std::vector<std::string> nombre_no_terminales(cantidad_estados);
    std::string alfabeto_latino{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    
    int indice_letra = 0;
    for (int i = 0; i < cantidad_estados; ++i) {
        if (i == estado_arranque) {
            nombre_no_terminales[i] = "S";
        } else {
            if (alfabeto_latino[indice_letra] == 'S') {
                indice_letra++; 
            }
            nombre_no_terminales[i] = std::string(1, alfabeto_latino[indice_letra++]);
        }
        gramatica.simbolos_no_terminales.emplace_back(nombre_no_terminales[i]);
    }
    
    gramatica.simbolo_arranque_ = "S";

    // Producciones { q -> a p | δ(q, a) = p }
    for (const auto& par_transicion : this->getter_funcion_transicion()) {
        int estado_origen = par_transicion.first.first;
        std::string simbolo = par_transicion.first.second;
        int estado_destino = par_transicion.second;

        std::string no_terminal_origen = nombre_no_terminales[estado_origen];
        std::string no_terminal_destino = nombre_no_terminales[estado_destino];

        gramatica.producciones_.insert({{no_terminal_origen, simbolo}, no_terminal_destino});
    }

    // Producciones { q -> ε | q ∈ F }
    for (int estado_final : this->getter_estado_aceptacion()) {
        std::string no_terminal_final = nombre_no_terminales[estado_final];
        gramatica.producciones_.insert({{no_terminal_final, "&"}, ""});
    }
    
    return gramatica;
}


