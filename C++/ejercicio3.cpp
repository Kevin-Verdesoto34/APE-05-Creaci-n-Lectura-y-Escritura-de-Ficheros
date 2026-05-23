#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class Producto {
private:
    string codigo, nombre;
    int cantidad;
    double precio;

public:
    Producto(string cod, string nom, int cant, double prec) {
        codigo = cod; nombre = nom; cantidad = cant; precio = prec;
    }
    string toString() {
        return codigo + "," + nombre + "," + to_string(cantidad) + "," + to_string(precio);
    }
};

void registrarProducto() {
    string cod, nom; int cant; double prec;
    cout << "Codigo del producto: "; cin >> cod;
    cin.ignore();
    cout << "Nombre del producto: "; getline(cin, nom);
    cout << "Cantidad: "; cin >> cant;
    cout << "Precio: "; cin >> prec;

    Producto prod(cod, nom, cant, prec);
    ofstream file("inventario.txt", ios::app);
    if (file.is_open()) {
        file << prod.toString() << endl;
        file.close();
        cout << "Producto añadido al inventario.\n";
    }
}

void calcularValorTotal() {
    ifstream file("inventario.txt");
    string linea;
    double granTotal = 0.0;

    if (file.is_open()) {
        while (getline(file, linea)) {
            if (linea.empty()) continue;
            stringstream ss(linea);
            string c, n, cantStr, precStr;
            getline(ss, c, ','); getline(ss, n, ',');
            getline(ss, cantStr, ','); getline(ss, precStr, ',');
            
            granTotal += (stoi(cantStr) * stod(precStr));
        }
        file.close();
        cout << "Valor total financiero del inventario: $" << granTotal << "\n";
    } else {
        cout << "No hay inventario registrado aún.\n";
    }
}

int main() {
    int op;
    do {
        cout << "\n--- INVENTARIO BÁSICO ---\n1. Registrar Producto\n2. Calcular Total Inventario\n3. Salir\nOpcion: ";
        cin >> op;
        if (op == 1) registrarProducto();
        if (op == 2) calcularValorTotal();
    } while (op != 3);
    return 0;
}
