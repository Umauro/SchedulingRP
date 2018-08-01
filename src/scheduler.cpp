#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <random>
#include <cstdlib>

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

struct sortNoAsignado{
    bool operator ()(Paciente const a, Paciente const b) const{
        return (a.sesiones > b.sesiones) ||
                ((a.sesiones == b.sesiones) &&
                    ((a.release > b.release) ||
                        ((a.release == b.release) &&
                            (a.categoria < b.categoria))));
    }
};

struct sortAsignado{
    bool operator ()(Paciente const a, Paciente const b) const{
        return (a.sesiones < b.sesiones) ||
                ((a.sesiones == b.sesiones) &&
                    ((a.tiempoEspera < b.tiempoEspera) &&
                        ((a.tiempoEspera == b.tiempoEspera) &&
                            (a.release > b.release))));
    }
};

Scheduler::Scheduler(int iteraciones, int parametro1, float probabilidad){
    iter = iteraciones;
    param1 = parametro1;
    paramProb = probabilidad;
    dias = 0;
    diasTrabajo = 0;
    turnos = 0;
    tiempo = 0;
    cantidadMaquina1 = 0;
    cantidadMaquina2 = 0;
    cantidadUrgent = 0;
    cantidadPalliative = 0;
    cantidadRadical = 0;
    pacientes = std::vector<Paciente>();
    noAsignados = std::vector<Paciente>();
    asignados = std::vector<Paciente>();
}

int Scheduler::leerInstancia(std::string instancia){
    int e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12,
    e13, test;
    int idCounter = 1;
    int urgentCounter = 0;
    int palliativeCounter = 0;
    int radicalCounter = 0;
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
            if(e1 == 1){
                urgentCounter++;
            }
            else if(e1 == 2){
                palliativeCounter++;
            }
            else{
                radicalCounter++;
            }
            idCounter++;
        }
    }
    archivo.close();
    cantidadUrgent = urgentCounter;
    cantidadPalliative = palliativeCounter;
    cantidadRadical = radicalCounter;
    return 0;
}

