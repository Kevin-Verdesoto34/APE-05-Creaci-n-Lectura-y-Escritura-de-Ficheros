#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>

const std::string ARCHIVO_VENTAS = "ventas.txt";
const std::string ARCHIVO_FACTURAS = "facturas.txt";

class Venta {
public:
    Venta(int numeroFactura, const std::string &producto, int cantidad, double precio)
        : numeroFactura(numeroFactura), producto(producto), cantidad(cantidad), precio(precio) {
        fecha = obtenerFechaActual();
    }

    int getNumeroFactura() const { return numeroFactura; }
    const std::string &getProducto() const { return producto; }
    int getCantidad() const { return cantidad; }
    double getPrecio() const { return precio; }
    const std::string &getFecha() const { return fecha; }

    void setFecha(const std::string &fechaValue) {
        fecha = fechaValue;
    }

    double calcularTotal() const {
        return cantidad * precio;
    }

    std::string generarFactura() const {
        std::ostringstream out;
        out << "+------------------------------------------------+\n";
        out << "¦            FACTURA DE VENTA\n";
        out << "+------------------------------------------------+\n";
        out << "Número de Factura: " << numeroFactura << "\n";
        out << "Fecha: " << fecha << "\n";
        out << "------------------------------------------------\n";
        out << "Producto: " << producto << "\n";
        out << "Cantidad: " << cantidad << "\n";
        out << "Precio Unitario: $" << precio << "\n";
        out << "------------------------------------------------\n";
        out << "TOTAL: $" << calcularTotal() << "\n";
        out << "------------------------------------------------\n";
        return out.str();
    }

    std::string toRegistro() const {
        std::ostringstream out;
        out << numeroFactura << "|" << producto << "|" << cantidad << "|" << precio << "|" << fecha;
        return out.str();
    }

private:
    int numeroFactura;
    std::string producto;
    int cantidad;
    double precio;
    std::string fecha;

    std::string obtenerFechaActual() const {
        std::time_t now = std::time(nullptr);
        char buffer[32];
        std::tm localTime;
#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&localTime, &now);
#else
        localtime_r(&now, &localTime);
#endif
        std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &localTime);
        return std::string(buffer);
    }
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

