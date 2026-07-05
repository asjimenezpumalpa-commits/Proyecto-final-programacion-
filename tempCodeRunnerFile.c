#include <stdio.h>
#include "contaminacion.h"

int main() {
    Zona zonas[NUM_ZONAS];
    int op;

    inicializar(zonas);
    cargar(zonas);

    do {
        menu();
        scanf("%d", &op);

        switch(op) {
            case 1: ingresarDatos(zonas); break;
            case 2: monitorear(zonas); break;
            case 3: predecir(zonas); break;
            case 4: alertas(zonas); break;
            case 5: promedios(zonas); break;
            case 6: recomendaciones(zonas); break;
            case 7: exportar(zonas); break;
            case 0: guardar(zonas); break;
        }
    } while(op != 0);

    return 0;
}
