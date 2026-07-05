#ifndef CONTAMINACION_H
#define CONTAMINACION_H

#define NUM_ZONAS 5
#define DIAS_HIST 30

#define LIMITE_CO2 400.0
#define LIMITE_SO2 20.0
#define LIMITE_NO2 40.0
#define LIMITE_PM25 15.0

typedef struct {
    float temperatura;
    float viento;
    float humedad;
} Clima;

typedef struct {
    float co2;
    float so2;
    float no2;
    float pm25;
} Contaminantes;

typedef struct {
    char nombre[30];
    Contaminantes actual;
    Contaminantes prediccion;
    Contaminantes historico[DIAS_HIST];
    Clima clima;
} Zona;

/* Funciones del sistema */
void menu();
void inicializar(Zona *z);
void ingresarDatos(Zona *z);
void monitorear(Zona *z);
void predecir(Zona *z);
void alertas(Zona *z);
void promedios(Zona *z);
void recomendaciones(Zona *z);
void exportar(Zona *z);
void cargar(Zona *z);
void guardar(Zona *z);

float promedioPonderado(float *datos, int n);

#endif
