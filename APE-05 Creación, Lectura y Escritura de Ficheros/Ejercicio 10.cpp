#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <regex>
#include <algorithm>

const std::string ARCHIVO_EMPLEADOS = "empleados.txt";

class Empleado {
public:
    Empleado(int id, const std::string &nombre, const std::string &puesto,
             const std::string &email, const std::string &telefono,
             double salario, const std::string &departamento)
        : id(id), nombre(nombre), puesto(puesto), email(email), telefono(telefono),
          salario(salario), departamento(departamento) {}

    int getId() const { return id; }
    const std::string &getNombre() const { return nombre; }
    const std::string &getPuesto() const { return puesto; }
    const std::string &getEmail() const { return email; }
    const std::string &getTelefono() const { return telefono; }
    double getSalario() const { return salario; }
    const std::string &getDepartamento() const { return departamento; }

    void setNombre(const std::string &value) { nombre = value; }
    void setPuesto(const std::string &value) { puesto = value; }
    void setEmail(const std::string &value) { email = value; }
    void setTelefono(const std::string &value) { telefono = value; }
    void setSalario(double value) { salario = value; }
    void setDepartamento(const std::string &value) { departamento = value; }

    std::string toRegistro() const {
        std::ostringstream out;
        out << id << "|" << nombre << "|" << puesto << "|" << email << "|"
            << telefono << "|" << salario << "|" << departamento;
        return out.str();
    }

