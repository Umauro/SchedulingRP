#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <random>

#include "scheduler.h"
#include "paciente.h"

struct sortComparator;
struct sortNoAsignado;
struct sortAsignado;

class Scheduler{
public:
    int iter;
    int param1;
    float paramProb;
    int dias;
    int diasTrabajo;
    int turnos;
    int tiempo;
    int cantidadMaquina1;
    int cantidadMaquina2;
    int cantidadUrgent;
    int cantidadPalliative;
    int cantidadRadical;
    std::vector<Paciente> pacientes;
    std::vector<Paciente> noAsignados;
    std::vector<Paciente> asignados;
    std::vector<int> schedule;
    std::vector<int> capacidadMaquinas;
    float mejorSolucion;

    Scheduler(int iteraciones, int parametro1, float probabilidad);
    int leerInstancia(std::string instancia);
    bool compPrimeraCapacidad(int j, Paciente &paciente);
    bool compCapacidad(int j, Paciente &paciente);
    int diaAsigIncompleta(int release, Paciente &paciente);
    int diaAsigCompleta(int release, Paciente &paciente);
    void asignar(int diaAsig, Paciente &paciente);
    void ASAP(Paciente &paciente);
    void JIP(Paciente paciente);
    float funcionObjetivo();
    void constructorSolucion();
    void metricas();
    void recalculador(std::vector<int> &capacidades, Paciente &paciente);
    void localSearch();
    void printSolucion();
};
#endif
