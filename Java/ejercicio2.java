import java.io.*;

public class ejercicio2 {
    public static void main(String[] args) {
        File archivo = new File("estudiantes.txt");
        int contador = 0;
        double sumaPromedios = 0.0;

        System.out.println("--- REGISTROS DE ESTUDIANTES ---");

        if (!archivo.exists()) {
            System.out.println("El archivo estudiantes.txt no existe.");
            return;
        }

        try (FileReader fr = new FileReader(archivo);
             BufferedReader br = new BufferedReader(fr)) {
            
            String linea;
            while ((linea = br.readLine()) != null) {
                if (linea.trim().isEmpty()) continue;

                String[] datos = linea.split(",");
                if (datos.length == 4) {
                    String nombre = datos[1];
                    String carrera = datos[2];
                    double prom = Double.parseDouble(datos[3]);

                    System.out.println("Estudiante: " + nombre + " | Carrera: " + carrera + " | Promedio: " + prom);
                    
                    sumaPromedios += prom;
                    contador++;
                }
            }
            
            System.out.println("--------------------------------");
            if (contador > 0) {
                System.out.println("Total Estudiantes: " + contador);
                System.out.println("Promedio General: " + (sumaPromedios / contador));
            } else {
                System.out.println("Archivo vacío.");
            }

        } catch (IOException e) {
            System.out.println("Error al leer el archivo: " + e.getMessage());
        }
    }
}
