#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "paciente.h"

Paciente::Paciente(int ide, int categor, int rel,
     int du, int ses, int interrup, int tipoM,
      int tiempoPS, int tiempoS, int a1, int a2, int a3,
       int a4, int a5){
    this -> id = ide;
    this -> categoria = categor;
    this -> release = rel;
    this -> due = du;
    this -> sesiones = ses;
    this -> interrupciones = interrup;
    this -> tipoMaquina = tipoM;
    this -> tiempoPrimeraSesion = tiempoPS;
    this -> tiempoSesion = tiempoS;
    int disponibilidad[5] = {a1, a2, a3, a4, a5};
    this ->  sesionesRestantes = ses;
}
