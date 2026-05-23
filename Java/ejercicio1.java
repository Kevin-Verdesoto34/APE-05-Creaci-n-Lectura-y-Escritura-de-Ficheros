import java.io.*;
import java.util.Scanner;

class Estudiante {
    private String cedula;
    private String nombre;
    private String carrera;
    private double promedio;

    public Estudiante(String cedula, String nombre, String carrera, double promedio) {
        this.cedula = cedula;
        this.nombre = nombre;
        this.carrera = carrera;
        this.promedio = promedio;
    }

    @Override
    public String toString() {
        return cedula + "," + nombre + "," + carrera + "," + promedio;
    }
}

public class ejercicio1 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("--- REGISTRO DE ESTUDIANTES ---");
        System.out.print("Ingrese Cédula: "); String ced = sc.nextLine();
        System.out.print("Ingrese Nombre: "); String nom = sc.nextLine();
        System.out.print("Ingrese Carrera: "); String car = sc.nextLine();
        System.out.print("Ingrese Promedio: "); double prom = sc.nextDouble();

        Estudiante est = new Estudiante(ced, nom, car, prom);

        try (FileWriter fw = new FileWriter("estudiantes.txt", true);
             PrintWriter pw = new PrintWriter(fw)) {
            
            pw.println(est.toString());
            System.out.println("Mensaje: Registro guardado con éxito.");
            
        } catch (IOException e) {
            System.out.println("Error al escribir en el archivo: " + e.getMessage());
        }
    }
}
