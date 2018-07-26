#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>

#include "scheduler.h"
#include "paciente.h"

struct sortComparator;

class Scheduler{
public:
    int dias;
    int diasTrabajo;
    int turnos;
    int tiempo;
    int cantidadMaquina1;
    int cantidadMaquina2;
    std::vector<Paciente> pacientes;
    std::vector<Paciente> noAsignados;
    std::vector<int> schedule;
    std::vector<int> capacidadMaquinas;

    Scheduler();
    int leerInstancia(std::string instancia);
    bool compPrimeraCapacidad(int j, Paciente paciente);
    bool compCapacidad(int j, Paciente paciente);
    int diaAsigIncompleta(int release, Paciente paciente);
    int diaAsigCompleta(int release, Paciente paciente);
    void asignar(int diaAsig, Paciente paciente);
    void ASAP(Paciente paciente);
    void JIP(Paciente paciente);
    void constructorSolucion();
    float funcionObjetivo();
    void printSolucion();
};
#endif
