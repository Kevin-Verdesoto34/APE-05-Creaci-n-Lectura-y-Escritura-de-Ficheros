#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

void agregarContacto() {
    string nom, tel, cor;
    cin.ignore();
    cout << "Nombre completo: "; getline(cin, nom);
    cout << "Telefono: "; getline(cin, tel);
    cout << "Correo: "; getline(cin, cor);

    ofstream file("agenda.txt", ios::app);
    if (file.is_open()) {
        file << nom << "," << tel << "," << cor << endl;
        file.close();
        cout << "Contacto guardado.\n";
    }
}

void buscarContacto() {
    string buscar;
    cin.ignore();
    cout << "Ingrese el nombre a buscar: "; getline(cin, buscar);
    ifstream file("agenda.txt");
    string linea;
    bool hallado = false;

    if (file.is_open()) {
        while (getline(file, linea)) {
            stringstream ss(linea);
            string n, t, c;
            getline(ss, n, ','); getline(ss, t, ','); getline(ss, c, ',');
            
            if (n.find(buscar) != string::npos) { // Búsqueda de subcadena
                cout << "Encontrado -> Nom: " << n << " | Tel: " << t << " | Correo: " << c << "\n";
                hallado = true;
            }
        }
        file.close();
        if (!hallado) cout << "No se encontraron coincidencias.\n";
    }
}

int main() {
    int op;
    do {
        cout << "\n--- AGENDA TELEFÓNICA ---\n1. Agregar Contacto\n2. Buscar Contacto\n3. Salir\nOpción: ";
        cin >> op;
        if (op == 1) agregarContacto();
        if (op == 2) buscarContacto();
    } while (op != 3);
    return 0;
}
