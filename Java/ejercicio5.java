import java.io.*;
import java.util.Scanner;

class Evaluacion {
    private String nombre;
    private double promedio;
    private String estado;

    public Evaluacion(String nombre, double n1, double n2, double n3) {
        this.nombre = nombre;
        this.promedio = (n1 + n2 + n3) / 3.0;
        this.estado = (this.promedio >= 7.0) ? "Aprobado" : "Reprobado";
    }

    @Override
    public String toString() {
        return nombre + "," + promedio + "," + estado;
    }
    public double getPromedio() { return promedio; }
    public String getEstado() { return estado; }
}

public class ejercicio5 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        System.out.println("--- REGISTRO DE NOTAS ---");
        System.out.print("Estudiante: "); String nom = sc.nextLine();
        System.out.print("Nota 1: "); double n1 = sc.nextDouble();
        System.out.print("Nota 2: "); double n2 = sc.nextDouble();
        System.out.print("Nota 3: "); double n3 = sc.nextDouble();

        Evaluacion e = new Evaluacion(nom, n1, n2, n3);

        try (PrintWriter pw = new PrintWriter(new FileWriter("calificaciones.txt", true))) {
            pw.println(e.toString());
            System.out.printf("Registro exitoso. Promedio: %.2f [%s]\n", e.getPromedio(), e.getEstado());
        } catch (IOException ex) {
            System.out.println("Error al guardar.");
        }
    }
}
