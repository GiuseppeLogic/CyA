// Universidad de La Laguna
// Escuela Superior de Ingeniería y Tecnología
// Grado en Ingeniería Informática
// Asignatura: Computabilidad y Algoritmia
// Curso: 2º
// Práctica 4: Expresiones regulares en C++
// Autor: Giuseppe Fuentes Moreno
// Correo: alu0101644080@ull.edu.es
// Fecha: 29/09/2026
// Archivo: expresion_regular.h
// Descripción: Contiene la definición de la clase ParseoHTML, las estructuras 
//              auxiliares (Comentario y Atributo) y las declaraciones de las 
//              funciones de ayuda al usuario.

#ifndef EXPRESION_REGULAR_H
#define EXPRESION_REGULAR_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <regex>
#include <fstream>

void MostrarUso();

void MostrarAyuda();

struct Comentario{
  int linea_comienzo;
  int linea_final;
  std::string contenido;
};

struct Atributo{
  int linea;
  std::string etiqueta;
  std::vector<std::string> atributos;
};

struct ContenidoEtiqueta{
  int linea;
  std::string etiqueta;
  std::string contenido;
};

class ParseoHTML{
  public:
    ParseoHTML(){};
    ~ParseoHTML(){};
    friend std::ostream& operator<<(std::ostream& out, const ParseoHTML& html);
    void ParsearDocumento(const std::string& ruta_documento);
  private:

    void ExtraerEstructura(const std::string& linea, int num_linea);
    void ExtraerEtiquetas(const std::string& linea, int num_linea);
    void ExtraerAtributos(const std::string& linea, int num_linea);
    void ProcesarComentarios(const std::string& linea, int num_linea);
    void ContarEtiquetas(const std::string& linea);
    void ExtraerEnlaces(const std::string& linea);
    void ExtraerTitulo(const std::string& linea);
    void ExtraerContenido(const std::string& linea, int num_linea);

    std::string programa_;
    std::string descripcion_;
    std::string titulo_;
    std::map<std::string, std::string> estructura_;
    std::multimap<int,std::string> etiquetas_;
    std::vector<Atributo> atributos_;
    std::vector<Comentario> comentarios_;
    std::map<std::string,int> frecuencia_etiquetas_;
    std::vector<std::string> enlaces_;
    std::vector<ContenidoEtiqueta> contenido_etiquetas_;

    bool en_comentario_ = false;
    Comentario comentario_actual_;
    int linea_doctype_ = -1;
};

#endif 