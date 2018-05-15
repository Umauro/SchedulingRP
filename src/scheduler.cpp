#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>


#include "scheduler.h"
#include "paciente.h"

// Override () operator for sort Paciente's objects //
struct sortComparator{
    bool operator ()(Paciente const a, Paciente const b) const{
        return (a.release < b.release) ||
                ((a.release == b.release)  &&
                    ((a.categoria > b.categoria) ||
                        ((a.categoria == b.categoria) &&
                            (a.sesiones < b.sesiones))));
    }
};

Scheduler::Scheduler(){
    dias = 0;
    diasTrabajo = 0;
    turnos = 0;
    tiempo = 0;
    cantidadMaquina1 = 0;
    cantidadMaquina2 = 0;
    pacientes = std::vector<Paciente>();
}

int Scheduler::leerInstancia(std::string instancia){
    int e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12,
    e13, test;
    int idCounter = 1;
    std::ifstream archivo(instancia);
    if(!archivo.good()){
        std::cout << "Error al leer la instancia \n";
        return 1;
    }
    archivo >> e1 >> e2 >> e3 >> e4 >> e5 >> e6 >> e7;
    this -> dias = e1;
    this -> diasTrabajo = e2;
    this -> turnos = e3;
    this -> tiempo = e4;
    this -> cantidadMaquina1 = e6;
    this -> cantidadMaquina2 = e7;

    while(archivo >> test){
        for(int i = 0; i < test; i++){
            archivo >> e1 >> e2 >> e3 >> e4 >> e5 >> e6 >> e7 >> e8
            >> e9 >> e10 >> e11 >> e12 >> e13;

            Paciente paciente(idCounter, e1, e2, e3, e4, e5, e6, e7, e8,
                e9, e10, e11, e12, e13);
            pacientes.push_back(paciente);
            idCounter++;
        }
    }
    archivo.close();
    std::sort(pacientes.begin(), pacientes.end(), sortComparator());
    return 0;
}
