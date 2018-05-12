#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "paciente.h"

class Scheduler{
public:
    int dias;
    int diasTrabajo;
    int turnos;
    int tiempo;
    int cantidadMaquina1;
    int cantidadMaquina2;
    std::vector<Paciente> pacientes;

    Scheduler();
    int leerInstancia(std::string instancia);
};
#endif
