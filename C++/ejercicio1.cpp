#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Estudiante {
private:
    string cedula;
    string nombre;
    string carrera;
    double promedio;

public:
    Estudiante(string ced, string nom, string car, double prom) {
        cedula = ced;
        nombre = nom;
        carrera = car;
        promedio = prom;
    }

    string toString() {
        return cedula + "," + nombre + "," + carrera + "," + to_string(promedio);
    }
};

int main() {
    string ced, nom, car;
    double prom;

    cout << "--- REGISTRO DE ESTUDIANTES ---\n";
    cout << "Ingrese Cedula: "; getline(cin, ced);
    cout << "Ingrese Nombre: "; getline(cin, nom);
    cout << "Ingrese Carrera: "; getline(cin, car);
    cout << "Ingrese Promedio: "; cin >> prom;

    Estudiante est(ced, nom, car, prom);

    ofstream archivo;
    archivo.open("estudiantes.txt", ios::app); // Modo append (añadir)

    if (archivo.is_open()) {
        archivo << est.toString() << endl;
        archivo.close();
        cout << "Mensaje: Estudiante registrado y guardado correctamente.\n";
    } else {
        cout << "Error al abrir el archivo.\n";
    }

    return 0;
}
