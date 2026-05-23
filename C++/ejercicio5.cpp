#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Evaluacion {
public:
    string nombre;
    double n1, n2, n3, promedio;
    string estado;

    Evaluacion(string nom, double nota1, double nota2, double nota3) {
        nombre = nom; n1 = nota1; n2 = nota2; n3 = nota3;
        promedio = (n1 + n2 + n3) / 3.0;
        estado = (promedio >= 7.0) ? "Aprobado" : "Reprobado";
    }

    string toString() {
        return nombre + "," + to_string(promedio) + "," + estado;
    }
};

int main() {
    string nom; double n1, n2, n3;
    cout << "--- SISTEMA DE CALIFICACIONES ---\n";
    cout << "Nombre del estudiante: "; getline(cin, nom);
    cout << "Nota 1: "; cin >> n1;
    cout << "Nota 2: "; cin >> n2;
    cout << "Nota 3: "; cin >> n3;

    Evaluacion eval(nom, n1, n2, n3);

    ofstream file("calificaciones.txt", ios::app);
    if (file.is_open()) {
        file << eval.toString() << endl;
        file.close();
        cout << "Notas registradas. Promedio: " << eval.promedio << " | Estado: " << eval.estado << "\n";
    }
    return 0;
}