bool Scheduler::compPrimeraCapacidad(int j, Paciente &paciente, std::vector<int> &capacidades){
    if(paciente.tipoMaquina == 2){
        if((capacidades[j + dias] - (paciente.tiempoSesion+paciente.tiempoPrimeraSesion)) < 0){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if((capacidades[j] - (paciente.tiempoPrimeraSesion +paciente.tiempoSesion)) < 0){
            return false;
        }
        else{
            return true;
        }
    }
}

bool Scheduler::compCapacidad(int j, Paciente &paciente, std::vector<int> &capacidades){
    if(paciente.tipoMaquina == 2){
        if((capacidades[j + dias] - paciente.tiempoSesion) < 0){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if((capacidades[j] - paciente.tiempoSesion) < 0){
            return false;
        }
        else{
            return true;
        }
    }
}

int Scheduler::diaAsigIncompleta(int release, Paciente &paciente, std::vector<int> &capacidades){
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
                if(compPrimeraCapacidad(j, paciente, capacidades)){
                    primera = false;
                }
                else{
                    flag = false;
                    break;
                }
            }
            else{
                if(compCapacidad(j, paciente, capacidades)){
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
int Scheduler::diaAsigCompleta(int release, Paciente &paciente, std::vector<int> &capacidades){
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
                if(compPrimeraCapacidad(j, paciente, capacidades)){
                    primera = false;
                }
                else{
                    flag = false;
                    break;
                }
            }
            else{
                if(compCapacidad(j, paciente, capacidades)){
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

void Scheduler::asignar(int diaAsig, Paciente &paciente, std::vector<int> &capacidades){
    int contador = 0;
    bool primera = true;
    for(int j = diaAsig; j < dias; j ++){
        if(contador == 0) paciente.inicio = j; //Se guarda el indice del inicio
        if(contador == paciente.sesiones){
            paciente.fin = j; //Se guarda el índice del fin
            break;
        }
        paciente.schedulePaciente[j] = 1;
        paciente.tiempoEspera = diaAsig - (paciente.release -1);
        contador++;
        if(paciente.tipoMaquina == 2){
            if(primera){
                capacidades[j + dias] -= (paciente.tiempoSesion+paciente.tiempoPrimeraSesion);
                primera = false;
            }
            else{
                capacidades[j + dias] -= paciente.tiempoSesion;
            }
        }
        else{
            if(primera){
                capacidades[j] -= (paciente.tiempoSesion+paciente.tiempoPrimeraSesion);
                primera = false;
            }
            else{
                capacidades[j] -= paciente.tiempoSesion;
            }
        }
    }
}


void Scheduler::ASAP(Paciente &paciente, std::vector<int> &capacidades, std::vector<Paciente> &asig, std::vector<Paciente> &noAsig){
    int release = paciente.release;
    int due = paciente.due;
    int diaAsig = 0;
    if(paciente.sesiones > (dias - due)){
        diaAsig = diaAsigIncompleta(release, paciente, capacidades);
        if(diaAsig){
            asignar(diaAsig, paciente, capacidades);
            asig.push_back(paciente);
        }
        else{
            noAsig.push_back(paciente);
        }
    }
    else{
        diaAsig = diaAsigCompleta(release, paciente, capacidades);
        if(diaAsig){
            asignar(diaAsig, paciente, capacidades);
            asig.push_back(paciente);
        }
        else{
            noAsig.push_back(paciente);
        }
    }
}

void Scheduler::JIP(Paciente paciente){
    std::cout << "JIP" << "\n";
}

float Scheduler::funcionObjetivo(){
    float suma = 0;
    for(auto &i:asignados){
        suma += i.tiempoEspera;
    }
    return (suma/asignados.size())*(100*asignados.size());
}

float Scheduler::funcionObjetivo(std::vector<Paciente> candidato){
    float suma = 0;
    for(auto &i:candidato){
        suma += i.tiempoEspera;
    }
    return (suma/candidato.size())*(100*asignados.size());
}

void Scheduler::constructorSolucion(){
    std::sort(pacientes.begin(), pacientes.end(), sortComparator());
    int largoLista = (cantidadMaquina1+cantidadMaquina2)*dias;
    capacidadMaquinas = std::vector<int>(largoLista, tiempo);
    for(auto &i:pacientes){
        ASAP(i, capacidadMaquinas, asignados, noAsignados);
    }
    mejorSolucion = funcionObjetivo();
}

void Scheduler::metricas(){
    float urgent = 0;
    float palliative = 0;
    float radical = 0;
    for(auto &i: noAsignados){
        if(i.categoria == 1){
            urgent += 1;
        }
        else if(i.categoria == 2){
            palliative += 1;
        }
        else{
            radical += 1;
        }
    }
    std::cout << "Waiting urgent: " << (urgent/cantidadUrgent)*100 << "% \n";
    std::cout << "Waiting palliative: " << (palliative/cantidadPalliative)*100 << "% \n";
    std::cout << "Waiting radical: " << (radical/cantidadRadical)*100 << "% \n";
    std::cout << "Total: " << ((urgent+palliative+radical)/pacientes.size())*100 << "% \n";
}

void Scheduler::recalculador(std::vector<int> &capacidades, Paciente &paciente){
    //Se actualizará el contador de capacidad de las máquinas desde el inicio al
    //fin del tratamiento de un paciente.
    //Se utiliza cuando se saca un paciente de la lista de asignados
    if(paciente.tipoMaquina == 2){
        capacidades[paciente.inicio + dias] += (paciente.tiempoSesion + paciente.tiempoPrimeraSesion);
        for(int i = paciente.inicio + 1; i < paciente.fin + 1; i++){
            capacidades[i + dias] += paciente.tiempoSesion;
        }
    }
    else{
        capacidades[paciente.inicio] += (paciente.tiempoSesion + paciente.tiempoPrimeraSesion);
        for(int i = paciente.inicio + 1; i < paciente.fin + 1; i++){
            capacidades[i] += paciente.tiempoSesion;
        }
    }
    //Reseteo del paciente que fue sacado
    paciente.schedulePaciente = std::vector<int>(dias, 0);
    paciente.inicio = -1;
    paciente.fin = -1;
}

void Scheduler::localSearch(){
    int randomNumber;
    float randomProb;
    float fitNuevo;
    std::random_device rd;
    std::mt19937 generador(rd());
    //std::default_random_engine generador;
    std::uniform_int_distribution<int> distribucion(1, param1);
    std::uniform_real_distribution<> prob(0,1.0);
    for(int iteracion = 0; iteracion < iter; iteracion++){
        std::vector<Paciente> nuevoAsignados = asignados;
        std::vector<Paciente> nuevoNoAsignados = noAsignados;
        std::vector<int> nuevaCapacidad = capacidadMaquinas;
        std::sort(nuevoAsignados.begin(), nuevoAsignados.end(), sortAsignado());
        randomNumber = distribucion(generador);
        for(int i = 0; i < randomNumber; i++){
            randomProb = prob(generador);
            if(randomProb < paramProb){
                std::shuffle(nuevoAsignados.end()-10,nuevoAsignados.end(), generador);
            }
            Paciente eliminado = nuevoAsignados.back();
            //std::cout<<"Eliminado: "<< eliminado.id << " "<< eliminado.inicio << " " << eliminado.sesiones << "\n";
            recalculador(nuevaCapacidad, eliminado);
            nuevoNoAsignados.push_back(eliminado);
            nuevoAsignados.pop_back();
        }
        //std::sort(nuevoNoAsignados.begin(), nuevoNoAsignados.end(), sortNoAsignado());
        randomProb = prob(generador);

            //usar ASAP para insertar.
        for(unsigned int j = 0; j < nuevoNoAsignados.size()/4;j++){
            std::uniform_int_distribution<int> salvavidas(0, nuevoNoAsignados.size() - 1);
            randomNumber = salvavidas(generador);
            Paciente porAsignar = nuevoNoAsignados[randomNumber];
            nuevoNoAsignados.erase(nuevoNoAsignados.begin()+randomNumber);
            //std::cout<<"Por asignar: " << porAsignar.release << " " << porAsignar.sesiones << "\n";
            ASAP(porAsignar, nuevaCapacidad, nuevoAsignados, nuevoNoAsignados);
        }

        fitNuevo = funcionObjetivo(nuevoAsignados);
        if(fitNuevo < mejorSolucion){
            mejorSolucion = fitNuevo;
            asignados = nuevoAsignados;
            noAsignados = nuevoNoAsignados;
            capacidadMaquinas = nuevaCapacidad;
        }
        //std::cout << iteracion << " " << mejorSolucion/(100*asignados.size()) << " " << asignados.size() << " \n";
    }

}

void Scheduler::printSolucion(){
    int counter = 0;
    for(auto &i:asignados){
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
    for(auto &i:noAsignados){
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
