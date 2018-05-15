#ifndef PACIENTE_H
#define PACIENTE_H



class Paciente{
public:
    int id;
    int categoria;
    int release;
    int due;
    int sesiones;
    int interrupciones;
    int tipoMaquina;
    int tiempoPrimeraSesion;
    int tiempoSesion;
    int disponibilidad[5];
    int sesionesRestantes;

    Paciente(int ide, int categor, int rel, int du, int ses,
            int interrup, int tipoMaquina, int tiempoPS,
            int tiempoS, int a1, int a2, int a3, int a4, int a5);

};


#endif
