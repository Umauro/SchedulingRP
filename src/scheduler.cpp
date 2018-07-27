#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>


#include "scheduler.h"
#include "paciente.h"

// Override () operator for sorting //
struct sortComparator{
    bool operator ()(Paciente const a, Paciente const b) const{
        return (a.release < b.release) ||
                ((a.release == b.release)  &&
                    ((a.categoria > b.categoria) ||
                        ((a.categoria == b.categoria) &&
                            (a.sesiones > b.sesiones))));
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
    noAsignados = std::vector<Paciente>();
    asignados = std::vector<Paciente>();
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
                e9, e10, e11, e12, e13, dias);
            pacientes.push_back(paciente);
            idCounter++;
        }
    }
    archivo.close();
    return 0;
}

bool Scheduler::compPrimeraCapacidad(int j, Paciente &paciente){
    if(paciente.tipoMaquina == 2){
        if((capacidadMaquinas[j + dias] - (paciente.tiempoSesion+paciente.tiempoPrimeraSesion)) < 0){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if((capacidadMaquinas[j] - (paciente.tiempoPrimeraSesion +paciente.tiempoSesion)) < 0){
            return false;
        }
        else{
            return true;
        }
    }
}

bool Scheduler::compCapacidad(int j, Paciente &paciente){
    if(paciente.tipoMaquina == 2){
        if((capacidadMaquinas[j + dias] - paciente.tiempoSesion) < 0){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if((capacidadMaquinas[j] - paciente.tiempoSesion) < 0){
            return false;
        }
        else{
            return true;
        }
    }
}

int Scheduler::diaAsigIncompleta(int release, Paciente &paciente){
    bool flag;
    bool primera;
    for(int i = (release - 1) ; i < dias; i++){
        flag = true;
        primera = true;
        for(int j = i; j < dias; j ++){
            if(primera){
                if(j % 5 == 4){
                    flag = false;
                    break;
                }
                if(compPrimeraCapacidad(j, paciente)){
                    primera = false;
                }
                else{
                    flag = false;
                    break;
                }
            }
            else{
                if(compCapacidad(j, paciente)){
                    primera = false;
                }
                else{
                    flag = false;
                    break;
                }
            }
        }
        if(flag){
            return i;
        }
    }
    //std::cout << "id no asignado: " << paciente.id << "\n";
    return 0;
}
int Scheduler::diaAsigCompleta(int release, Paciente &paciente){
    bool flag;
    bool primera;
    for(int i = (release - 1) ; i < paciente.due; i++){
        flag = true;
        primera = true;
        for(int j = i; j < i + paciente.sesiones; j ++){
            if(primera){
                if(j % 5 == 4){
                    flag = false;
                    break;
                }
                if(compPrimeraCapacidad(j, paciente)){
                    if(paciente.id == 3){
                        std::cout << "tiro falso \n";
                    }
                    primera = false;
                }
                else{
                    flag = false;
                    if(paciente.id == 3){
                        std::cout << "tiro falso P\n";
                    }
                    break;
                }
            }
            else{
                if(compCapacidad(j, paciente)){
                    if(paciente.id == 3){
                        std::cout << "tiro falso \n";
                    }
                    primera = false;
                }
                else{
                    flag = false;
                    if(paciente.id == 3){
                        std::cout << "tiro falso \n";
                    }
                    break;
                }
            }
        }
        if(flag){
            return i;
        }
    }
    //std::cout << "id no asignado: " << paciente.id << "\n";
    return 0;
}

void Scheduler::asignar(int diaAsig, Paciente &paciente){
    int contador = 0;
    bool primera = true;
    for(int j = diaAsig; j < dias; j ++){
        if(contador == paciente.sesiones) break;
        paciente.schedulePaciente[j] = 1;
        paciente.tiempoEspera = diaAsig - (paciente.release -1);
        contador++;
        if(paciente.tipoMaquina == 2){
            if(primera){
                capacidadMaquinas[j + dias] -= (paciente.tiempoSesion+paciente.tiempoPrimeraSesion);
                primera = false;
            }
            else{
                capacidadMaquinas[j + dias] -= paciente.tiempoSesion;
            }
        }
        else{
            if(primera){
                capacidadMaquinas[j] -= (paciente.tiempoSesion+paciente.tiempoPrimeraSesion);
                primera = false;
            }
            else{
                capacidadMaquinas[j] -= paciente.tiempoSesion;
            }
        }
    }
}


void Scheduler::ASAP(Paciente &paciente){
    int release = paciente.release;
    int due = paciente.due;
    int diaAsig = 0;
    if(paciente.sesiones > (dias - due)){
        diaAsig = diaAsigIncompleta(release, paciente);
        if(diaAsig){
            asignar(diaAsig, paciente);
            asignados.push_back(paciente);
        }
        else{
            noAsignados.push_back(paciente);
        }
    }
    else{
        diaAsig = diaAsigCompleta(release, paciente);
        if(diaAsig){
            asignar(diaAsig, paciente);
            asignados.push_back(paciente);
        }
        else{
            noAsignados.push_back(paciente);
        }
    }
}

void Scheduler::JIP(Paciente paciente){
    std::cout << "JIP" << "\n";
}

void Scheduler::constructorSolucion(){
    std::sort(pacientes.begin(), pacientes.end(), sortComparator());
    int largoLista = (cantidadMaquina1+cantidadMaquina2)*dias;
    capacidadMaquinas = std::vector<int>(largoLista, tiempo);
    schedule = std::vector<int>(pacientes.size()*dias,0);
    for(auto &i:pacientes){
        ASAP(i);
    }
}

float Scheduler::funcionObjetivo(){
    float suma = 0;
    for(auto &i:asignados){
        suma += i.tiempoEspera;
    }
    return (suma/asignados.size());
}

void Scheduler::printSolucion(){
    int counter = 0;
    for(auto &i:pacientes){
        std::cout << i.id <<":  ";
        for(unsigned int j = 0; j < i.schedulePaciente.size();j++){
            counter++;
            std::cout << i.schedulePaciente[j];
            if(counter % diasTrabajo == 0){
                std::cout << " ";
            }
        }
        std::cout << " \n";
    }
    std::cout << "\n";
    std::cout << "\n" << "Capacidades Máquinas \n";
    counter = 0;
    for(auto &i: capacidadMaquinas){
        if(counter % dias == 0){
            std::cout << "\n";
        }
        std::cout << i << " ";
        counter++;
    }
    std::cout << "\n";
}
