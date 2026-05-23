import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Ejercicio9 {

    private static final String ARCHIVO_VENTAS = "ventas.txt";
    private static final String ARCHIVO_FACTURAS = "facturas.txt";
    private static List<Venta> ventas = new ArrayList<>();
    private static int numeroFactura = 1;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        cargarVentas();

        int opcion;
        do {
            mostrarMenu();
            opcion = leerEntero(scanner, "Seleccione una opción: ");
            switch (opcion) {
                case 1:
                    registrarVenta(scanner);
                    break;
                case 2:
                    generarFactura(scanner);
                    break;
                case 3:
                    mostrarHistorialVentas();
                    break;
                case 4:
                    mostrarEstadisticas();
                    break;
                case 5:
                    System.out.println("Gracias por usar el Sistema de Ventas. ¡Hasta luego!");
                    break;
                default:
                    System.out.println("Opción inválida. Intente de nuevo.");
            }
            System.out.println();
        } while (opcion != 5);

        guardarVentas();
        scanner.close();
    }

    private static void mostrarMenu() {
        System.out.println("=== Sistema de Ventas ===");
        System.out.println("1. Registrar venta");
        System.out.println("2. Generar factura");
        System.out.println("3. Mostrar historial de ventas");
        System.out.println("4. Ver estadísticas");
        System.out.println("5. Salir");
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
                    System.out.println("Debe ingresar un valor mayor que cero.");
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

    private static void registrarVenta(Scanner scanner) {
        String producto = leerTextoNoVacio(scanner, "Nombre del producto: ");
        int cantidad = leerEntero(scanner, "Cantidad: ");
        double precio = leerDoublePositivo(scanner, "Precio unitario: $");

        Venta venta = new Venta(numeroFactura, producto, cantidad, precio);
        ventas.add(venta);
        numeroFactura++;

        guardarVentas();
        System.out.printf("Venta registrada. Total: $%.2f%n", venta.calcularTotal());
    }

    private static void generarFactura(Scanner scanner) {
        if (ventas.isEmpty()) {
            System.out.println("No hay ventas registradas.");
            return;
        }

        System.out.println("Seleccione el número de factura a generar:");
        int numero = leerEntero(scanner, "Número de factura: ");

        Venta venta = buscarVentaPorNumero(numero);
        if (venta != null) {
            String factura = venta.generarFactura();
            guardarFactura(factura);
            System.out.println(factura);
            System.out.println("Factura guardada en " + ARCHIVO_FACTURAS);
        } else {
            System.out.println("Factura no encontrada.");
        }
    }

    private static Venta buscarVentaPorNumero(int numero) {
        for (Venta venta : ventas) {
            if (venta.getNumeroFactura() == numero) {
                return venta;
            }
        }
        return null;
    }

    private static void mostrarHistorialVentas() {
        if (ventas.isEmpty()) {
            System.out.println("No hay ventas registradas.");
            return;
        }

        System.out.println("=== Historial de Ventas ===");
        System.out.printf("%-5s %-25s %-10s %-12s %-12s%n", "ID", "Producto", "Cantidad", "Precio Unit.", "Total");
        System.out.println("─".repeat(70));

        for (Venta venta : ventas) {
            System.out.printf("%-5d %-25s %-10d $%-11.2f $%-11.2f%n",
                    venta.getNumeroFactura(),
                    truncar(venta.getProducto(), 23),
                    venta.getCantidad(),
                    venta.getPrecio(),
                    venta.calcularTotal());
        }
    }

    private static void mostrarEstadisticas() {
        if (ventas.isEmpty()) {
            System.out.println("No hay ventas registradas.");
            return;
        }

        double totalVentas = 0;
        int totalProductos = 0;

        for (Venta venta : ventas) {
            totalVentas += venta.calcularTotal();
            totalProductos += venta.getCantidad();
        }

        double promedio = totalVentas / ventas.size();

        System.out.println("=== Estadísticas de Ventas ===");
        System.out.printf("Total de transacciones: %d%n", ventas.size());
        System.out.printf("Total de productos vendidos: %d%n", totalProductos);
        System.out.printf("Monto total vendido: $%.2f%n", totalVentas);
        System.out.printf("Venta promedio: $%.2f%n", promedio);
    }

    private static String truncar(String texto, int longitud) {
        if (texto.length() <= longitud) {
            return texto;
        }
        return texto.substring(0, longitud - 2) + "..";
    }

    private static void guardarFactura(String factura) {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(ARCHIVO_FACTURAS, true))) {
            writer.write(factura);
            writer.write("\n" + "═".repeat(50) + "\n");
        } catch (IOException e) {
            System.out.println("Error al guardar factura: " + e.getMessage());
        }
    }

    private static void cargarVentas() {
        File archivo = new File(ARCHIVO_VENTAS);
        if (!archivo.exists()) {
            return;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                String[] partes = linea.split("\\|");
                if (partes.length == 5) {
                    int numero = Integer.parseInt(partes[0]);
                    String producto = partes[1];
                    int cantidad = Integer.parseInt(partes[2]);
                    double precio = Double.parseDouble(partes[3]);
                    String fecha = partes[4];

                    Venta venta = new Venta(numero, producto, cantidad, precio);
                    venta.setFecha(fecha);
                    ventas.add(venta);

                    if (numero >= numeroFactura) {
                        numeroFactura = numero + 1;
                    }
                }
            }
        } catch (IOException | NumberFormatException e) {
            System.out.println("Error al cargar ventas: " + e.getMessage());
        }
    }

    private static void guardarVentas() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(ARCHIVO_VENTAS))) {
            for (Venta venta : ventas) {
                writer.write(venta.toRegistro());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar ventas: " + e.getMessage());
        }
    }
}

class Venta {

    private final int numeroFactura;
    private final String producto;
    private final int cantidad;
    private final double precio;
    private String fecha;

    public Venta(int numeroFactura, String producto, int cantidad, double precio) {
        this.numeroFactura = numeroFactura;
        this.producto = producto;
        this.cantidad = cantidad;
        this.precio = precio;
        this.fecha = obtenerFechaActual();
    }

    public int getNumeroFactura() {
        return numeroFactura;
    }

    public String getProducto() {
        return producto;
    }

    public int getCantidad() {
        return cantidad;
    }

    public double getPrecio() {
        return precio;
    }

    public double calcularTotal() {
        return cantidad * precio;
    }

    public void setFecha(String fecha) {
        this.fecha = fecha;
    }

    private String obtenerFechaActual() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss");
        return LocalDateTime.now().format(formatter);
    }

    public String generarFactura() {
        StringBuilder factura = new StringBuilder();
        factura.append("╔════════════════════════════════════════════════╗\n");
        factura.append("║            FACTURA DE VENTA\n");
        factura.append("╚════════════════════════════════════════════════╝\n");
        factura.append(String.format("Número de Factura: %d\n", numeroFactura));
        factura.append(String.format("Fecha: %s\n", fecha));
        factura.append("────────────────────────────────────────────────\n");
        factura.append(String.format("Producto: %s\n", producto));
        factura.append(String.format("Cantidad: %d\n", cantidad));
        factura.append(String.format("Precio Unitario: $%.2f\n", precio));
        factura.append("────────────────────────────────────────────────\n");
        factura.append(String.format("TOTAL: $%.2f\n", calcularTotal()));
        factura.append("════════════════════════════════════════════════\n");
        return factura.toString();
    }

    public String toRegistro() {
        return numeroFactura + "|" + producto + "|" + cantidad + "|" + precio + "|" + fecha;
    }
}

