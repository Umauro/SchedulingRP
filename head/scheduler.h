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
    std::list<int> schedule;
    std::list<int> capacidadMaquinas;

    Scheduler();
    int leerInstancia(std::string instancia);
    void constructorSolucion();
};
#endif
