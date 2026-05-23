#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

const std::string NOMBRE_ARCHIVO = "asistencia.txt";

int leerEntero(const std::string &mensaje) {
    int valor;
    while (true) {
        std::cout << mensaje;
        if (std::cin >> valor) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return valor;
        }
        std::cout << "Debe ingresar un numero valido." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string leerTextoNoVacio(const std::string &mensaje) {
    std::string texto;
    do {
        std::cout << mensaje;
        std::getline(std::cin, texto);
        if (texto.empty()) {
            std::cout << "El valor no puede estar vacio. Intente de nuevo." << std::endl;
        }
    } while (texto.empty());
    return texto;
}

std::string leerEstado() {
    std::string estado;
    do {
        std::cout << "Estado (P = presente, F = falta): ";
        std::getline(std::cin, estado);
        for (auto &c : estado) c = toupper(static_cast<unsigned char>(c));
        if (estado != "P" && estado != "F") {
            std::cout << "Estado invalido. Ingrese P o F." << std::endl;
        }
    } while (estado != "P" && estado != "F");
    return estado;
}

void guardarRegistro(const std::string &registro) {
    std::ofstream archivo(NOMBRE_ARCHIVO, std::ios::app);
    if (!archivo) {
        std::cout << "Error al abrir el archivo para guardar el registro." << std::endl;
        return;
    }
    archivo << registro << std::endl;
}

void registrarAsistencia() {
    std::string nombre = leerTextoNoVacio("Nombre del estudiante: ");
    std::string fecha = leerTextoNoVacio("Fecha (dd/mm/aaaa): ");
    std::string estado = leerEstado();
    std::string registro = nombre + "|" + fecha + "|" + estado;
    guardarRegistro(registro);
    std::cout << "Asistencia registrada correctamente." << std::endl;
}

void mostrarHistorial() {
    std::ifstream archivo(NOMBRE_ARCHIVO);
    if (!archivo) {
        std::cout << "No hay historial de asistencia registrado." << std::endl;
        return;
    }

    std::cout << "--- Historial de asistencia ---" << std::endl;
    std::string linea;
    int contador = 0;
    while (std::getline(archivo, linea)) {
        contador++;
        std::stringstream ss(linea);
        std::string nombre, fecha, estado;
        if (std::getline(ss, nombre, '|') && std::getline(ss, fecha, '|') && std::getline(ss, estado)) {
            std::string textoEstado = (estado == "P") ? "Presente" : "Falta";
            std::cout << contador << ". " << nombre << " - " << fecha << " - " << textoEstado << std::endl;
        } else {
            std::cout << contador << ". Registro invalido: " << linea << std::endl;
        }
    }
    if (contador == 0) {
        std::cout << "No hay registros en el archivo." << std::endl;
    }
}

void contarFaltas() {
    std::ifstream archivo(NOMBRE_ARCHIVO);
    if (!archivo) {
        std::cout << "No hay registros disponibles para contar faltas." << std::endl;
        return;
    }

    int faltas = 0;
    std::string linea;
    while (std::getline(archivo, linea)) {
        std::stringstream ss(linea);
        std::string nombre, fecha, estado;
        if (std::getline(ss, nombre, '|') && std::getline(ss, fecha, '|') && std::getline(ss, estado)) {
            if (estado == "F") {
                faltas++;
            }
        }
    }
    std::cout << "Total de faltas registradas: " << faltas << std::endl;
}

void mostrarMenu() {
    std::cout << "=== Control de Asistencia ===" << std::endl;
    std::cout << "1. Registrar asistencia" << std::endl;
    std::cout << "2. Mostrar historial" << std::endl;
    std::cout << "3. Contar faltas" << std::endl;
    std::cout << "4. Salir" << std::endl;
}

int main() {
    int opcion;
    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opcion: ");
        switch (opcion) {
            case 1:
                registrarAsistencia();
                break;
            case 2:
                mostrarHistorial();
                break;
            case 3:
                contarFaltas();
                break;
            case 4:
                std::cout << "Saliendo del programa. !Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opcion invalida. Intente de nuevo." << std::endl;
        }
        std::cout << std::endl;
    } while (opcion != 4);
    return 0;
}
