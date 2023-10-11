<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>

using namespace std;

string encodeMethod1(const string& text, int n, const string& inputFileName, const string& outputFileName) {
        // Abre los archivos de entrada y salida usando inputFileName y outputFileName
        std::ifstream inputFile(inputFileName);
        std::ofstream outputFile(outputFileName);

    char character;
    while (inputFile.get(character)) {
        // Convierte el carácter en su representación binaria
        std::bitset<8> binary(character);
        std::bitset<8> encodedBinary; // Crear un bitset para almacenar el resultado codificado

        // Mostrar la conversión inicial en binario en la consola
        std::cout << "Carácter '" << character << "' en binario: " << binary << std::endl;

            // Realiza la codificación para cada bloque de n bits
            for (int i = 0; i < 8; i += n) {
            std::bitset<8> block;
            for (int j = 0; j < n; j++) {
                if (i + j < 8) {
                    block[j] = binary[i + j];
                }
            }

            int countOnes = block.count(); // Contar unos en el bloque
            int countZeros = n - countOnes; // Calcular el número de ceros

            for (int j = 0; j < n; j++) {
                if (countOnes == countZeros) {
                    encodedBinary[i + j] = !block[j]; // Invertir cada bit si hay igual cantidad de unos y ceros
                } else if (countZeros > countOnes) {
                    if (j % 2 == 0) {
                        encodedBinary[i + j] = !block[j]; // Invertir cada 2 bits si hay más ceros
                    } else {
                        encodedBinary[i + j] = block[j];
                    }
                } else {
                    if (j % 3 == 0) {
                        encodedBinary[i + j] = !block[j]; // Invertir cada 3 bits si hay más unos
                    } else {
                        encodedBinary[i + j] = block[j];
                    }
                }
            }
        }

        // Escribe la representación binaria codificada en el archivo de salida
        outputFile << encodedBinary;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Cadena de caracteres codificada y guardada en output.txt." << std::endl;

    return 0;
}

string decodeMethod1(const string& encodedText, int n) {
    std::ifstream inputFile("output.txt");
    std::ofstream outputFile("decoded_output.txt");

    std::bitset<8> encodedBinary;
    while (inputFile >> encodedBinary) {
        // Realiza la decodificación para cada bloque de n bits
        std::bitset<8> decodedBinary;

        for (int i = 0; i < 8; i += n) {
            std::bitset<8> block;
            for (int j = 0; j < n; j++) {
                if (i + j < 8) {
                    block[j] = encodedBinary[i + j];
                }
            }

            int countOnes = block.count(); // Contar unos en el bloque
            int countZeros = n - countOnes; // Calcular el número de ceros

            for (int j = 0; j < n; j++) {
                if (countOnes == countZeros) {
                    decodedBinary[i + j] = !block[j]; // Invertir cada bit si hay igual cantidad de unos y ceros
                } else if (countZeros > countOnes) {
                    if (j % 2 == 0) {
                        decodedBinary[i + j] = !block[j]; // Invertir cada 2 bits si hay más ceros
                    } else {
                        decodedBinary[i + j] = block[j];
                    }
                } else {
                    if (j % 3 == 0) {
                        decodedBinary[i + j] = !block[j]; // Invertir cada 3 bits si hay más unos
                    } else {
                        decodedBinary[i + j] = block[j];
                    }
                }
            }
        }

        // Convierte el bitset decodificado en un carácter y escribe en el archivo de salida
        char decodedCharacter = static_cast<char>(decodedBinary.to_ulong());
        outputFile << decodedCharacter;
    }

    inputFile.close();
    outputFile.close();

    std::cout << "Cadena de bits decodificada y guardada en decoded_output.txt." << std::endl;

    return 0;
}

string encodeMethod2(const string& text, int n) {
    string encodedText;
    string binaryText;

    // Convierte el texto en una cadena de 0s y 1s
    for (char character : text) {
        bitset<8> binary(character);
        binaryText += binary.to_string();
    }

    // Reorganiza los bits según las especificaciones
    for (int i = 0; i < binaryText.length(); i++) {
        int newIndex = (i + n) % binaryText.length();
        encodedText += binaryText[newIndex];
    }

    return encodedText;
}

string decodeMethod2(const string& encodedText, int n) {
    string binaryText;
    string decodedText;

    // Invertir el reordenamiento de los bits
    for (int i = 0; i < encodedText.length(); i++) {
        int originalIndex = (i - n + encodedText.length()) % encodedText.length();
        binaryText += encodedText[originalIndex];
    }

    // Dividir la cadena binaria en bloques de 8 bits y convertirlos a caracteres ASCII
    for (size_t i = 0; i < binaryText.length(); i += 8) {
        string binaryByte = binaryText.substr(i, 8);
        bitset<8> bits(binaryByte);
        char character = char(bits.to_ulong());
        decodedText += character;
    }

    return decodedText;
}

int main() {
    int choice;
    cout << "Elije una opción:\n";
    cout << "1. Codificar (Metodo 1).\n";
    cout << "2. Decodificar (Metodo 1).\n";
    cout << "3. Codificar (Metodo 2).\n";
    cout << "4. Decodificar (Metodo 2).\n";
    cin >> choice;

    if (choice < 1 || choice > 4) {
        cerr << "Opcion no valida. Debes elegir entre 1 y 4." << endl;
        return 1;
    }

    string inputFileName; // Nombre del archivo de entrada
    string outputFileName; // Nombre del archivo de salida
    int n;

    cout << "Ingresa el nombre del archivo de entrada: ";
    cin >> inputFileName;
    cout << "Ingresa el nombre del archivo de salida (archivo .txt): ";
    cin >> outputFileName;

    ifstream inputFile(inputFileName);
    ofstream outputFile(outputFileName);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Error al abrir los archivos." << endl;
        return 1;
    }

    cout << "Ingresa el valor de n: ";
    cin >> n;

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    string result;

    if (choice == 1) {
        result = encodeMethod1(text, n, inputFileName, outputFileName);
    } else if (choice == 2) {
        result = decodeMethod1(text, n);
    } else if (choice == 3) {
        result = encodeMethod2(text, n);
    } else {
        result = decodeMethod2(text, n);
    }

    outputFile << result;

    inputFile.close();
    outputFile.close();

    if (choice == 1) {
        cout << "Texto codificado y guardado en " << outputFileName << "." << endl;
    } else if (choice == 2) {
        cout << "Texto decodificado y guardado en " << outputFileName << "." << endl;
    } else if (choice == 3) {
        cout << "Texto codificado y guardado en " << outputFileName << "." << endl;
    } else {
        cout << "Texto decodificado y guardado en " << outputFileName << "." << endl;
    }

    return 0;
}

=======
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
>>>>>>> ec9e017d5108d382c1b67de0bc9d726bef5f8e94
