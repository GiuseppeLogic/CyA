// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 6: Simulador de DFAs
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 15/10/2026
// Archivo: automata_finito.h
// Descripción: Declaración de la clase Automata encargada de modelar la 
//              estructura de un Autómata Finito Determinista (DFA) y definir
//              sus métodos de verificación y simulación.

#ifndef AUTOMATA_FINITO_H
#define AUTOMATA_FINITO_H

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

class Automata{
  public:
    ~Automata(){};
    Automata(){};
    void ComprobarCadena(const std::string& cadena) const;
    bool getter_invalido() const{return automata_invalido_;};
    bool EsCompatible() const;
    std::vector<int> EstadosAbsorcion() const;
    std::vector<int> EstadosBucle() const;
    void HacerComplementario();
    std::vector<std::pair<int,int>> EsMinimo() const;
    friend std::ostream& operator<<(std::ostream& out, const std::pair<int,int>& par_estados);
    friend std::ostream& operator<<(std::ostream& out, const Automata& automata);
    friend std::ostream& operator<<(std::ostream& out, const std::set<std::string>& alfabeto);
    friend std::istream& operator>>(std::istream& inp, Automata& automata);

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