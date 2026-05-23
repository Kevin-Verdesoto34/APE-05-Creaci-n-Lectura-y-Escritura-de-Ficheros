#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

const std::string ARCHIVO_LIBROS = "biblioteca.txt";

class Libro {
public:
    Libro(const std::string &codigo, const std::string &titulo, const std::string &autor, const std::string &estado)
        : codigo(codigo), titulo(titulo), autor(autor), estado(estado) {}

    const std::string &getCodigo() const { return codigo; }
    const std::string &getTitulo() const { return titulo; }
    const std::string &getAutor() const { return autor; }
    const std::string &getEstado() const { return estado; }

    bool estaDisponible() const {
        return estado == "disponible";
    }

    void prestar() {
        estado = "prestado";
    }

    void devolver() {
        estado = "disponible";
    }

    std::string toRegistro() const {
        std::ostringstream out;
        out << codigo << "|" << titulo << "|" << autor << "|" << estado;
        return out.str();
    }

    std::string toString() const {
        std::ostringstream out;
        out << "Código: " << codigo << "\n"
            << "Título: " << titulo << "\n"
            << "Autor: " << autor << "\n"
            << "Estado: " << estado;
        return out.str();
    }

private:
    std::string codigo;
    std::string titulo;
    std::string autor;
    std::string estado;
};

int leerEntero(const std::string &mensaje) {
    while (true) {
        std::cout << mensaje;
        int valor;
        if (std::cin >> valor) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return valor;
        }
        std::cout << "Debe ingresar un número válido." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string leerTextoNoVacio(const std::string &mensaje) {
    while (true) {
        std::cout << mensaje;
        std::string texto;
        std::getline(std::cin, texto);
        if (!texto.empty()) {
            return texto;
        }
        std::cout << "El texto no puede estar vacío." << std::endl;
    }
}

std::vector<Libro> cargarLibros() {
    std::vector<Libro> libros;
    std::ifstream archivo(ARCHIVO_LIBROS);
    if (!archivo.is_open()) {
        return libros;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string codigo, titulo, autor, estado;
        if (std::getline(ss, codigo, '|') && std::getline(ss, titulo, '|') && std::getline(ss, autor, '|') && std::getline(ss, estado)) {
            libros.emplace_back(codigo, titulo, autor, estado);
        }
    }
    return libros;
}

void guardarLibros(const std::vector<Libro> &libros) {
    std::ofstream archivo(ARCHIVO_LIBROS);
    if (!archivo.is_open()) {
        std::cout << "Error al guardar libros." << std::endl;
        return;
    }
    for (const auto &libro : libros) {
        archivo << libro.toRegistro() << "\n";
    }
}

Libro *buscarLibro(std::vector<Libro> &libros, const std::string &codigo) {
    for (auto &libro : libros) {
        if (libro.getCodigo() == codigo) {
            return &libro;
        }
    }
    return nullptr;
}

void mostrarMenu() {
    std::cout << "=== Biblioteca Virtual ===" << std::endl;
    std::cout << "1. Agregar libro" << std::endl;
    std::cout << "2. Buscar libro" << std::endl;
    std::cout << "3. Mostrar libros disponibles" << std::endl;
    std::cout << "4. Mostrar libros prestados" << std::endl;
    std::cout << "5. Prestar libro" << std::endl;
    std::cout << "6. Devolver libro" << std::endl;
    std::cout << "7. Mostrar todos los libros" << std::endl;
    std::cout << "8. Salir" << std::endl;
}

void agregarLibro(std::vector<Libro> &libros) {
    std::string codigo = leerTextoNoVacio("Código del libro: ");
    if (buscarLibro(libros, codigo) != nullptr) {
        std::cout << "El código ya existe en la biblioteca." << std::endl;
        return;
    }
    std::string titulo = leerTextoNoVacio("Título del libro: ");
    std::string autor = leerTextoNoVacio("Autor del libro: ");
    libros.emplace_back(codigo, titulo, autor, "disponible");
    guardarLibros(libros);
    std::cout << "Libro agregado exitosamente." << std::endl;
}

void buscarLibro(const std::vector<Libro> &libros) {
    std::string codigo = leerTextoNoVacio("Ingrese el código del libro a buscar: ");
    for (const auto &libro : libros) {
        if (libro.getCodigo() == codigo) {
            std::cout << libro.toString() << std::endl;
            return;
        }
    }
    std::cout << "Libro no encontrado." << std::endl;
}

void mostrarDisponibles(const std::vector<Libro> &libros) {
    bool encontrado = false;
    std::cout << "=== Libros Disponibles ===" << std::endl;
    for (const auto &libro : libros) {
        if (libro.estaDisponible()) {
            std::cout << libro.toString() << std::endl << std::endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "No hay libros disponibles en este momento." << std::endl;
    }
}

void mostrarPrestados(const std::vector<Libro> &libros) {
    bool encontrado = false;
    std::cout << "=== Libros Prestados ===" << std::endl;
    for (const auto &libro : libros) {
        if (!libro.estaDisponible()) {
            std::cout << libro.toString() << std::endl << std::endl;
            encontrado = true;
        }
    }
    if (!encontrado) {
        std::cout << "No hay libros prestados actualmente." << std::endl;
    }
}

void prestarLibro(std::vector<Libro> &libros) {
    std::string codigo = leerTextoNoVacio("Ingrese el código del libro a prestar: ");
    Libro *libro = buscarLibro(libros, codigo);
    if (libro == nullptr) {
        std::cout << "Libro no encontrado." << std::endl;
    } else if (!libro->estaDisponible()) {
        std::cout << "El libro ya está prestado." << std::endl;
    } else {
        libro->prestar();
        guardarLibros(libros);
        std::cout << "Libro prestado exitosamente." << std::endl;
    }
}

void devolverLibro(std::vector<Libro> &libros) {
    std::string codigo = leerTextoNoVacio("Ingrese el código del libro a devolver: ");
    Libro *libro = buscarLibro(libros, codigo);
    if (libro == nullptr) {
        std::cout << "Libro no encontrado." << std::endl;
    } else if (libro->estaDisponible()) {
        std::cout << "El libro ya está disponible." << std::endl;
    } else {
        libro->devolver();
        guardarLibros(libros);
        std::cout << "Libro devuelto exitosamente." << std::endl;
    }
}

void mostrarTodos(const std::vector<Libro> &libros) {
    if (libros.empty()) {
        std::cout << "La biblioteca está vacía." << std::endl;
        return;
    }
    std::cout << "=== Todos los Libros ===" << std::endl;
    for (const auto &libro : libros) {
        std::cout << libro.toString() << std::endl << std::endl;
    }
}

int main() {
    std::vector<Libro> libros = cargarLibros();
    int opcion;
    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opción: ");
        switch (opcion) {
            case 1:
                agregarLibro(libros);
                break;
            case 2:
                buscarLibro(libros);
                break;
            case 3:
                mostrarDisponibles(libros);
                break;
            case 4:
                mostrarPrestados(libros);
                break;
            case 5:
                prestarLibro(libros);
                break;
            case 6:
                devolverLibro(libros);
                break;
            case 7:
                mostrarTodos(libros);
                break;
            case 8:
                std::cout << "Gracias por usar la Biblioteca Virtual. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
        std::cout << std::endl;
    } while (opcion != 8);
    return 0;
}
