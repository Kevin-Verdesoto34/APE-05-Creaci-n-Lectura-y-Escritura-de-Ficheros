import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class Ejercicio7 {

    private static final String ARCHIVO_CUENTAS = "cuentas.txt";

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        Map<String, CuentaUsuario> cuentas = cargarCuentas();

        System.out.println("=== Cajero Automático Básico ===");
        String usuario = leerTextoNoVacio(scanner, "Ingrese su nombre de usuario: ");
        CuentaUsuario cuenta = cuentas.getOrDefault(usuario, new CuentaUsuario(usuario, 0.0));
        cuentas.put(usuario, cuenta);

        int opcion;
        do {
            mostrarMenu();
            opcion = leerEntero(scanner, "Seleccione una opción: ");
            switch (opcion) {
                case 1:
                    System.out.printf("Saldo actual de %s: $%.2f%n", cuenta.getUsuario(), cuenta.getSaldo());
                    break;
                case 2:
                    realizarDeposito(scanner, cuenta);
                    guardarCuentas(cuentas);
                    break;
                case 3:
                    realizarRetiro(scanner, cuenta);
                    guardarCuentas(cuentas);
                    break;
                case 4:
                    System.out.println("Gracias por usar el cajero. Hasta luego.");
                    break;
                default:
                    System.out.println("Opción inválida. Intente de nuevo.");
            }
            System.out.println();
        } while (opcion != 4);

        scanner.close();
    }

    private static void mostrarMenu() {
        System.out.println("1. Consultar saldo");
        System.out.println("2. Depositar dinero");
        System.out.println("3. Retirar dinero");
        System.out.println("4. Salir");
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
                if (valor <= 0) {
                    System.out.println("Debe ingresar un monto mayor que cero.");
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

    private static void realizarDeposito(Scanner scanner, CuentaUsuario cuenta) {
        double monto = leerDoublePositivo(scanner, "Monto a depositar: $");
        cuenta.depositar(monto);
        System.out.printf("Depósito exitoso. Nuevo saldo: $%.2f%n", cuenta.getSaldo());
    }

    private static void realizarRetiro(Scanner scanner, CuentaUsuario cuenta) {
        double monto = leerDoublePositivo(scanner, "Monto a retirar: $");
        if (cuenta.retirar(monto)) {
            System.out.printf("Retiro exitoso. Nuevo saldo: $%.2f%n", cuenta.getSaldo());
        } else {
            System.out.println("Fondos insuficientes. Operación cancelada.");
        }
    }

    private static Map<String, CuentaUsuario> cargarCuentas() {
        Map<String, CuentaUsuario> cuentas = new HashMap<>();
        File archivo = new File(ARCHIVO_CUENTAS);
        if (!archivo.exists()) {
            return cuentas;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                String[] partes = linea.split("\\|");
                if (partes.length == 2) {
                    String usuario = partes[0];
                    double saldo = Double.parseDouble(partes[1]);
                    cuentas.put(usuario, new CuentaUsuario(usuario, saldo));
                }
            }
        } catch (IOException | NumberFormatException e) {
            System.out.println("Error al cargar cuentas: " + e.getMessage());
        }
        return cuentas;
    }

    private static void guardarCuentas(Map<String, CuentaUsuario> cuentas) {
        File archivo = new File(ARCHIVO_CUENTAS);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(archivo))) {
            for (CuentaUsuario cuenta : cuentas.values()) {
                writer.write(cuenta.toRegistro());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar cuentas: " + e.getMessage());
        }
    }
}

class CuentaUsuario {

    private final String usuario;
    private double saldo;

    public CuentaUsuario(String usuario, double saldo) {
        this.usuario = usuario;
        this.saldo = saldo;
    }

    public String getUsuario() {
        return usuario;
    }

    public double getSaldo() {
        return saldo;
    }

    public void depositar(double monto) {
        if (monto > 0) {
            saldo += monto;
        }
    }

    public boolean retirar(double monto) {
        if (monto > 0 && monto <= saldo) {
            saldo -= monto;
            return true;
        }
        return false;
    }

    public String toRegistro() {
        return usuario + "|" + saldo;
    }
}