    std::string toString() const {
        std::ostringstream out;
        out << "ID: " << id << "\n"
            << "Nombre: " << nombre << "\n"
            << "Puesto: " << puesto << "\n"
            << "Email: " << email << "\n"
            << "Teléfono: " << telefono << "\n"
            << "Salario: $" << std::fixed << std::setprecision(2) << salario << "\n"
            << "Departamento: " << departamento;
        return out.str();
    }

private:
    int id;
    std::string nombre;
    std::string puesto;
    std::string email;
    std::string telefono;
    double salario;
    std::string departamento;
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

double leerDoubleNoNegativo(const std::string &mensaje) {
    while (true) {
        std::cout << mensaje;
        double valor;
        if (std::cin >> valor) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (valor >= 0) {
                return valor;
            }
            std::cout << "Debe ingresar un valor no negativo." << std::endl;
        } else {
            std::cout << "Debe ingresar un numero válido." << std::endl;
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

std::string leerEmail(const std::string &mensaje) {
    std::regex patron(R"(([^@\s]+)@([^@\s]+)\.([^@\s]+))");
    while (true) {
        std::string email = leerTextoNoVacio(mensaje);
        if (std::regex_match(email, patron)) {
            return email;
        }
        std::cout << "Email inválido. Debe contener @ y un dominio." << std::endl;
    }
}

std::string leerTelefono(const std::string &mensaje) {
    std::regex patron(R"(^\d{7,}$)");
    while (true) {
        std::string telefono = leerTextoNoVacio(mensaje);
        if (std::regex_match(telefono, patron)) {
            return telefono;
        }
        std::cout << "Teléfono inválido. Debe tener al menos 7 dígitos." << std::endl;
    }
}

std::vector<Empleado> cargarEmpleados(int &ultimoId) {
    std::vector<Empleado> empleados;
    std::ifstream archivo(ARCHIVO_EMPLEADOS);
    if (!archivo.is_open()) {
        return empleados;
    }

    std::string linea;
    while (std::getline(archivo, linea)) {
        std::istringstream ss(linea);
        std::string idStr, nombre, puesto, email, telefono, salarioStr, departamento;
        if (std::getline(ss, idStr, '|') && std::getline(ss, nombre, '|') &&
            std::getline(ss, puesto, '|') && std::getline(ss, email, '|') &&
            std::getline(ss, telefono, '|') && std::getline(ss, salarioStr, '|') &&
            std::getline(ss, departamento)) {
            try {
                int id = std::stoi(idStr);
                double salario = std::stod(salarioStr);
                empleados.emplace_back(id, nombre, puesto, email, telefono, salario, departamento);
                if (id > ultimoId) {
                    ultimoId = id;
                }
            } catch (...) {
                // ignorar línea malformada
            }
        }
    }

    return empleados;
}

void guardarEmpleados(const std::vector<Empleado> &empleados) {
    std::ofstream archivo(ARCHIVO_EMPLEADOS);
    if (!archivo.is_open()) {
        std::cout << "Error al guardar empleados." << std::endl;
        return;
    }

    for (const auto &empleado : empleados) {
        archivo << empleado.toRegistro() << "\n";
    }
}

Empleado *buscarPorId(std::vector<Empleado> &empleados, int id) {
    for (auto &empleado : empleados) {
        if (empleado.getId() == id) {
            return &empleado;
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

void mostrarMenuPrincipal() {
    std::cout << "╔════════════════════════════════════════════╗" << std::endl;
    std::cout << "║    SISTEMA DE GESTIÓN DE EMPLEADOS         ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════╝" << std::endl;
    std::cout << "1. Agregar empleado " << std::endl;
    std::cout << "2. Listar empleados" << std::endl;
    std::cout << "3. Buscar empleado" << std::endl;
    std::cout << "4. Actualizar empleado" << std::endl;
    std::cout << "5. Eliminar empleado" << std::endl;
    std::cout << "6. Ver estadísticas" << std::endl;
    std::cout << "7. Salir" << std::endl;
}

void listarTodosEmpleados(const std::vector<Empleado> &empleados) {
    if (empleados.empty()) {
        std::cout << "No hay empleados registrados." << std::endl;
        return;
    }

    std::cout << "\n=== Lista de Empleados ===" << std::endl;
    std::cout << std::left << std::setw(4) << "ID" << std::setw(20) << "Nombre" << std::setw(15)
              << "Puesto" << std::setw(20) << "Email" << std::setw(15) << "Teléfono" << std::setw(12)
              << "Salario" << std::setw(15) << "Depto." << std::endl;
    std::cout << std::string(110, '─') << std::endl;

    for (const auto &emp : empleados) {
        std::cout << std::left << std::setw(4) << emp.getId()
                  << std::setw(20) << truncar(emp.getNombre(), 18)
                  << std::setw(15) << truncar(emp.getPuesto(), 13)
                  << std::setw(20) << truncar(emp.getEmail(), 18)
                  << std::setw(15) << truncar(emp.getTelefono(), 13)
                  << "$" << std::setw(11) << std::fixed << std::setprecision(2) << emp.getSalario()
                  << std::setw(15) << truncar(emp.getDepartamento(), 13)
                  << std::endl;
    }
}

int main() {
    int ultimoId = 0;
    std::vector<Empleado> empleados = cargarEmpleados(ultimoId);
    int opcion;

    do {
        mostrarMenuPrincipal();
        opcion = leerEntero("Seleccione una opción: ");
        switch (opcion) {
            case 1: {
                std::cout << "\n--- Agregar Empleado ---" << std::endl;
                std::string nombre = leerTextoNoVacio("Nombre completo: ");
                std::string puesto = leerTextoNoVacio("Puesto: ");
                std::string email = leerEmail("Email: ");
                std::string telefono = leerTelefono("Teléfono: ");
                double salario = leerDoubleNoNegativo("Salario: $");
                std::string departamento = leerTextoNoVacio("Departamento: ");

                ultimoId++;
                empleados.emplace_back(ultimoId, nombre, puesto, email, telefono, salario, departamento);
                guardarEmpleados(empleados);
                std::cout << "✓ Empleado agregado con ID: " << ultimoId << std::endl;
                break;
            }
            case 2:
                listarTodosEmpleados(empleados);
                break;
            case 3: {
                int id = leerEntero("Ingrese el ID del empleado a buscar: ");
                Empleado *empleado = buscarPorId(empleados, id);
                if (empleado != nullptr) {
                    std::cout << "\n=== Información del Empleado ===" << std::endl;
                    std::cout << empleado->toString() << std::endl;
                } else {
                    std::cout << "Empleado no encontrado." << std::endl;
                }
                break;
            }
            case 4: {
                std::cout << "\n--- Actualizar Empleado ---" << std::endl;
                int id = leerEntero("Ingrese el ID del empleado a actualizar: ");
                Empleado *empleado = buscarPorId(empleados, id);
                if (empleado == nullptr) {
                    std::cout << "Empleado no encontrado." << std::endl;
                    break;
                }
                std::cout << "Datos actuales:" << std::endl;
                std::cout << empleado->toString() << std::endl;
                std::cout << "\nIngrese los nuevos datos (o presione Enter para mantener el actual):" << std::endl;

                std::string nombre;
                std::getline(std::cin, nombre);
                if (!nombre.empty()) empleado->setNombre(nombre);

                std::string puesto;
                std::getline(std::cin, puesto);
                if (!puesto.empty()) empleado->setPuesto(puesto);

                std::string email;
                std::getline(std::cin, email);
                if (!email.empty() && std::regex_match(email, std::regex(R"(([^@\s]+)@([^@\s]+)\.([^@\s]+))"))) {
                    empleado->setEmail(email);
                }

                std::string telefono;
                std::getline(std::cin, telefono);
                if (!telefono.empty() && std::regex_match(telefono, std::regex(R"(^\d{7,}$)"))) {
                    empleado->setTelefono(telefono);
                }

                std::cout << "Nuevo salario: $";
                std::string salarioStr;
                std::getline(std::cin, salarioStr);
                if (!salarioStr.empty()) {
                    try {
                        double salario = std::stod(salarioStr);
                        if (salario >= 0) empleado->setSalario(salario);
                    } catch (...) {
                    }
                }

                std::string departamento;
                std::getline(std::cin, departamento);
                if (!departamento.empty()) empleado->setDepartamento(departamento);

                guardarEmpleados(empleados);
                std::cout << "✓ Empleado actualizado exitosamente." << std::endl;
                break;
            }
            case 5: {
                int id = leerEntero("Ingrese el ID del empleado a eliminar: ");
                Empleado *empleado = buscarPorId(empleados, id);
                if (empleado == nullptr) {
                    std::cout << "Empleado no encontrado." << std::endl;
                    break;
                }
                std::cout << "¿Desea eliminar a: " << empleado->getNombre() << "? (s/n): ";
                std::string confirmacion;
                std::getline(std::cin, confirmacion);
                if (confirmacion == "s" || confirmacion == "S") {
                    empleados.erase(std::remove_if(empleados.begin(), empleados.end(),
                        [&](const Empleado &e) { return e.getId() == id; }), empleados.end());
                    guardarEmpleados(empleados);
                    std::cout << "✓ Empleado eliminado exitosamente." << std::endl;
                } else {
                    std::cout << "Operación cancelada." << std::endl;
                }
                break;
            }
            case 6: {
                if (empleados.empty()) {
                    std::cout << "No hay empleados registrados." << std::endl;
                    break;
                }
                double totalSalarios = 0.0;
                double salarioMaximo = 0.0;
                double salarioMinimo = std::numeric_limits<double>::max();
                std::string empleadoMayorSalario;
                std::string empleadoMenorSalario;
                for (const auto &emp : empleados) {
                    totalSalarios += emp.getSalario();
                    if (emp.getSalario() > salarioMaximo) {
                        salarioMaximo = emp.getSalario();
                        empleadoMayorSalario = emp.getNombre();
                    }
                    if (emp.getSalario() < salarioMinimo) {
                        salarioMinimo = emp.getSalario();
                        empleadoMenorSalario = emp.getNombre();
                    }
                }
                double promedio = totalSalarios / empleados.size();
                std::cout << "\n=== Estadísticas de Nómina ===" << std::endl;
                std::cout << "Total de empleados: " << empleados.size() << std::endl;
                std::cout << "Nómina total: $" << std::fixed << std::setprecision(2) << totalSalarios << std::endl;
                std::cout << "Salario promedio: $" << std::fixed << std::setprecision(2) << promedio << std::endl;
                std::cout << "Mayor salario: $" << std::fixed << std::setprecision(2) << salarioMaximo << " (" << empleadoMayorSalario << ")" << std::endl;
                std::cout << "Menor salario: $" << std::fixed << std::setprecision(2) << salarioMinimo << " (" << empleadoMenorSalario << ")" << std::endl;
                break;
            }
            case 7:
                std::cout << "Gracias por usar el Sistema. ¡Hasta luego!" << std::endl;
                break;
            default:
                std::cout << "Opción invalida. Intente de nuevo." << std::endl;
                break;
        }
        std::cout << std::endl;
    } while (opcion != 7);

    guardarEmpleados(empleados);
    return 0;
}