import java.io.*;
import java.util.Scanner;

class Producto {
    private String codigo, nombre;
    private int cantidad;
    private double precio;

    public Producto(String codigo, String nombre, int cantidad, double precio) {
        this.codigo = codigo; this.nombre = nombre; this.cantidad = cantidad; this.precio = precio;
    }
    @Override
    public String toString() { return codigo + "," + nombre + "," + cantidad + "," + precio; }
}

public class ejercicio3 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int op;
        do {
            System.out.println("\n--- SISTEMA DE INVENTARIO ---");
            System.out.println("1. Registrar Producto\n2. Calcular Valor Total\n3. Salir");
            System.out.print("Seleccione: "); op = sc.nextInt(); sc.nextLine();

            if (op == 1) {
                System.out.print("Código: "); String cod = sc.next(); sc.nextLine();
                System.out.print("Nombre: "); String nom = sc.nextLine();
                System.out.print("Cantidad: "); int cant = sc.nextInt();
                System.out.print("Precio: "); double prec = sc.nextDouble();

                try (PrintWriter pw = new PrintWriter(new FileWriter("inventario.txt", true))) {
                    pw.println(new Producto(cod, nom, cant, prec).toString());
                    System.out.println("Producto guardado.");
                } catch (IOException e) { System.out.println("Error."); }
            } else if (op == 2) {
                File f = new File("inventario.txt");
                if (!f.exists()) { System.out.println("Sin registros."); continue; }
                double total = 0;
                try (BufferedReader br = new BufferedReader(new FileReader(f))) {
                    String l;
                    while ((l = br.readLine()) != null) {
                        String[] d = l.split(",");
                        total += (Integer.parseInt(d[2]) * Double.parseDouble(d[3]));
                    }
                    System.out.println("Valor total de activos en almacén: $" + total);
                } catch (Exception e) { System.out.println("Error de lectura."); }
            }
        } while (op != 3);
    }
}
