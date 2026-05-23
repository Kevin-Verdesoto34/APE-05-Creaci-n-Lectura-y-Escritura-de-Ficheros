#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    ifstream archivo("estudiantes.txt");
    string linea;
    int contador = 0;
    double sumaPromedios = 0.0;

    cout << "--- REGISTROS DE ESTUDIANTES ---\n";

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            if (linea.empty()) continue;
            
            stringstream ss(linea);
            string cedula, nombre, carrera, promStr;
            
            getline(ss, cedula, ',');
            getline(ss, nombre, ',');
            getline(ss, carrera, ',');
            getline(ss, promStr, ',');

            double prom = stod(promStr);
            cout << "Estudiante: " << nombre << " | Carrera: " << carrera << " | Promedio: " << prom << "\n";
            
            sumaPromedios += prom;
            contador++;
        }
        archivo.close();

        cout << "--------------------------------\n";
        if (contador > 0) {
            cout << "Total Estudiantes: " << contador << "\n";
            cout << "Promedio General del Aula: " << (sumaPromedios / contador) << "\n";
        } else {
            cout << "No hay datos para procesar.\n";
        }
    } else {
        cout << "El archivo estudiantes.txt no existe o no se puede abrir.\n";
    }

    return 0;
}
