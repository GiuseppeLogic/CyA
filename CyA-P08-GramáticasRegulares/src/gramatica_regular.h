// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 8: Gramática Regular a partir de un DFA
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 29/10/2026
// Archivo: gramatica_regular.h
// Descripción: Declaración de la clase Gramatica y Automata.

#ifndef GRAMATICA_REGULAR_H
#define GRAMATICA_REGULAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <set>
#include <iomanip>
#include <sstream>

void MostrarUso();

void MostrarAyuda();


std::ostream& operator<<(std::ostream& out, const std::pair<int,int>& par_estados);
std::ostream& operator<<(std::ostream& out, const std::vector<int>& estados);
std::ostream& operator<<(std::ostream& out, const std::set<std::string>& alfabeto);

class Gramatica{
  public:
     Gramatica(){};
     ~Gramatica(){};

     std::string MostrarProdNoTerminal(const std::string& simbolo_no_terminal)  const;
     bool ComprobarCadena(const std::string& cadena) const;
     
     friend std::ostream& operator<<(std::ostream& out, const Gramatica& gramatica);
     friend std::ofstream& operator<<(std::ofstream& out, const Gramatica& gramatica);
     friend class Automata;

  private:
    std::set<std::string> alfabeto_;
    std::vector<std::string> simbolos_no_terminales;
    std::map<std::pair<std::string,std::string>,std::string> producciones_;
    std::string simbolo_arranque_{"S"};
    int lineal_izquierda{0};
};

class Automata{
  public:
    ~Automata(){};
    Automata(const std::string& nombre_fichero);

    bool getter_invalido() const{return automata_invalido_;};
    std::map<std::pair<int,std::string>,int> getter_funcion_transicion() const{return funcion_transicion_;};
    int getter_cantidad_estados() const {return cantidad_estados_;};
    std::set<std::string> getter_alfabeto() const {return alfabeto_;};
    int getter_estado_arranque() const {return estado_arranque_;};
    std::set<int> getter_estado_aceptacion() const{return estados_aceptacion_;};

    void ComprobarCadena(const std::string& cadena) const;
    bool EsCompatible() const;
    std::vector<int> EstadosAbsorcion() const;
    std::vector<int> EstadosBucle() const;
    std::vector<std::pair<int,int>> EsMinimo() const;
    void MinimizarDFA();
    Gramatica ConvertToGrammar() const;

    friend std::ostream& operator<<(std::ostream& out, const Automata& automata);
    friend std::istream& operator>>(std::istream& inp, Automata& automata);
    friend bool operator==( const Automata& automata_1, const Automata& automata_2);
    

  private:
    int cantidad_estados_;
    std::set<std::string> alfabeto_;
    int estado_arranque_{-1};
    std::set<int> estados_aceptacion_;
    std::map<std::pair<int,std::string>,int> funcion_transicion_;
    bool automata_invalido_{false};

    bool EstadosEquivalentes(int estado_1, int estado_2) const;

};



#endif 