import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Ejercicio8 {

    private static final String ARCHIVO_LIBROS = "biblioteca.txt";
    private static List<Libro> libros = new ArrayList<>();

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        cargarLibros();

        int opcion;
        do {
            mostrarMenu();
            opcion = leerEntero(scanner, "Seleccione una opción: ");
            switch (opcion) {
                case 1:
                    agregarLibro(scanner);
                    break;
                case 2:
                    buscarLibro(scanner);
                    break;
                case 3:
                    mostrarDisponibles();
                    break;
                case 4:
                    mostrarPrestados();
                    break;
                case 5:
                    prestarLibro(scanner);
                    break;
                case 6:
                    devolverLibro(scanner);
                    break;
                case 7:
                    mostrarTodos();
                    break;
                case 8:
                    System.out.println("Gracias por usar la Biblioteca Virtual. ¡Hasta luego!");
                    break;
                default:
                    System.out.println("Opción inválida. Intente de nuevo.");
            }
            System.out.println();
        } while (opcion != 8);

        guardarLibros();
        scanner.close();
    }

    private static void mostrarMenu() {
        System.out.println("=== Biblioteca Virtual ===");
        System.out.println("1. Agregar libro");
        System.out.println("2. Buscar libro");
        System.out.println("3. Mostrar libros disponibles");
        System.out.println("4. Mostrar libros prestados");
        System.out.println("5. Prestar libro");
        System.out.println("6. Devolver libro");
        System.out.println("7. Mostrar todos los libros");
        System.out.println("8. Salir");
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

    private static void agregarLibro(Scanner scanner) {
        String codigo = leerTextoNoVacio(scanner, "Código del libro: ");
        if (buscarPorCodigo(codigo) != null) {
            System.out.println("El código ya existe en la biblioteca.");
            return;
        }
        String titulo = leerTextoNoVacio(scanner, "Título del libro: ");
        String autor = leerTextoNoVacio(scanner, "Autor del libro: ");
        Libro libro = new Libro(codigo, titulo, autor, "disponible");
        libros.add(libro);
        guardarLibros();
        System.out.println("Libro agregado exitosamente.");
    }

    private static void buscarLibro(Scanner scanner) {
        String codigo = leerTextoNoVacio(scanner, "Ingrese el código del libro a buscar: ");
        Libro libro = buscarPorCodigo(codigo);
        if (libro != null) {
            System.out.println(libro);
        } else {
            System.out.println("Libro no encontrado.");
        }
    }

    private static Libro buscarPorCodigo(String codigo) {
        for (Libro libro : libros) {
            if (libro.getCodigo().equalsIgnoreCase(codigo)) {
                return libro;
            }
        }
        return null;
    }

    private static void mostrarDisponibles() {
        List<Libro> disponibles = new ArrayList<>();
        for (Libro libro : libros) {
            if (libro.isDisponible()) {
                disponibles.add(libro);
            }
        }

        if (disponibles.isEmpty()) {
            System.out.println("No hay libros disponibles en este momento.");
        } else {
            System.out.println("=== Libros Disponibles ===");
            for (int i = 0; i < disponibles.size(); i++) {
                System.out.printf("%d. %s%n", i + 1, disponibles.get(i));
            }
        }
    }

    private static void mostrarPrestados() {
        List<Libro> prestados = new ArrayList<>();
        for (Libro libro : libros) {
            if (!libro.isDisponible()) {
                prestados.add(libro);
            }
        }

        if (prestados.isEmpty()) {
            System.out.println("No hay libros prestados actualmente.");
        } else {
            System.out.println("=== Libros Prestados ===");
            for (int i = 0; i < prestados.size(); i++) {
                System.out.printf("%d. %s%n", i + 1, prestados.get(i));
            }
        }
    }

    private static void mostrarTodos() {
        if (libros.isEmpty()) {
            System.out.println("La biblioteca está vacía.");
        } else {
            System.out.println("=== Todos los Libros ===");
            for (int i = 0; i < libros.size(); i++) {
                System.out.printf("%d. %s%n", i + 1, libros.get(i));
            }
        }
    }

    private static void prestarLibro(Scanner scanner) {
        String codigo = leerTextoNoVacio(scanner, "Ingrese el código del libro a prestar: ");
        Libro libro = buscarPorCodigo(codigo);
        if (libro == null) {
            System.out.println("Libro no encontrado.");
        } else if (!libro.isDisponible()) {
            System.out.println("El libro ya está prestado.");
        } else {
            libro.prestar();
            guardarLibros();
            System.out.println("Libro prestado exitosamente.");
        }
    }

    private static void devolverLibro(Scanner scanner) {
        String codigo = leerTextoNoVacio(scanner, "Ingrese el código del libro a devolver: ");
        Libro libro = buscarPorCodigo(codigo);
        if (libro == null) {
            System.out.println("Libro no encontrado.");
        } else if (libro.isDisponible()) {
            System.out.println("El libro ya está disponible.");
        } else {
            libro.devolver();
            guardarLibros();
            System.out.println("Libro devuelto exitosamente.");
        }
    }

    private static void cargarLibros() {
        File archivo = new File(ARCHIVO_LIBROS);
        if (!archivo.exists()) {
            return;
        }

        try (BufferedReader reader = new BufferedReader(new FileReader(archivo))) {
            String linea;
            while ((linea = reader.readLine()) != null) {
                String[] partes = linea.split("\\|");
                if (partes.length == 4) {
                    String codigo = partes[0];
                    String titulo = partes[1];
                    String autor = partes[2];
                    String estado = partes[3];
                    libros.add(new Libro(codigo, titulo, autor, estado));
                }
            }
        } catch (IOException e) {
            System.out.println("Error al cargar libros: " + e.getMessage());
        }
    }

    private static void guardarLibros() {
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(ARCHIVO_LIBROS))) {
            for (Libro libro : libros) {
                writer.write(libro.toRegistro());
                writer.newLine();
            }
        } catch (IOException e) {
            System.out.println("Error al guardar libros: " + e.getMessage());
        }
    }
}

class Libro {

    private final String codigo;
    private final String titulo;
    private final String autor;
    private String estado;

    public Libro(String codigo, String titulo, String autor, String estado) {
        this.codigo = codigo;
        this.titulo = titulo;
        this.autor = autor;
        this.estado = estado;
    }

    public String getCodigo() {
        return codigo;
    }

    public boolean isDisponible() {
        return estado.equalsIgnoreCase("disponible");
    }

    public void prestar() {
        estado = "prestado";
    }

    public void devolver() {
        estado = "disponible";
    }

    public String toRegistro() {
        return codigo + "|" + titulo + "|" + autor + "|" + estado;
    }

    @Override
    public String toString() {
        return String.format("[%s] %s - %s (%s)", codigo, titulo, autor, estado);
    }
}
