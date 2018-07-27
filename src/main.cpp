#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>

#include "scheduler.h"


int main(int args, char **argv){
    if(args != 2){
        std::cout << "Cantidad de Parámetros Incorrectos";
        return 1;
    }
    Scheduler scheduler = Scheduler();
    if(scheduler.leerInstancia(argv[1])){
        return 1;
    };
    scheduler.constructorSolucion();
    scheduler.printSolucion();
    std::cout << "Asignados: " << scheduler.asignados.size() << "\n";
    std::cout << "No Asignados: " << scheduler.noAsignados.size() << "\n";
    std::cout << "Avg waiting time: " << scheduler.funcionObjetivo() <<"\n";
    return 0;
}
