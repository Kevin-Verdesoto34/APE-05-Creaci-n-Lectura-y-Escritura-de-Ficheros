import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Ejercicio10 {

    private static final String ARCHIVO_EMPLEADOS = "empleados.txt";
    private static List<Empleado> empleados = new ArrayList<>();
    private static int ultimoId = 0;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        cargarEmpleados();

        int opcion;
        do {
            mostrarMenuPrincipal();
            opcion = leerEntero(scanner, "Seleccione una opción: ");
            switch (opcion) {
                case 1:
                    agregarEmpleado(scanner);
                    break;
                case 2:
                    listarTodosEmpleados();
                    break;
                case 3:
                    buscarEmpleado(scanner);
                    break;
                case 4:
                    actualizarEmpleado(scanner);
                    break;
                case 5:
                    eliminarEmpleado(scanner);
                    break;
                case 6:
                    mostrarEstadisticas();
                    break;
                case 7:
                    System.out.println("Gracias por usar el Sistema. ¡Hasta luego!");
                    break;
                default:
                    System.out.println("Opción inválida. Intente de nuevo.");
            }
            System.out.println();
        } while (opcion != 7);

        guardarEmpleados();
        scanner.close();
    }

    private static void mostrarMenuPrincipal() {
        System.out.println("╔════════════════════════════════════════════╗");
        System.out.println("║    SISTEMA DE GESTIÓN DE EMPLEADOS         ║");
        System.out.println("╚════════════════════════════════════════════╝");
        System.out.println("1. Agregar empleado (CREATE)");
        System.out.println("2. Listar empleados (READ)");
        System.out.println("3. Buscar empleado");
        System.out.println("4. Actualizar empleado (UPDATE)");
        System.out.println("5. Eliminar empleado (DELETE)");
        System.out.println("6. Ver estadísticas");
        System.out.println("7. Salir");
    }

    private static int leerEntero(Scanner scanner, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String linea = scanner.nextLine().trim();
            try {
                return Integer.parseInt(linea);
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un número válido.");
            }
        }
    }

    private static double leerDoublePositivo(Scanner scanner, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String linea = scanner.nextLine().trim();
            try {
                double valor = Double.parseDouble(linea);
                if (valor < 0) {
                    System.out.println("Debe ingresar un valor no negativo.");
                } else {
                    return valor;
                }
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un número válido.");
            }
        }
    }

    private static String leerTextoNoVacio(Scanner scanner, String mensaje) {
        while (true) {
            System.out.print(mensaje);
            String texto = scanner.nextLine().trim();
            if (!texto.isEmpty()) {
                return texto;
            }
            System.out.println("El texto no puede estar vacío.");
        }
    }

    private static String leerEmail(Scanner scanner, String mensaje) {
        while (true) {
            String email = leerTextoNoVacio(scanner, mensaje);
            if (email.contains("@") && email.contains(".")) {
                return email;
            }
            System.out.println("Email inválido. Debe contener @ y un dominio.");
        }
    }

    private static String leerTelefono(Scanner scanner, String mensaje) {
        while (true) {
            String telefono = leerTextoNoVacio(scanner, mensaje);
            if (telefono.matches("\\d{7,}")) {
                return telefono;
            }
            System.out.println("Teléfono inválido. Debe tener al menos 7 dígitos.");
        }
    }

    private static void agregarEmpleado(Scanner scanner) {
        System.out.println("\n--- Agregar Empleado ---");
        String nombre = leerTextoNoVacio(scanner, "Nombre completo: ");
        String puesto = leerTextoNoVacio(scanner, "Puesto: ");
        String email = leerEmail(scanner, "Email: ");
        String telefono = leerTelefono(scanner, "Teléfono: ");
        double salario = leerDoublePositivo(scanner, "Salario: $");
        String departamento = leerTextoNoVacio(scanner, "Departamento: ");

        ultimoId++;
        Empleado empleado = new Empleado(ultimoId, nombre, puesto, email, telefono, salario, departamento);
        empleados.add(empleado);
        guardarEmpleados();
        System.out.println("✓ Empleado agregado con ID: " + ultimoId);
    }

    private static void listarTodosEmpleados() {
        if (empleados.isEmpty()) {
            System.out.println("No hay empleados registrados.");
            return;
        }

        System.out.println("\n=== Lista de Empleados ===");
        System.out.printf("%-4s %-20s %-15s %-20s %-15s %-12s %-15s%n",
                "ID", "Nombre", "Puesto", "Email", "Teléfono", "Salario", "Depto.");
        System.out.println("─".repeat(110));

        for (Empleado emp : empleados) {
            System.out.printf("%-4d %-20s %-15s %-20s %-15s $%-11.2f %-15s%n",
                    emp.getId(),
                    truncar(emp.getNombre(), 18),
                    truncar(emp.getPuesto(), 13),
                    truncar(emp.getEmail(), 18),
                    truncar(emp.getTelefono(), 13),
                    emp.getSalario(),
                    truncar(emp.getDepartamento(), 13));
        }
    }

    private static void buscarEmpleado(Scanner scanner) {
        int id = leerEntero(scanner, "Ingrese el ID del empleado a buscar: ");
        Empleado empleado = buscarPorId(id);

        if (empleado != null) {
            System.out.println("\n=== Información del Empleado ===");
            System.out.println(empleado);
        } else {
            System.out.println("Empleado no encontrado.");
        }
    }

    private static Empleado buscarPorId(int id) {
        for (Empleado emp : empleados) {
            if (emp.getId() == id) {
                return emp;
            }
        }
        return null;
    }

    private static void actualizarEmpleado(Scanner scanner) {
        System.out.println("\n--- Actualizar Empleado ---");
        int id = leerEntero(scanner, "Ingrese el ID del empleado a actualizar: ");
        Empleado empleado = buscarPorId(id);

        if (empleado == null) {
            System.out.println("Empleado no encontrado.");
            return;
        }

        System.out.println("Datos actuales:");
        System.out.println(empleado);
        System.out.println("\nIngrese los nuevos datos (o presione Enter para mantener el actual):");

        String nombre = scanner.nextLine().trim();
        if (!nombre.isEmpty()) {
            empleado.setNombre(nombre);
        }

        String puesto = scanner.nextLine().trim();
        if (!puesto.isEmpty()) {
            empleado.setPuesto(puesto);
        }

        String email = scanner.nextLine().trim();
        if (!email.isEmpty() && email.contains("@")) {
            empleado.setEmail(email);
        }

        String telefono = scanner.nextLine().trim();
        if (!telefono.isEmpty() && telefono.matches("\\d{7,}")) {
            empleado.setTelefono(telefono);
        }

        System.out.print("Nuevo salario: $");
        String salarioStr = scanner.nextLine().trim();
        if (!salarioStr.isEmpty()) {
            try {
                double salario = Double.parseDouble(salarioStr);
                if (salario >= 0) {
                    empleado.setSalario(salario);
                }
            } catch (NumberFormatException e) {
                // Mantener salario anterior
            }
        }

        String departamento = scanner.nextLine().trim();
        if (!departamento.isEmpty()) {
            empleado.setDepartamento(departamento);
        }

        guardarEmpleados();
        System.out.println("✓ Empleado actualizado exitosamente.");
    }

    private static void eliminarEmpleado(Scanner scanner) {
        int id = leerEntero(scanner, "Ingrese el ID del empleado a eliminar: ");
        Empleado empleado = buscarPorId(id);

        if (empleado == null) {
            System.out.println("Empleado no encontrado.");
            return;
        }

        System.out.println("¿Desea eliminar a: " + empleado.getNombre() + "? (s/n): ");
        String confirmacion = scanner.nextLine().trim().toLowerCase();

        if (confirmacion.equals("s")) {
            empleados.remove(empleado);
            guardarEmpleados();
            System.out.println("✓ Empleado eliminado exitosamente.");
        } else {
            System.out.println("Operación cancelada.");
        }
    }

    private static void mostrarEstadisticas() {
        if (empleados.isEmpty()) {
            System.out.println("No hay empleados registrados.");
            return;
        }

        double totalSalarios = 0;
        double salarioMaximo = 0;
        double salarioMinimo = Double.MAX_VALUE;
        String empleadoMayorSalario = "";
        String empleadoMenorSalario = "";

        for (Empleado emp : empleados) {
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

        System.out.println("\n=== Estadísticas de Nómina ===");
        System.out.printf("Total de empleados: %d%n", empleados.size());
        System.out.printf("Nómina total: $%.2f%n", totalSalarios);
        System.out.printf("Salario promedio: $%.2f%n", promedio);
        System.out.printf("Mayor salario: $%.2f (%s)%n", salarioMaximo, empleadoMayorSalario);
        System.out.printf("Menor salario: $%.2f (%s)%n", salarioMinimo, empleadoMenorSalario);
    }

    private static String truncar(String texto, int longitud) {
        if (texto.length() <= longitud) {
            return texto;
        }
        return texto.substring(0, longitud - 2) + "..";
    }

    private static void cargarEmpleados() {
        File archivo = new File(ARCHIVO_EMPLEADOS);
        if (!archivo.exists()) {
            return;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                String[] partes = linea.split("\\|");
                if (partes.length == 7) {
                    int id = Integer.parseInt(partes[0]);
                    String nombre = partes[1];
                    String puesto = partes[2];
                    String email = partes[3];
                    String telefono = partes[4];
                    double salario = Double.parseDouble(partes[5]);
                    String departamento = partes[6];

                    Empleado empleado = new Empleado(id, nombre, puesto, email, telefono, salario, departamento);
                    empleados.add(empleado);

                    if (id > ultimoId) {
                        ultimoId = id;
                    }
                }
            }
        } catch (IOException | NumberFormatException e) {
            System.out.println("Error al cargar empleados: " + e.getMessage());
        }
    }

    private static void guardarEmpleados() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(ARCHIVO_EMPLEADOS))) {
            for (Empleado empleado : empleados) {
                writer.write(empleado.toRegistro());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar empleados: " + e.getMessage());
        }
    }
}

