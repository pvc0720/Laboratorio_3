#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
using namespace std;

// Estructura para representar un usuario
struct Usuario {
    string cedula;
    string clave;
    double saldo;
};

// Función para comparar una clave ingresada con la clave guardada en "sudo.txt"
bool compararClave(const string& claveIngresada) {
    // Convierte la clave ingresada a su representación binaria
    string claveBinaria = "";
    for (char c : claveIngresada) {
        claveBinaria += bitset<8>(c).to_string();
    }

    // Invierte los unos y ceros en la clave binaria
    for (char &c : claveBinaria) {
        if (c == '0') {
            c = '1';
        } else if (c == '1') {
            c = '0';
        }
    }

    // Lee la clave guardada en el archivo "sudo.txt"
    ifstream archivo("sudo.txt");
    string claveGuardada;

    if (archivo.is_open()) {
        archivo >> claveGuardada;
        archivo.close();
    } else {
        cerr << "No se pudo abrir el archivo sudo.txt" << endl;
        return false; // En caso de error al abrir el archivo
    }

    // Compara la clave ingresada con la clave guardada
    return claveBinaria == claveGuardada;
}


// Función para registrar un nuevo usuario y guardar en un archivo
void registrarUsuario(vector<Usuario>& usuarios) {
    Usuario nuevoUsuario;

    cout << "Ingresa la cedula del nuevo usuario: ";
    cin >> nuevoUsuario.cedula;

    cout << "Ingresa la clave del nuevo usuario: ";
    cin >> nuevoUsuario.clave;

    cout << "Ingresa el saldo inicial del nuevo usuario (COP): ";
    cin >> nuevoUsuario.saldo;

    usuarios.push_back(nuevoUsuario);

    // Guardar el nuevo usuario en un archivo
    ofstream archivoUsuarios("usuarios.txt", ios::app); // Modo de apertura para agregar al archivo existente

    if (archivoUsuarios.is_open()) {
        archivoUsuarios << nuevoUsuario.cedula << " " << nuevoUsuario.clave << " " << nuevoUsuario.saldo << endl;
        archivoUsuarios.close();
    } else {
        cerr << "No se pudo abrir el archivo de usuarios." << endl;
    }

    cout << "Usuario registrado exitosamente." << endl;
}

// Función para cargar usuarios desde el archivo "usuarios.txt"
void cargarUsuariosDesdeArchivo(vector<Usuario>& usuarios) {
    usuarios.clear();
    ifstream archivoUsuarios("usuarios.txt");

    if (archivoUsuarios.is_open()) {
        Usuario usuario;
        while (archivoUsuarios >> usuario.cedula >> usuario.clave >> usuario.saldo) {
            usuarios.push_back(usuario);
        }
        archivoUsuarios.close();
    }
}

// Función para restar el costo de la operación (1000 COP) del saldo de un usuario
void restarCostoOperacion(Usuario& usuario) {
    usuario.saldo -= 1000; // Restar el costo de la operación
}

// Función para permitir el acceso como administrador
void ingresarComoAdministrador(vector<Usuario>& usuarios) {
    string claveAdmin;
    cout << "Ingresa la clave de administrador: ";
    cin >> claveAdmin;

    if (compararClave(claveAdmin)) {
        cout << "Acceso como administrador autorizado." << endl;

        // Menú de opciones para el administrador
        int opcion;

        do {
            cout << "Menu de opciones como administrador:" << endl;
            cout << "1. Agregar un nuevo usuario" << endl;
            cout << "2. Salir" << endl;
            cout << "Elige una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1:
                registrarUsuario(usuarios);
                break;
            case 2:
                break; // Salir del menú de administrador
            default:
                cout << "Opción no válida." << endl;
            }
        } while (opcion != 2);
    } else {
        cout << "Acceso como administrador denegado." << endl;
    }
}

// Función para permitir el acceso como usuario
void ingresarComoUsuario(vector<Usuario>& usuarios) {
    string cedulaUsuario;
    Usuario* usuarioActual = nullptr;

    // Cargar los usuarios desde el archivo "usuarios.txt"
    cargarUsuariosDesdeArchivo(usuarios);

    cout << "Ingresa tu cedula como usuario: ";
    cin >> cedulaUsuario;

    for (Usuario& usuario : usuarios) {
        if (usuario.cedula == cedulaUsuario) {
            usuarioActual = &usuario;
            break;
        }
    }

    if (usuarioActual) {
        string claveUsuario;
        cout << "Ingresa tu clave como usuario: ";
        cin >> claveUsuario;

        if (claveUsuario == usuarioActual->clave) {
            cout << "Acceso como usuario autorizado." << endl;

            // Menú de opciones para los usuarios
            int opcion;

            do {
                cout << "Menu de opciones para usuarios:" << endl;
                cout << "1. Consultar saldo" << endl;
                cout << "2. Retirar dinero" << endl;
                cout << "3. Salir" << endl;
                cout << "Elige una opcion: ";
                cin >> opcion;

                switch (opcion) {
                case 1:
                    // Consultar saldo
                    restarCostoOperacion(*usuarioActual);
                    cout << "Saldo actual (COP) para el usuario " << usuarioActual->cedula << ": " << usuarioActual->saldo << endl;
                    break;
                case 2:
                    double cantidad;
                    cout << "Ingresa la cantidad que deseas retirar (COP): ";
                    cin >> cantidad;

                    if (cantidad + 1000 > usuarioActual->saldo) {
                        cout << "Saldo insuficiente para realizar la operación." << endl;
                    } else {
                        usuarioActual->saldo -= cantidad + 1000; // Deduce el costo de la operación y la cantidad retirada
                        cout << "Retiro exitoso. Nuevo saldo: " << usuarioActual->saldo << " COP" << endl;
                    }
                    break;
                case 3:
                    break; // Salir del menú de usuario
                default:
                    cout << "Opción no válida." << endl;
                }
            } while (opcion != 3);

            // Actualizar el archivo de usuarios después de las operaciones
            ofstream archivoUsuarios("usuarios.txt");
            if (archivoUsuarios.is_open()) {
                for (const Usuario& u : usuarios) {
                    archivoUsuarios << u.cedula << " " << u.clave << " " << u.saldo << endl;
                }
                archivoUsuarios.close();
            } else {
                cerr << "No se pudo abrir el archivo de usuarios." << endl;
            }
        } else {
            cout << "Clave incorrecta. Acceso denegado." << endl;
        }
    } else {
        cout << "Usuario no encontrado. Acceso denegado." << endl;
    }
}

int main() {
    vector<Usuario> usuarios;
    cargarUsuariosDesdeArchivo(usuarios);

    int opcion;

    do {
        cout << "Menu de opciones:" << endl;
        cout << "1. Ingresar como administrador" << endl;
        cout << "2. Ingresar como usuario" << endl;
        cout << "3. Salir" << endl;
        cout << "Elige una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            ingresarComoAdministrador(usuarios);
            break;
        case 2:
            ingresarComoUsuario(usuarios);
            break;
        case 3:
            break; // Salir del programa
        default:
            cout << "Opción no válida." << endl;
        }
    } while (opcion != 3);

    return 0;
}
