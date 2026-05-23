import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Ejercicio6 {

    private static final String NOMBRE_ARCHIVO = "asistencia.txt";

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int opcion;

        do {
            mostrarMenu();
            opcion = leerEntero(scanner, "Seleccione una opción: ");

            switch (opcion) {
                case 1:
                    registrarAsistencia(scanner);
                    break;
                case 2:
                    mostrarHistorial();
                    break;
                case 3:
                    contarFaltas();
                    break;
                case 4:
                    System.out.println("Saliendo del programa. ¡Hasta luego!");
                    break;
                default:
                    System.out.println("Opción inválida. Intente de nuevo.");
            }

            System.out.println();
        } while (opcion != 4);

        scanner.close();
    }

    private static void mostrarMenu() {
        System.out.println("=== Control de Asistencia ===");
        System.out.println("1. Registrar asistencia");
        System.out.println("2. Mostrar historial");
        System.out.println("3. Contar faltas");
        System.out.println("4. Salir");
    }

    private static int leerEntero(Scanner scanner, String mensaje) {
        int valor;
        while (true) {
            System.out.print(mensaje);
            String linea = scanner.nextLine().trim();
            try {
                valor = Integer.parseInt(linea);
                return valor;
            } catch (NumberFormatException e) {
                System.out.println("Debe ingresar un número válido.");
            }
        }
    }

    private static void registrarAsistencia(Scanner scanner) {
        String nombre = leerTextoNoVacio(scanner, "Nombre del estudiante: ");
        String fecha = leerTextoNoVacio(scanner, "Fecha (dd/mm/aaaa): ");
        String estado = leerEstado(scanner);

        String registro = nombre + "|" + fecha + "|" + estado;
        guardarRegistro(registro);

        System.out.println("Asistencia registrada correctamente.");
    }

    private static String leerTextoNoVacio(Scanner scanner, String mensaje) {
        String texto;
        do {
            System.out.print(mensaje);
            texto = scanner.nextLine().trim();
            if (texto.isEmpty()) {
                System.out.println("El valor no puede estar vacío. Intente de nuevo.");
            }
        } while (texto.isEmpty());
        return texto;
    }

    private static String leerEstado(Scanner scanner) {
        String estado;
        do {
            System.out.print("Estado (P = presente, F = falta): ");
            estado = scanner.nextLine().trim().toUpperCase();
            if (!estado.equals("P") && !estado.equals("F")) {
                System.out.println("Estado inválido. Ingrese P o F.");
            }
        } while (!estado.equals("P") && !estado.equals("F"));
        return estado;
    }

    private static void guardarRegistro(String registro) {
        File archivo = new File(NOMBRE_ARCHIVO);
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(archivo, true))) {
            writer.write(registro);
            writer.newLine();
        } catch (IOException e) {
            System.out.println("Error al guardar el registro: " + e.getMessage());
        }
    }

    private static void mostrarHistorial() {
        File archivo = new File(NOMBRE_ARCHIVO);
        if (!archivo.exists()) {
            System.out.println("No hay historial de asistencia registrado.");
            return;
        }

        System.out.println("--- Historial de asistencia ---");
        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            int contador = 0;
            while ((linea = reader.readLine()) != null) {
                contador++;
                String[] partes = linea.split("\\|");
                if (partes.length == 3) {
                    String nombre = partes[0];
                    String fecha = partes[1];
                    String estado = partes[2].equals("P") ? "Presente" : "Falta";
                    System.out.printf("%d. %s - %s - %s%n", contador, nombre, fecha, estado);
                } else {
                    System.out.printf("%d. Registro inválido: %s%n", contador, linea);
                }
            }
            if (contador == 0) {
                System.out.println("No hay registros en el archivo.");
            }
        } catch (IOException e) {
            System.out.println("Error al leer el historial: " + e.getMessage());
        }
    }

    private static void contarFaltas() {
        File archivo = new File(NOMBRE_ARCHIVO);
        if (!archivo.exists()) {
            System.out.println("No hay registros disponibles para contar faltas.");
            return;
        }

        int faltas = 0;
        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                String[] partes = linea.split("\\|");
                if (partes.length >= 3 && partes[2].equalsIgnoreCase("F")) {
                    faltas++;
                }
            }
            System.out.println("Total de faltas registradas: " + faltas);
        } catch (IOException e) {
            System.out.println("Error al contar faltas: " + e.getMessage());
        }
    }
}