class Empleado {

    private final int id;
    private String nombre;
    private String puesto;
    private String email;
    private String telefono;
    private double salario;
    private String departamento;

    public Empleado(int id, String nombre, String puesto, String email, String telefono, double salario, String departamento) {
        this.id = id;
        this.nombre = nombre;
        this.puesto = puesto;
        this.email = email;
        this.telefono = telefono;
        this.salario = salario;
        this.departamento = departamento;
    }

    public int getId() {
        return id;
    }

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    public String getPuesto() {
        return puesto;
    }

    public void setPuesto(String puesto) {
        this.puesto = puesto;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public String getTelefono() {
        return telefono;
    }

    public void setTelefono(String telefono) {
        this.telefono = telefono;
    }

    public double getSalario() {
        return salario;
    }

    public void setSalario(double salario) {
        this.salario = salario;
    }

    public String getDepartamento() {
        return departamento;
    }

    public void setDepartamento(String departamento) {
        this.departamento = departamento;
    }

    public String toRegistro() {
        return id + "|" + nombre + "|" + puesto + "|" + email + "|" + telefono + "|" + salario + "|" + departamento;
    }

    @Override
    public String toString() {
        return String.format(
                "ID: %d%n" +
                "Nombre: %s%n" +
                "Puesto: %s%n" +
                "Email: %s%n" +
                "Teléfono: %s%n" +
                "Salario: $%.2f%n" +
                "Departamento: %s",
                id, nombre, puesto, email, telefono, salario, departamento);
    }
}

