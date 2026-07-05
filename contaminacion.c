#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contaminacion.h"

void menu() {
    printf("\n--- MENU ---\n");
    printf("1. Ingresar datos actuales\n");
    printf("2. Monitorear contaminacion actual\n");
    printf("3. Predecir contaminacion 24h\n");
    printf("4. Alertas preventivas\n");
    printf("5. Promedios historicos (30 dias)\n");
    printf("6. Recomendaciones\n");
    printf("7. Exportar reporte\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

void inicializar(Zona *z) {
    const char *nombres[] = {
        "Zona Norte", "Zona Sur", "Zona Centro", "Zona Este", "Zona Oeste"
    };

    for(int i = 0; i < NUM_ZONAS; i++) {
        strcpy(z[i].nombre, nombres[i]);

        z[i].clima.temperatura = 20.0;
        z[i].clima.viento = 5.0;
        z[i].clima.humedad = 60.0;

        for(int j = 0; j < DIAS_HIST; j++) {
            z[i].historico[j].co2  = 300 + rand() % 200;
            z[i].historico[j].so2  = 10  + rand() % 20;
            z[i].historico[j].no2  = 20  + rand() % 30;
            z[i].historico[j].pm25 = 10  + rand() % 15;
        }

        z[i].actual = z[i].historico[DIAS_HIST - 1];
    }
}

void ingresarDatos(Zona *z) {
    int n;
    printf("\nSeleccione zona:\n");
    for(int i = 0; i < NUM_ZONAS; i++)
        printf("%d. %s\n", i+1, z[i].nombre);

    scanf("%d", &n);
    if(n < 1 || n > NUM_ZONAS) return;
    n--;

    printf("CO2: ");  scanf("%f", &z[n].actual.co2);
    printf("SO2: ");  scanf("%f", &z[n].actual.so2);
    printf("NO2: ");  scanf("%f", &z[n].actual.no2);
    printf("PM2.5: ");scanf("%f", &z[n].actual.pm25);

    printf("Temperatura: "); scanf("%f", &z[n].clima.temperatura);
    printf("Viento: ");      scanf("%f", &z[n].clima.viento);
    printf("Humedad: ");     scanf("%f", &z[n].clima.humedad);

    for(int i = 0; i < DIAS_HIST - 1; i++)
        z[n].historico[i] = z[n].historico[i + 1];

    z[n].historico[DIAS_HIST - 1] = z[n].actual;

    printf("Datos ingresados correctamente\n");
}

void monitorear(Zona *z) {
    printf("\n--- MONITOREO ACTUAL ---\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        printf("\n%s\n", z[i].nombre);
        printf("CO2: %.2f\n", z[i].actual.co2);
        printf("SO2: %.2f\n", z[i].actual.so2);
        printf("NO2: %.2f\n", z[i].actual.no2);
        printf("PM2.5: %.2f\n", z[i].actual.pm25);
    }
}

float promedioPonderado(float *datos, int n) {
    float suma = 0, peso = 0;
    for(int i = 0; i < n; i++) {
        float p = (float)(i + 1) / n;
        suma += datos[i] * p;
        peso += p;
    }
    return suma / peso;
}

void predecir(Zona *z) {
    printf("\n--- PREDICCION 24 HORAS ---\n");

    for(int i = 0; i < NUM_ZONAS; i++) {
        float co2[DIAS_HIST], so2[DIAS_HIST], no2[DIAS_HIST], pm[DIAS_HIST];

        for(int j = 0; j < DIAS_HIST; j++) {
            co2[j] = z[i].historico[j].co2;
            so2[j] = z[i].historico[j].so2;
            no2[j] = z[i].historico[j].no2;
            pm[j]  = z[i].historico[j].pm25;
        }

        float fv = 1.0 - z[i].clima.viento / 50.0;
        if(fv < 0.5) fv = 0.5;

        z[i].prediccion.co2  = promedioPonderado(co2, DIAS_HIST) * fv;
        z[i].prediccion.so2  = promedioPonderado(so2, DIAS_HIST) * fv;
        z[i].prediccion.no2  = promedioPonderado(no2, DIAS_HIST) * fv;
        z[i].prediccion.pm25 = promedioPonderado(pm, DIAS_HIST)  * fv;

        printf("\n%s\n", z[i].nombre);
        printf("CO2: %.2f\n", z[i].prediccion.co2);
        printf("SO2: %.2f\n", z[i].prediccion.so2);
        printf("NO2: %.2f\n", z[i].prediccion.no2);
        printf("PM2.5: %.2f\n", z[i].prediccion.pm25);
    }
}

void alertas(Zona *z) {
    printf("\n--- ALERTAS ---\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        int alerta = 0;
        if(z[i].actual.co2 > LIMITE_CO2) alerta++;
        if(z[i].actual.so2 > LIMITE_SO2) alerta++;
        if(z[i].actual.no2 > LIMITE_NO2) alerta++;
        if(z[i].actual.pm25 > LIMITE_PM25) alerta++;

        if(alerta > 0)
            printf("%s: ALERTA (%d contaminantes)\n", z[i].nombre, alerta);
    }
}

void promedios(Zona *z) {
    printf("\n--- PROMEDIOS HISTORICOS ---\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        float co2=0, so2=0, no2=0, pm=0;
        for(int j = 0; j < DIAS_HIST; j++) {
            co2 += z[i].historico[j].co2;
            so2 += z[i].historico[j].so2;
            no2 += z[i].historico[j].no2;
            pm  += z[i].historico[j].pm25;
        }
        printf("%s -> CO2: %.2f\n", z[i].nombre, co2/DIAS_HIST);
    }
}

void recomendaciones(Zona *z) {
    printf("\n--- RECOMENDACIONES ---\n");
    for(int i = 0; i < NUM_ZONAS; i++) {
        if(z[i].actual.co2 > LIMITE_CO2)
            printf("%s: Reducir trafico y actividades al aire libre\n", z[i].nombre);
    }
}

void exportar(Zona *z) {
    FILE *f = fopen("reporte.txt", "w");
    if(!f) return;

    for(int i = 0; i < NUM_ZONAS; i++) {
        fprintf(f, "%s\n", z[i].nombre);
        fprintf(f, "CO2 actual: %.2f\n\n", z[i].actual.co2);
    }
    fclose(f);
    printf("Reporte generado\n");
}

void cargar(Zona *z) {
    FILE *f = fopen("datos.dat", "rb");
    if(f) {
        fread(z, sizeof(Zona), NUM_ZONAS, f);
        fclose(f);
    }
}

void guardar(Zona *z) {
    FILE *f = fopen("datos.dat", "wb");
    if(f) {
        fwrite(z, sizeof(Zona), NUM_ZONAS, f);
        fclose(f);
    }
}
