import java.io.*;
import java.util.Scanner;

public class ejercicio4 {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int op;
        do {
            System.out.println("\n--- AGENDA TELEFÓNICA ---");
            System.out.println("1. Agregar Contacto\n2. Buscar Contacto\n3. Salir");
            System.out.print("Opción: "); op = sc.nextInt(); sc.nextLine();

            if (op == 1) {
                System.out.print("Nombre: "); String n = sc.nextLine();
                System.out.print("Teléfono: "); String t = sc.nextLine();
                System.out.print("Correo: "); String c = sc.nextLine();
                try (PrintWriter pw = new PrintWriter(new FileWriter("agenda.txt", true))) {
                    pw.println(n + "," + t + "," + c);
                    System.out.println("Contacto registrado.");
                } catch (IOException e) { System.out.println("Error."); }
                
            } else if (op == 2) {
                System.out.print("Nombre a buscar: "); String busq = sc.nextLine().toLowerCase();
                File f = new File("agenda.txt");
                if (!f.exists()) continue;
                try (BufferedReader br = new BufferedReader(new FileReader(f))) {
                    String l; boolean ok = false;
                    while ((l = br.readLine()) != null) {
                        String[] d = l.split(",");
                        if (d[0].toLowerCase().contains(busq)) {
                            System.out.println("-> " + d[0] + " | Tel: " + d[1] + " | Email: " + d[2]);
                            ok = true;
                        }
                    }
                    if (!ok) System.out.println("Sin coincidencias.");
                } catch (Exception e) { System.out.println("Error."); }
            }
        } while (op != 3);
    }
}
