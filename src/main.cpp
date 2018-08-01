#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "scheduler.h"


int main(int args, char **argv){
    if(args != 5){
        std::cout << "Cantidad de Parámetros Incorrectos";
        return 1;
    }
    Scheduler scheduler = Scheduler(atoi(argv[2]),atoi(argv[3]), atof(argv[4]));
    if(scheduler.leerInstancia(argv[1])){
        return 1;
    };
    scheduler.constructorSolucion();
    //scheduler.printSolucion();
    //std::cout << "Asignados: " << scheduler.asignados.size() << "\n";
    //std::cout << "No Asignados: " << scheduler.noAsignados.size() << "\n";
    //std::cout << "Avg waiting time: " << scheduler.funcionObjetivo()/(100*scheduler.asignados.size())  <<"\n";
    //scheduler.metricas();
    //std::cout << "\n";
    scheduler.localSearch();
    //scheduler.printSolucion();
    std::cout << "Asignados: " << scheduler.asignados.size() << "\n";
    std::cout << "No Asignados: " << scheduler.noAsignados.size() << "\n";
    std::cout << "Avg waiting time: " << scheduler.funcionObjetivo()/(100*scheduler.asignados.size()) <<"\n";
    scheduler.metricas();
    return 0;
}