double leerDoublePositivo(const std::string &mensaje) {
    while (true) {
        std::cout << mensaje;
        double valor;
        if (std::cin >> valor) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (valor > 0) {
                return valor;
            }
            std::cout << "Debe ingresar un valor mayor que cero." << std::endl;
        } else {
            std::cout << "Debe ingresar un número válido." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
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

void guardarFactura(const std::string &factura) {
    std::ofstream archivo(ARCHIVO_FACTURAS, std::ios::app);
    if (!archivo) {
        std::cout << "Error al guardar factura." << std::endl;
        return;
    }
    archivo << factura << "\n" << std::string(50, '-') << "\n";
}

std::vector<Venta> cargarVentas(int &numeroFactura) {
    std::vector<Venta> ventas;
    std::ifstream archivo(ARCHIVO_VENTAS);
    if (!archivo) {
        return ventas;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string numeroStr, producto, cantidadStr, precioStr, fecha;

        if (std::getline(ss, numeroStr, '|') && std::getline(ss, producto, '|') &&
            std::getline(ss, cantidadStr, '|') && std::getline(ss, precioStr, '|') &&
            std::getline(ss, fecha)) {

            try {
                int numero = std::stoi(numeroStr);
                int cantidad = std::stoi(cantidadStr);
                double precio = std::stod(precioStr);
                Venta venta(numero, producto, cantidad, precio);
                venta.setFecha(fecha);
                ventas.push_back(venta);
                if (numero >= numeroFactura) {
                    numeroFactura = numero + 1;
                }
            } catch (...) {
                // ignorar línea malformada
            }
        }
    }

    return ventas;
}

void guardarVentas(const std::vector<Venta> &ventas) {
    std::ofstream archivo(ARCHIVO_VENTAS);
    if (!archivo) {
        std::cout << "Error al guardar ventas." << std::endl;
        return;
    }
    for (const auto &venta : ventas) {
        archivo << venta.toRegistro() << "\n";
    }
}

Venta *buscarVentaPorNumero(std::vector<Venta> &ventas, int numero) {
    for (auto &venta : ventas) {
        if (venta.getNumeroFactura() == numero) {
            return &venta;
        }
    }
    return nullptr;
}

std::string truncar(const std::string &texto, size_t longitud) {
    if (texto.size() <= longitud) {
        return texto;
    }
    return texto.substr(0, longitud - 2) + "..";
}

void mostrarMenu() {
    std::cout << "=== Sistema de Ventas ===" << std::endl;
    std::cout << "1. Registrar venta" << std::endl;
    std::cout << "2. Generar factura" << std::endl;
    std::cout << "3. Mostrar historial de ventas" << std::endl;
    std::cout << "4. Ver estadísticas" << std::endl;
    std::cout << "5. Salir" << std::endl;
}

void mostrarHistorialVentas(const std::vector<Venta> &ventas) {
    if (ventas.empty()) {
        std::cout << "No hay ventas registradas." << std::endl;
        return;
    }

    std::cout << "=== Historial de Ventas ===" << std::endl;
    std::cout << std::left << std::setw(5) << "ID" << std::setw(25) << "Producto"
              << std::setw(10) << "Cantidad" << std::setw(14) << "Precio Unit." << std::setw(14)
              << "Total" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (const auto &venta : ventas) {
        std::cout << std::left << std::setw(5) << venta.getNumeroFactura()
                  << std::setw(25) << truncar(venta.getProducto(), 23)
                  << std::setw(10) << venta.getCantidad()
                  << "$" << std::setw(13) << std::fixed << std::setprecision(2) << venta.getPrecio()
                  << "$" << std::setw(13) << std::fixed << std::setprecision(2) << venta.calcularTotal()
                  << std::endl;
    }
}

void mostrarEstadisticas(const std::vector<Venta> &ventas) {
    if (ventas.empty()) {
        std::cout << "No hay ventas registradas." << std::endl;
        return;
    }

    double totalVentas = 0.0;
    int totalProductos = 0;
    for (const auto &venta : ventas) {
        totalVentas += venta.calcularTotal();
        totalProductos += venta.getCantidad();
    }
    double promedio = totalVentas / ventas.size();

    std::cout << "=== Estadísticas de Ventas ===" << std::endl;
    std::cout << "Total de transacciones: " << ventas.size() << std::endl;
    std::cout << "Total de productos vendidos: " << totalProductos << std::endl;
    std::cout << "Monto total vendido: $" << std::fixed << std::setprecision(2) << totalVentas << std::endl;
    std::cout << "Venta promedio: $" << std::fixed << std::setprecision(2) << promedio << std::endl;
}

int main() {
    int numeroFactura = 1;
    std::vector<Venta> ventas = cargarVentas(numeroFactura);

    int opcion;
    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opción: ");
        switch (opcion) {
            case 1: {
                std::string producto = leerTextoNoVacio("Nombre del producto: ");
                int cantidad = leerEntero("Cantidad: ");
                double precio = leerDoublePositivo("Precio unitario: $");
                Venta venta(numeroFactura, producto, cantidad, precio);
                ventas.push_back(venta);
                numeroFactura++;
                guardarVentas(ventas);
                std::cout << "Venta registrada. Total: $" << std::fixed << std::setprecision(2)
                          << venta.calcularTotal() << std::endl;
                break;
            }
            case 2: {
                if (ventas.empty()) {
                    std::cout << "No hay ventas registradas." << std::endl;
                    break;
                }
                int numero = leerEntero("Número de factura: ");
                Venta *venta = buscarVentaPorNumero(ventas, numero);
                if (venta == nullptr) {
                    std::cout << "Factura no encontrada." << std::endl;
                } else {
                    std::string factura = venta->generarFactura();
                    guardarFactura(factura);
                    std::cout << factura << std::endl;
                    std::cout << "Factura guardada en " << ARCHIVO_FACTURAS << std::endl;
                }
                break;
            }
            case 3:
                mostrarHistorialVentas(ventas);
                break;
            case 4:
                mostrarEstadisticas(ventas);
                break;
            case 5:
                std::cout << "Gracias por usar el Sistema de Ventas. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
        std::cout << std::endl;
    } while (opcion != 5);

    guardarVentas(ventas);
    return 0;
}
