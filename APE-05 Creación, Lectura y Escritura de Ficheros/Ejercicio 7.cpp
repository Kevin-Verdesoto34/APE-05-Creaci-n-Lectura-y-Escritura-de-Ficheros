#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

const std::string ARCHIVO_CUENTAS = "cuentas.txt";

class CuentaUsuario {
public:
    CuentaUsuario(const std::string& usuario, double saldo)
        : usuario(usuario), saldo(saldo) {}

    const std::string& getUsuario() const { return usuario; }
    double getSaldo() const { return saldo; }

    void depositar(double monto) {
        if (monto > 0) {
            saldo += monto;
        }
    }

    bool retirar(double monto) {
        if (monto > 0 && monto <= saldo) {
            saldo -= monto;
            return true;
        }
        return false;
    }

    std::string toRegistro() const {
        std::ostringstream out;
        out << usuario << "|" << saldo;
        return out.str();
    }

private:
    std::string usuario;
    double saldo;
};

int leerEntero(const std::string& mensaje) {
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

double leerDoublePositivo(const std::string& mensaje) {
    while (true) {
        std::cout << mensaje;
        double valor;
        if (std::cin >> valor) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (valor > 0) {
                return valor;
            }
            std::cout << "Debe ingresar un monto mayor que cero." << std::endl;
        } else {
            std::cout << "Debe ingresar un número válido." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string leerTextoNoVacio(const std::string& mensaje) {
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

std::vector<CuentaUsuario> cargarCuentas() {
    std::vector<CuentaUsuario> cuentas;
    std::ifstream archivo(ARCHIVO_CUENTAS);
    if (!archivo.is_open()) {
        return cuentas;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string usuario;
        std::string saldoStr;
        if (std::getline(ss, usuario, '|') && std::getline(ss, saldoStr)) {
            try {
                double saldo = std::stod(saldoStr);
                cuentas.emplace_back(usuario, saldo);
            } catch (...) {
                // ignorar línea malformada
            }
        }
    }
    return cuentas;
}

void guardarCuentas(const std::vector<CuentaUsuario>& cuentas) {
    std::ofstream archivo(ARCHIVO_CUENTAS);
    if (!archivo.is_open()) {
        std::cout << "Error al guardar cuentas." << std::endl;
        return;
    }

    for (const auto& cuenta : cuentas) {
        archivo << cuenta.toRegistro() << '\n';
    }
}

CuentaUsuario* buscarCuenta(std::vector<CuentaUsuario>& cuentas, const std::string& usuario) {
    for (auto& cuenta : cuentas) {
        if (cuenta.getUsuario() == usuario) {
            return &cuenta;
        }
    }
    return nullptr;
}

void mostrarMenu() {
    std::cout << "=== Cajero Automático Básico ===" << std::endl;
    std::cout << "1. Consultar saldo" << std::endl;
    std::cout << "2. Depositar dinero" << std::endl;
    std::cout << "3. Retirar dinero" << std::endl;
    std::cout << "4. Salir" << std::endl;
}

int main() {
    std::vector<CuentaUsuario> cuentas = cargarCuentas();
    std::cout << "Bienvenido al Cajero Automático Básico." << std::endl;
    std::string usuario = leerTextoNoVacio("Ingrese su nombre de usuario: ");

    CuentaUsuario* cuenta = buscarCuenta(cuentas, usuario);
    if (cuenta == nullptr) {
        cuentas.emplace_back(usuario, 0.0);
        cuenta = &cuentas.back();
        std::cout << "Cuenta nueva creada con saldo inicial de $0.00." << std::endl;
    }

    int opcion;
    do {
        mostrarMenu();
        opcion = leerEntero("Seleccione una opción: ");
        switch (opcion) {
            case 1:
                std::cout << "Saldo actual de " << cuenta->getUsuario() << ": $" << cuenta->getSaldo() << std::endl;
                break;
            case 2: {
                double monto = leerDoublePositivo("Monto a depositar: $");
                cuenta->depositar(monto);
                guardarCuentas(cuentas);
                std::cout << "Depósito exitoso, nuevo saldo: $" << cuenta->getSaldo() << std::endl;
                break;
            }
            case 3: {
                double monto = leerDoublePositivo("Monto a retirar: $");
                if (cuenta->retirar(monto)) {
                    guardarCuentas(cuentas);
                    std::cout << "Retiro exitoso, nuevo saldo: $" << cuenta->getSaldo() << std::endl;
                } else {
                    std::cout << "Fondos insuficientes." << std::endl;
                }
                break;
            }
            case 4:
                std::cout << "Gracias por usar el cajero. Hasta luego." << std::endl;
                break;
            default:
                std::cout << "Opción inválida. Intente de nuevo." << std::endl;
        }
        std::cout << std::endl;
    } while (opcion != 4);

    return 0;
}
