// GRUPO DE PROYECTO 1
// Integrantes:
// - Miguel Madera
// - Santiago Romero
// - Nabil Bedros

// LIBRERIAS.

#include <iostream>  // para operaciones de entrada y salida de datos.
#include <string>    // para manejar string
#include <limits.h>  // para los punteros NEW.
#include <algorithm> // para usar .erase

using namespace std;

//-----------------------------------------------------------------------------------------------------
//-------------------------------------- Estructuras -----------------------------------------

// Tipos de especies: Heroes y Orcos
struct Especie
{
    string nombre_especie;
    int danno_fortaleza;
    int salud;
    int rapidez;
    int identificador;
    Especie *siguiente; // no se pone como null porque ela lista esta planteada de otra manera.
};

// Listas enlazadas de Especies
struct Lista_especie
{
    Especie *primero_especie = nullptr;
    int cantidad = 0;
};

// Personajes
struct personaje
{
    mochila *mochila = nullptr;
    string nombre;
    Especie *tipo;
    // Double porque puede explotar con el poder vigor enano
    double vitalidad;
    int fortaleza;
    int identificador;
    personaje *siguiente = nullptr;
};

// Implementos
struct Implemento
{
    string nombre_implemento;
    string tipo_implemento;
    int usos;
    int fortalezanecesaria;
    int valor;
    int identificador;
    Implemento *siguiente = nullptr;
};

// Mochila
struct mochila
{
    int identificador; // sera el mismo del personaje al que pernenezca.
    Implemento *implementos = nullptr;
    Poder_magico *poderes = nullptr;
    // aqui va el mapa.
};

// Poderes Magicos
struct Poder_magico
{
    string nombre_poder;
    string funcion;
    int identificador;
    Poder_magico *siguiente = nullptr;
};

// Mapa
// CAMBIAR
struct adyacencia
{
    string nombre; // compartira el nombre de la sala a la que pertenece. con otro que le ponga el usuario.
    int distancia;
    int energia_requerida;
    int identificador_adyacencia;
    adyacencia *siguiente = nullptr;
};

struct sala
{
    string nombre;
    int distancia;
    int energia_necesaria;
    int cantidad_adyacencias;
    int identificador_sala;
    adyacencia *adyacencia_siguiente = nullptr;
    sala *sala_siguiente = nullptr;
};

//-----------------------------------------------------------------------------------------------------
//--------------------------- Inicializacion cantidades y reguladores -----------------------------------

// Regulador para cuando se eliminen tipos de orcos en la lista.
int regulador_tipo_orco = 0;
int regulador_tipo_heroe = 0;

// Para los personajes
int regulador_personaje_orco = 0;
int cantidad_personaje_orco = 0;

int regulador_personaje_heroe = 0;
int cantidad_personaje_heroe = 0;

int cantidad_personajes_jugar = 0;
int regulador_personajes_jugar = 0;

// Para implementos
int cantidad_implementos = 0;
int regulador_implemento = 0;

// Para Poderes
int cantidad_poderes = 0;
int regulador_poderes = 0;

// Para el mapa (Salas y adyacencias) (CAMBIAR)
int cantidad_salas = 0;
int regulador_salas = 0;

int cantidad_adyacencias = 0;  // para el id de las adyacencias
int regulador_adyacencias = 0; // para el id tambien

//-----------------------------------------------------------------------------------------------------
//-------------------------------------- FUNCIONES GENERAlES-------------------------------------------

// Funciones para verificar formato de respuesta
// Función para validar y leer un entero con mensaje personalizado
int obtener_entero(const string &mensaje)
{
    string entrada;
    int valor;
    const size_t MAX_DIGITOS = 10; // Un límite seguro para la mayoría de los 'int'

    while (true)
    {
        cout << mensaje;
        getline(cin, entrada);

        bool es_valido = true;

        if (entrada == "0")
        {
            es_valido = false;
        }

        if (entrada.length() > MAX_DIGITOS)
        {
            es_valido = false;
            cout << "Error. El número ingresado es demasiado grande.\n";
            continue; // Volver al inicio del bucle para pedir una nueva entrada
        }

        for (char c : entrada)
        {
            if (!isdigit(c))
            {
                es_valido = false;
                break;
            }
        }

        if (es_valido && !entrada.empty())
        {
            try
            {
                valor = stoi(entrada);
                return valor;
            }
            catch (const std::out_of_range &oor)
            {
                cout << "Error. El número ingresado está fuera del rango permitido.\n";
                // No es estrictamente necesario aquí si ya validamos la longitud,
                // pero es una buena práctica para cubrir otros casos.
            }
        }
        else
        {
            cout << "Error. Ingrese solo números naturales.\n";
        }
    }
}

// Funcion especifica para el menu de opciones
// Se ve mas bonito asi
int obtener_opcion()
{
    return obtener_entero("Ingrese una opcion: ");
}

// Booleano para verificar string:
bool verificar_string(string respuesta)
{

    // Eliminar espacios en blanco al inicio y final
    respuesta.erase(remove(respuesta.begin(), respuesta.end(), ' '), respuesta.end());

    // Si la respuesta esta vacia, retorna Falso
    if (respuesta.empty())
    {
        return false;
    }

    for (char caracter : respuesta) // itera por cada elemento de la cadena.
    {
        if (isdigit(caracter)) // Si caracter es un numero, retorna Falso
        {
            return false;
        }
    }
    return true; // Si no hay numeros, retorna true
}

// Pide y devuelve valor del string ya verificado
// Mas practico para implementar en codigo
string devolver_string_verificada(const string &mensaje)
{
    string entrada;

    // Imprime el mensaje indicado y pide respuesta
    cout << mensaje;
    getline(cin, entrada);

    while (!(verificar_string(entrada)))
    {
        cout << "Formato Invalido." << endl;
        cout << mensaje;
        getline(cin, entrada);
    }

    return (entrada);
}

//-----------------------------------------------------------------------------------------------------
//------------------------------ PARA LOS TIPOS DE ESPECIES -------------------------------------------

// PARA CREAR UN NUEVO TIPO DE ORCO.
void Crear_tipo(Lista_especie &lista, int tipo) // tipo=1 orco / tipo=2 heroe
{                                               // toma la direccion de memoria de la lista.
    Especie *nuevo = new Especie();

    if (tipo == 1)
    { // si es orco
        cout << "\nIngrese los datos para la nueva especie de Orco: \n";
    }
    else if (tipo == 2)
    { // si es heroe
        cout << "\nIngrese los datos para la nueva especie de Heroe: \n";
    }
    else
    {
        cout << "ERROR. TIPO NO ESPECIFICADO. VERIFICAR NUMERO QUE SE LE PUSO A LA FUNCION CREAR_TIPO \n";
    }

    nuevo->nombre_especie = devolver_string_verificada("Nombre de la especie: ");

    if (tipo == 1)
    {
        nuevo->danno_fortaleza = obtener_entero("Danno: ");
    }

    else if (tipo == 2)
    {
        nuevo->danno_fortaleza = obtener_entero("Fortaleza: ");
    }
    else
    {
        cout << "ERROR. TIPO NO ESPECIFICADO. VERIFICAR NUMERO QUE SE LE PUSO A LA FUNCION CREAR_TIPO \n";
    }

    nuevo->salud = obtener_entero("Salud: ");
    nuevo->rapidez = obtener_entero("Rapidez: ");

    cout << endl;

    // inserta de primero en la lista.
    nuevo->siguiente = lista.primero_especie; // a nuevo lo pone a apuntar a lo que este apuntando la lista (nullptr o otro si ya hay elementos en la lista).
    lista.primero_especie = nuevo;            // y lista ahora apunta a nuevo ya que lista siempre es la cabeza de la lista en este caso.
    lista.cantidad = lista.cantidad + 1;

    // Sube el identificador uno mas uno
    if (tipo == 1)
    {
        nuevo->identificador = (lista.cantidad + regulador_tipo_orco); // le sumo el regulador para que el identificador no quede igual al ultimo
        // se le suma 1
        cout << "Especie de orco " << nuevo->nombre_especie << " ha sido agregada exitosamente. \n";
    }
    else
    {
        nuevo->identificador = (lista.cantidad + regulador_tipo_heroe); // le sumo el regulador para que el identificador no quede igual al ultimo
        // se le suma 1
        cout << "Especie de heroe " << nuevo->nombre_especie << " ha sido agregada exitosamente. \n";
    }
}

// Para mostrar listas de especies
void mostrar_lista(Lista_especie &lista, int tipo) // tipo=1 orco / tipo=2 heroe
{                                                  // toma la direccion de memoria.
    if (lista.cantidad == 0)
    { // si no hay elementos no hace nada.
        cout << "No hay tipos de Orcos disponibles. \n\n";
        return;
    }

    if (tipo == 1)
    {
        cout << "\nHay [" << lista.cantidad << "] tipos de orcos disponibles" << endl;
    }
    else
    {
        cout << "\nHay [" << lista.cantidad << "] tipos de heroes disponibles" << endl;
    }
    Especie *actual = lista.primero_especie; // se crea una variable auxiliar para igualarla al primer elemnto de la lista.
    while (actual != nullptr)
    { // si es igual a nullptr significa que es el ultimo elemento de la lista.
        cout << actual->identificador << "-";
        cout << "Nombre= " << actual->nombre_especie << endl;
        if (tipo == 1)
        {
            cout << "Danno = " << actual->danno_fortaleza << endl;
        }
        else
        {
            cout << "fortaleza = " << actual->danno_fortaleza << endl;
        }
        cout << "Salud = " << actual->salud << endl;
        cout << "Rapidez = " << actual->rapidez << "\n"
             << endl;
        actual = actual->siguiente; // pasa a siguiente elemento de la lista.
    }
    cout << "No hay mas tipos disponibles.\n"
         << endl;
}

// Para actalizar tipos de especies.
void actualizar_tipo(Lista_especie &lista, int tipo) // tipo=1 orco / tipo=2 heroe
{
    if (lista.cantidad == 0)
    {
        cout << "La lista se encuentra vacia.\n\n";
        return; // deja de ejecutar la funcion
    }

    int referencia;
    cout << "\nLos tipos de especie disponible son: ";
    cout << "------------------------" << endl;
    mostrar_lista(lista, tipo);
    if (tipo == 1)
    {
        referencia = obtener_entero("Ingrese el numero del tipo de orco que desea modificar: ");
    }
    else if (tipo == 2)
    {
        referencia = obtener_entero("ingrese el numero del tipo de heroe que desea modificar: ");
    }
    else
    {
        cout << "ERROR. VERIFICAR CODIGO. ACTUALIZAR_TIPO" << endl;
    }
    Especie *actual = lista.primero_especie;
    bool encontrado = false;

    while (actual != nullptr)
    {
        if (actual->identificador == referencia)
        {
            if (tipo == 1)
            {
                cout << "\nIngrese los datos a actualizar para el orco con identificador " << referencia << ":\n";
            }
            else
            {
                cout << "\nIngrese los datos a actualizar para el heroe con identificador " << referencia << ":\n";
            }
            cin.ignore(); // Limpiar el buffer

            actual->nombre_especie = devolver_string_verificada("Nombre de la especie: ");

            if (tipo == 1)
            {
                actual->danno_fortaleza = obtener_entero("Danno: ");
            }
            else
            {
                actual->danno_fortaleza = obtener_entero("Fortaleza: ");
            }

            actual->salud = obtener_entero("Salud: ");
            actual->rapidez = obtener_entero("Rapidez: ");

            if (tipo == 1)
            {
                cout << "Datos del orco con ID " << referencia << " actualizados exitosamente.\n";
            }
            else
            {
                cout << "Datos del heroe con ID " << referencia << " actualizados exitosamente.\n";
            }
            encontrado = true;
            break; // Importante salir del bucle una vez encontrado
        }
        actual = actual->siguiente;
    }

    if (!encontrado)
    {
        cout << "No se encontró ningún tipo de orco con el ID: " << referencia << ".\n";
    }
}

// funcion para liberar memoria dinamica.
void destruir_lista_especie(Lista_especie &lista)
{                                            // Toma la direccion de memoria de la lista que se le pase (del tipo).
    Especie *actual = lista.primero_especie; // declara un nodo actual para igualarlo al primer elemento de la lista.
    while (actual != nullptr)
    {                               // si es nullptr es porque es el ultimo elemento.
        Especie *eliminar = actual; // se crea una variable aux (eliminar) para darle la direccion de memoria de actual.
        actual = actual->siguiente; // se manda al siguente nodo al actual.
        delete eliminar;            // se elimina la variable aux (eliminar).
    }
    // la declaramos en null a la lista.
    lista.primero_especie = nullptr;
    lista.cantidad = 0;
}

// Funcion que retorna un puntero a una especie.
Especie *encontrar_especie(Lista_especie &lista, int identificador)
{
    Especie *actual = lista.primero_especie;
    while (actual != nullptr)
    {
        if (actual->identificador == identificador)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    cout << "La especie ingresada no existe." << endl;
    return actual = nullptr;
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- PARA LOS PERSONAJES ---------------------------------------------

// Para crear personajes.
void crear_personaje(personaje &lista_personaje, Lista_especie &lista_tipos, int tipo)
{
    if (lista_tipos.cantidad == 0)
    { // si no hay tipos creados se sale.
        cout << "Todavia no hay especies disponibles. Debe crearlas antes de crear un personaje." << endl;
        return;
    }
    else
    {
        personaje *nuevo = new personaje;
        int num_tipo = 0;                   // numero del tipo del personaje (identificador)
        Especie *puntero_especie = nullptr; // puntero al que se le asignara la especie.

        if (tipo == 1)
        {
            cout << "Indique los datos para el nuevo personaje Orco." << endl;

            nuevo->nombre = devolver_string_verificada("Nombre: ");

            cout << "Ingrese la especie al que pertenecera el Orco" << nuevo->nombre << "\n";
            cout << "Especies disponibles: " << endl;
            mostrar_lista(lista_tipos, tipo);

            cout << "Ingrese la especie del Orco " << nuevo->nombre << ": ";
            num_tipo = obtener_entero("");

            puntero_especie = encontrar_especie(lista_tipos, num_tipo);
        }
        else
        { // lo mismo pero para heroes.
            cout << "Indique los datos para el nuevo personaje Heroe. " << endl;

            nuevo->nombre = devolver_string_verificada("Nombre: ");

            cout << "Ingrese la especie al que pertenecera el Heroe" << nuevo->nombre << "\n";
            cout << "Especies disponibles: " << endl;
            mostrar_lista(lista_tipos, tipo);

            cout << "Ingrese la especie del Personaje " << nuevo->nombre << ":";
            num_tipo = obtener_entero("");

            puntero_especie = encontrar_especie(lista_tipos, num_tipo);
            // para validar que si esta la especie en la lista.
        }

        if (puntero_especie == nullptr)
        {
            cout << "La especie debe crearse primero." << endl;
            cout << "Creacion del personaje: " << nuevo->nombre << " fue fallida " << endl;
            delete nuevo; // liberamos memoria dinamica
            return;
        }
        else
        {
            nuevo->tipo = puntero_especie;
        }
        // lo mandamos a la lista enlazada.
        nuevo->siguiente = lista_personaje.siguiente;
        lista_personaje.siguiente = nuevo;

        if (tipo == 1)
        {
            cantidad_personaje_orco += 1; // sumamos a la cantidad de orcos.
            nuevo->identificador = (cantidad_personaje_orco + regulador_personaje_orco);
            cout << "El orco: " << nuevo->nombre << ", se agrego correctamente." << endl;
        }
        else
        {
            cantidad_personaje_heroe += 1; // sumamos a la cantidad de heroes.
            nuevo->identificador = (cantidad_personaje_heroe + regulador_personaje_heroe);
            cout << "El heroe: " << nuevo->nombre << ", se agrego correctamente." << endl;
        }
    }
}

// Funcion para mostrar los personajes
void mostrar_personajes(personaje &lista, int tipo) // tipo=1 orco / tipo=2 heroe
{                                                   // toma la direccion de memoria.
    // para validar que si hay personajes para mostrar.
    if (tipo == 1)
    { // para orcos.
        if (cantidad_personaje_orco == 0)
        {
            cout << "\nNo hay especies de orcos para mostra actualmente \n";
            cout << "primero debe crearlos\n";
            return;
        }
    }
    else
    { // para heroes
        if (cantidad_personaje_heroe == 0)
        {
            cout << "\nno hay personajes heroes para mostra actualmente \n";
            cout << "primero debe crerlos\n";
        }
    }

    if (tipo == 1)
    {
        cout << "\nHay [" << cantidad_personaje_orco << "] personajes orcos" << endl;
    }
    else
    {
        cout << "\nHay [" << cantidad_personaje_heroe << "] personajes heroes" << endl;
    }
    personaje *actual = lista.siguiente; // se crea una variable auxiliar para igualarla al primer elemnto de la lista.
    while (actual != nullptr)
    { // si es igual a nullptr significa que es el ultimo elemento de la lista.
        cout << actual->identificador << "-";
        cout << "Nombre: " << actual->nombre << endl;
        cout << "Especie: " << actual->tipo->nombre_especie << endl;
        cout << endl;
        actual = actual->siguiente;
    }
    cout << "No hay mas personajes.\n"
         << endl;
}

// Encontrar un personaje
personaje *encontrar_personaje(personaje &lista_personajes, int identificador)
{
    personaje *actual = lista_personajes.siguiente;
    while (actual != nullptr)
    {
        if (actual->identificador == identificador)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return actual = nullptr;
}

// para saber cuantos pesonajes hay de una especie
int cantidad_personajes_por_especie(personaje &lista_personajes, Especie *mostrar)
{
    personaje *actual = lista_personajes.siguiente;
    int contador = 0;
    while ((actual != nullptr))
    {
        if (actual->tipo->identificador == mostrar->identificador)
        {
            contador += 1;
        }
        actual = actual->siguiente;
    }
    return contador;
}

// funcion para modificar a los personajes.
void actualizar_personaje(personaje &lista, Lista_especie lista_tipo, int tipo) // tipo=1 orco / tipo=2 heroe
{
    if (tipo == 1)
    { // para orcos.
        if (cantidad_personaje_orco == 0)
        {
            cout << "\nNo hay personajes orcos para mostrar actualmente \n";
            cout << "Primero debe crearlos\n";
            return;
        }
    }
    else
    { // para heroes
        if (cantidad_personaje_heroe == 0)
        {
            cout << "\nNo hay personajes heroes para mostrar actualmente \n";
            cout << "primero debe crearlos\n";
            return;
        }
    }
    // el procedimineto es similar al de crear personajes.
    int identificador_tipo = 0;
    int referencia;
    Especie *especie_nueva = nullptr;

    cout << "Los personajes disponibles son: \n";
    mostrar_personajes(lista, tipo);

    referencia = obtener_entero("Ingrese el numero del personaje que desea modificar: ");

    personaje *actual = lista.siguiente;
    bool encontrado = false;

    while (actual != nullptr)
    {
        if (actual->identificador == referencia)
        {
            encontrado = true;
            cout << "ingrese los datos para actualizar al personaje " << actual->nombre << endl;
            cin.ignore(); // Limpiar el buffer

            actual->nombre = devolver_string_verificada("Nombre del personaje: ");

            cout << "\n Coloque el nuevo tipo de de especie al que pertenecera el personje \n";
            cout << "\n Tipos disponibles son: " << endl;
            cout << "------------------------" << endl;
            mostrar_lista(lista_tipo, tipo);

            // Identificador
            identificador_tipo = obtener_entero("Coloque el numero del tipo del personaje: ");

            especie_nueva = encontrar_especie(lista_tipo, identificador_tipo);

            if (especie_nueva == nullptr)
            {
                cout << "La que coloco no se encuentra disponible." << endl;
                cout << "La actualiacion del personaje: " << actual->nombre << " fue fallida " << endl;
                return;
            }
            else
            {
                cout << "El personaje ahora de nombre: " << actual->nombre << " se actualizo correctamente";
            }
        }
        actual = actual->siguiente;
    }
    if (!encontrado)
    {
        cout << "No se encontró ningún personaje con el ID: " << referencia << ".\n";
        cout << "Actualizacion fallida \n";
    }
}

// Para borrar un personaje.
void borrar_personaje(personaje &lista, int tipo)
{
    if (tipo == 1)
    { // para orcos.
        if (cantidad_personaje_orco == 0)
        {
            cout << "\nNo hay personajes orcos para eliminar actualente \n";
            cout << "Primero debe crearlos\n";
            return;
        }
    }
    else
    { // para heroes
        if (cantidad_personaje_heroe == 0)
        {
            cout << "\nNo hay personajes heroes para eliminar actualmente \n";
            cout << "Primero debe crearlos\n";
            return;
        }
    }

    int identificador = 0;
    personaje *actual = lista.siguiente; // aounta al primero de la lista
    personaje *anterior = nullptr;

    cout << "Los personajes disponibles son: \n";
    mostrar_personajes(lista, tipo);
    identificador = obtener_entero("Coloque el numero del personaje a modificar: ");

    if ((actual != nullptr) && (actual->identificador == identificador))
    {
        lista.siguiente = actual->siguiente;
        cout << "El personaje: " << actual->nombre << " fue borrado axitosamente." << endl;
        delete actual;
        if (tipo == 1)
        {
            regulador_personaje_orco += 1;
            cantidad_personaje_orco -= 1;
        }
        else
        {
            regulador_personaje_heroe += 1;
            cantidad_personaje_heroe -= 1;
        }
        return;
    }

    while ((actual != nullptr) && (actual->identificador != identificador))
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != nullptr)
    {
        personaje *eliminar = actual;
        anterior->siguiente = actual->siguiente;
        cout << "El personaje " << eliminar->nombre << " se elimino correctapente. \n ";
        delete eliminar;

        if (tipo == 1)
        {
            regulador_personaje_orco += 1;
            cantidad_personaje_orco -= 1;
        }
        else
        {
            regulador_personaje_heroe += 1;
            cantidad_personaje_heroe -= 1;
        }
    }
    else
    {
        cout << "El numero de personaje: " << identificador << " no se encuentra en la lista \n ";
        cout << "Eliminacion fallida \n";
    }
}

// Funcion para destruir la lista de personajes.
void destruir_lista_personajes(personaje &lista)
{                                        // Toma la direccion de memoria de la lista que se le pase (del tipo).
    personaje *actual = lista.siguiente; // declara un nodo actual para igualarlo al primer elemento de la lista.
    while (actual != nullptr)
    {                                 // si es nullptr es porque es el ultimo elemento.
        personaje *eliminar = actual; // se crea una variable aux (eliminar) para darle la direccion de memoria de actual.
        actual = actual->siguiente;   // se manda al siguente nodo al actual.
        delete eliminar;              // se elimina la variable aux (eliminar).
    }
    // la declaramos en null a la lista.
}

// esta funcion la coloque
// para eliminar un elemento de la lista
void eliminar_elemento_lista(Lista_especie &lista, personaje lista_personajes, int tipo)
{
    if (lista.cantidad == 0)
    {
        cout << "No hay elementos en la lista para borrar\n";
        return;
    }
    cout << "Las especies disponibles son: \n";
    mostrar_lista(lista, tipo);

    int identificador = 0;

    identificador = obtener_entero("Ingrese el numero del tipo que desea borrar: ");

    if (identificador > 0)
    {
        int controlador_eliminar = 0; // para controlar si se va a eliminar o no el tipo

        // respecto a los tipos
        Especie *actual = lista.primero_especie;
        Especie *anterior = nullptr; // Inicializamos anterior como nullptr

        // respecto a los personajes
        Especie *especie_borrar = encontrar_especie(lista, identificador);
        int cantida_personajes = 0;
        // Caso especial: el elemento a eliminar es el primero de la lista

        // para ver la cntidad de personajes que exixten con el tipo que desea borrar el usuario.
        cantida_personajes = cantidad_personajes_por_especie(lista_personajes, especie_borrar);

        // si si hay personajes se le dice al usuario a ver si de todas formas decea eliminar el tipo.
        if (cantida_personajes > 0)
        {
            cout << " Hay: " << cantida_personajes << "personajes de esta especie, por lo tanto. \n";
            cout << "Al eliminar la especie se boraran los personajes del tipo \n";
            cout << "eliminar de todas formas coloque 1 \n";
            cout << "no eliminar coloque 2 \n";
            do
            {
                cout << "(1) o (2)\n";
                controlador_eliminar = obtener_entero("coloque el numero:");
            } while ((controlador_eliminar != 1) && (controlador_eliminar != 2));
        }
        else
        {
            cout << " no hay personajes atualmente de este tipo, no hay problema al eliminarlo. \n";
        }
        // para eliminar si el usuario dice que si
        if (controlador_eliminar == 1)
        {
            // procedemos a borrar los personajes del tipo
            borrar_personajes_de_tipo(lista_personajes, especie_borrar, tipo);
        }
        else if (controlador_eliminar == 2)
        {
            cout << "eliminacion del tipo " << especie_borrar->nombre_especie << " fallida.\n";
            return;
        }

        // sigue el procedimiento normal para elimanar el tipo.
        if ((actual != nullptr) && actual->identificador == identificador)
        {
            lista.primero_especie = actual->siguiente;
            cout << "El tipo " << actual->nombre_especie << " se elimino correctamente" << endl;
            delete actual;
            lista.cantidad -= 1;
            if (tipo == 1)
            {
                regulador_tipo_orco += 1;
            }
            else
            {
                regulador_tipo_heroe += 1;
            }
            return; // Salimos de la función después de eliminar el primer elemento
        }

        // Caso general: el elemento a eliminar no es el primero
        while ((actual != nullptr) && (actual->identificador != identificador))
        {
            anterior = actual;
            actual = actual->siguiente;
        }

        if (actual != nullptr)
        {
            Especie *eliminar = actual;
            anterior->siguiente = actual->siguiente;
            cout << "El tipo " << eliminar->nombre_especie << " se elimino correctamente" << endl;
            delete eliminar;
            lista.cantidad -= 1;
            if (tipo == 1)
            {
                regulador_tipo_orco += 1;
            }
            else
            {
                regulador_tipo_heroe += 1;
            }
        }
        else
        {
            cout << "El numero de especie: " << identificador << " no se encuentra en la lista. \n";
            cout << "Eliminacion fallida \n";
        }
    }
    else
    {
        cout << "El numero de especie ingresado no es valido. \n";
    }
}

// borra todos los personajes de un tipo (epecie).
void borrar_personajes_de_tipo(personaje &lista_personajes, Especie *tipo_borrar, int tipo)
{
    personaje *actual = lista_personajes.siguiente; // El primero en la lista (después del nodo cabeza)
    personaje *anterior = &lista_personajes;        // El anterior al que se elimina (inicialmente la cabeza)
    personaje *eliminar = nullptr;                  // Para almacenar el nodo a eliminar

    while (actual != nullptr)
    {
        if (actual->tipo != nullptr && actual->tipo->identificador == tipo_borrar->identificador)
        {
            eliminar = actual;
            anterior->siguiente = actual->siguiente; // Enlaza el anterior con el siguiente del nodo a eliminar
            actual = actual->siguiente;              // Avanza actual para seguir buscando

            cout << "El personaje: " << eliminar->nombre << " fue borrado." << endl;
            delete eliminar; // Se libera la memoria del personaje borrado

            if (tipo == 1)
            {
                regulador_personaje_orco++;
                cantidad_personaje_orco--;
            }
            else
            {
                regulador_personaje_heroe++;
                cantidad_personaje_heroe--;
            }
        }
        else
        {
            anterior = actual;          // Si no se borra, el actual se convierte en el anterior
            actual = actual->siguiente; // Avanza al siguiente personaje
        }
    }
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES IMPLEMENTOS --------------------------------------------

// para crear implementos.
void crear_implemento(Implemento &lista_implemento)
{
    Implemento *nuevo = new Implemento;
    cout << "Proceda a llenar las espicificaciones del imlemento. \n\n";
    nuevo->nombre_implemento = devolver_string_verificada("Nobre:");
    nuevo->tipo_implemento = tipo_implemento(nuevo); // funcion para obteren el tipo de implemento.
    nuevo->usos = obtener_entero("Usos: ");
    nuevo->fortalezanecesaria = obtener_entero("Fortaleza necesaria para usar el implemento: ");
    nuevo->valor = obtener_entero("valor: ");
    // lo agregamos a la lista enlazada.
    nuevo->siguiente = lista_implemento.siguiente;
    lista_implemento.siguiente = nuevo;
    cantidad_implementos += 1;
    nuevo->identificador = cantidad_implementos + regulador_implemento;
    cout << "El implemento: " << nuevo->nombre_implemento << "se creo correctamente.";
}

void mostrar_implementos(Implemento &lista_implemento)
{
    if (cantidad_implementos == 0)
    {
        cout << "No hay implementos disponibles en este momento. \n";
        return;
    }
    else
    {
        cout << "la candtad de implementos es: [" << cantidad_implementos << "]. \n";
    }

    Implemento *actual = lista_implemento.siguiente;
    while (actual != nullptr)
    {
        cout << "Nombre: " << actual->nombre_implemento << endl;
        cout << "ID: " << actual->identificador << endl;
        cout << "Tipo: " << actual->tipo_implemento << endl;
        cout << "Uso: " << actual->usos << endl;
        cout << "Fortaleza necesaria para usarlo: " << actual->fortalezanecesaria << endl;
        cout << "Valor: " << actual->valor << endl
             << endl;
        actual = actual->siguiente;
    }
    cout << "no hay mas implementos disponibles\n";
}

// para obtener el tipo de implemento.
string tipo_implemento(Implemento *Implemento)
{
    string tipo;
    int opcion = 0;
    int infinito = 0; // o lo pone bien o lo pone bien;
    do
    {
        cout << "\nLos tipos de implementos son: \n";
        cout << "1. Ataque\n";
        cout << "2. Proteccion.\n";
        cout << "3. Cura.\n";
        cout << "Indique que tipo de implemento sera " << Implemento->nombre_implemento << ": ";
        opcion = obtener_entero("");
        if (opcion == 1)
        {
            return tipo = "Ataque";
        }
        else if (opcion == 2)
        {
            return tipo = "Proteccion";
        }
        else if (opcion == 3)
        {
            return tipo = "Cura";
        }
        else
        {
            cout << "Opcion invalida, el numero " << opcion << " no esta disponible.\n";
        }

    } while (infinito != 1);
    cout << "ESTA LINEA NO SE DEBERIA DE EJECUTAR" << endl;
    return tipo;
} // lo marca porque el bucle es infinito.

// buscar implemento.
Implemento *buscar_implemento(Implemento &lista_implemento, int identificador)
{
    Implemento *encontrado = &lista_implemento;
    while (encontrado != nullptr)
    {
        if (encontrado->identificador == identificador)
        {
            return encontrado;
        }
        encontrado = encontrado->siguiente;
    }
    return encontrado = nullptr;
}

// para modificar los implementos.
void modificar_implemento(Implemento &lista_implemento)
{
    if (cantidad_implementos == 0)
    {
        cout << "No hay implementos disponibles para modificar \n";
        cout << "modificacion fallida.\n";
        return;
    }
    int identificador = 0;
    cout << "los impplementos disponibles son: \n";
    mostrar_implementos(lista_implemento);
    identificador = obtener_entero("Coloque el ID del implemento que desea modificar: ");
    Implemento *actualizar = buscar_implemento(lista_implemento, identificador);
    if (actualizar == nullptr)
    {
        cout << "El ID que coloco no es valido.\n";
        cout << "No hay ningun implemento con ese ID.\n";
        cout << "Actualizacion fallida.\n";
        return;
    }

    //                          VERRRRRRRRRRRRRRRRRRRR
    // coloco este mensaje porque no se porque el progra no corre normal.
    cout << "precione enter para modificar el implemto: " << actualizar->nombre_implemento;
    // hay que darle enter para que corra.

    cin.ignore();
    actualizar->nombre_implemento = devolver_string_verificada("Nombre: ");
    actualizar->tipo_implemento = tipo_implemento(actualizar);
    actualizar->usos = obtener_entero("Usos: ");
    actualizar->fortalezanecesaria = obtener_entero("Fortaleza necesaria para usarlo:");
    actualizar->valor = obtener_entero("Valor: ");
    cout << "Implemento: " << actualizar->nombre_implemento << ", modificado correctamente.\n ";
}

// para destruir la lista de implementos.
void destruir_implementos(Implemento &lista_implemento)
{
    Implemento *actual = lista_implemento.siguiente;
    Implemento *eliminar = nullptr;
    while (actual != nullptr)
    {
        eliminar = actual;
        actual = actual->siguiente;
        delete eliminar;
    }
    cantidad_implementos = 0;
    regulador_implemento = 0;
}

// para borrar implementos.
void borrar_implementos(Implemento &lista_implemento)
{
    if (cantidad_implementos == 0)
    {
        cout << "No hay implementos para eliminar: \n";
        cout << "Eliminacion fallida. \n";
        return;
    }
    cout << "Los implementos disponibles son: \n";
    mostrar_implementos(lista_implemento);
    int identificador = obtener_entero("coloque el ID del implemento que desea eliminar:");
    Implemento *encontrado = buscar_implemento(lista_implemento, identificador);
    if (encontrado == nullptr)
    {
        cout << "  No existe un implemento con el ID: " << identificador;
        cout << "\nEliminacion fallida. \n";
        return;
    }

    // comiza la eliminacion del implemento.
    Implemento *actual = lista_implemento.siguiente;

    // si es el primero en la lista.
    if (actual->identificador == identificador)
    {
        lista_implemento.siguiente = actual->siguiente;
        cout << "El implemento: " << actual->nombre_implemento << "se elimino correctamente.\n";
        delete actual;
        cantidad_implementos -= 1;
        regulador_implemento += 1;
        return;
    }

    // si no es el primero.
    Implemento *anterior = nullptr;
    while ((actual != nullptr) && (actual->identificador != identificador))
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    // ahora eliminamos.
    Implemento *eliminar = actual;
    anterior->siguiente = actual->siguiente;
    cout << "El elemento: " << eliminar->nombre_implemento << "se elimino correctamente. ";
    delete eliminar;
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES MOCHILA -----------------------------------------------

// para llenar la mochila de un personaje.
void llenar_mochila(personaje *&personaje_a_llenar, Implemento &Implementos, Poder_magico &poderes)
{
    int opcio = 0;
    int cantidad_objetos = 0;
    int identificador = 0;
    int contrilador_interno = 0;
    cout << "Puede escoger un maximo 5 objetos, entre poderes magicos o implementos. \n";
    do
    {
        cout << "Tiene " << cantidad_objetos << " objetos en la mochila.\n";
        cout << " Que tipo de objeto desea agregar.\n";
        cout << "1. Implemento.\n";
        cout << "2. Poderes Magicos.\n";
        cout << "3. Salir.\n";
        opcio = obtener_entero("");
        if (opcio == 1)
        {
            Implemento *nuevo_implemento = nullptr;
            // para verificar si el ID es valido, lo pone bien o lo pone bien.
            do
            {
                cout << "Los implementos disponibles son:\n";
                mostrar_implementos(Implementos);
                cout << "coloque el ID del implemento que desea colocar: ";
                identificador = obtener_entero("");
                nuevo_implemento = buscar_implemento(Implementos, identificador);

                if (nuevo_implemento == nullptr)
                {
                    cout << "El ID: " << identificador << " no existe.\n";
                    cout << "coloque uno valido.\n";
                }
                else if (nuevo_implemento->fortalezanecesaria > personaje_a_llenar->tipo->danno_fortaleza)
                {
                    cout << "El implemento: " << nuevo_implemento->nombre_implemento << " no puede ser usado por el personaje: " << personaje_a_llenar->nombre << endl;
                    cout << "Coloque uno valido.\n";
                }
                else
                {
                    contrilador_interno = 1;
                }
            } while (contrilador_interno != 1);
            // procedemos a colocar el implemento en la mochila.
            // cramos nuva memoria dinamica.
            Implemento *colocar = new Implemento;
            *colocar = *nuevo_implemento;
            // ahora la asignamos a la mochila.
            colocar->siguiente = personaje_a_llenar->mochila->implementos;
            personaje_a_llenar->mochila->implementos = colocar;
            cantidad_objetos += 1;
            contrilador_interno = 0; // poria.
            cout << "El implemento " << colocar->nombre_implemento << " se agrego correctamente a la mochila.\n";
        }
        else if (opcio == 2)
        {
            Poder_magico *nuevo_poder = nullptr;
            // para verificar si el ID es valido, lo pone bien o lo pone bien.
            do
            {
                cout << "Los poderes magicos disponibles son:\n";
                mostrar_poderes(poderes);
                cout << "coloque el ID del poder que desea colocar: ";
                identificador = obtener_entero("");
                nuevo_poder = encontrar_poder(poderes, identificador);
                if (nuevo_poder == nullptr)
                {
                    cout << "El ID: " << identificador << " no existe.\n";
                    cout << "coloque uno valido.\n";
                }
                else
                {
                    contrilador_interno = 1;
                }
            } while (contrilador_interno != 1);
            // procedemos a colocar el poder en la mochila.
            // cramos nuva memoria dinamica.
            Poder_magico *colocar = new Poder_magico;
            *colocar = *nuevo_poder;
            // ajhora la asignamos a la mochila.
            colocar->siguiente = personaje_a_llenar->mochila->poderes;
            personaje_a_llenar->mochila->poderes = colocar;
            cantidad_objetos += 1;
            contrilador_interno = 0; // porcia.
            cout << "El poder " << colocar->nombre_poder << " se agrego correctamente a la mochila.\n";
        }
        else if (opcio == 3)
        {
            cout << "La cantidad de objetos que coloco fue: " << cantidad_objetos << endl;
        }
        else
        {
            cout << "opcion invaida, el numero " << opcio << " no es una opcion valida.\n";
        }

    } while ((opcio != 3) && (cantidad_objetos != 5));
    if (cantidad_objetos == 5)
    {
        cout << "Ya no pudes agregar mas objetos.\n";
        cout << "La mochila del personaje " << personaje_a_llenar->nombre << " se lleno correctamente. ";
    }
}

// para editar los personajes
void modificar_mochila(personaje *personajes_jugar, Implemento &Implementos, Poder_magico &poderes)
{
    if (cantidad_personajes_jugar == 0)
    {
        cout << "No hay personajes en el equipo para modificar\n";
        cout << "Modificacion fallida\n";
        return;
    }

    int identificador;
    cout << "Los personajes que forman parte del equipo son: \n";
    mostrar_personajes_jugar(personajes_jugar);
    cout << "Coloque el ID del personaje cuya mochila desea modificar: ";
    identificador = obtener_entero("");

    personaje *actual = personajes_jugar;
    while ((actual != nullptr) && (actual->identificador != identificador))
    {
        actual = actual->siguiente;
    }

    if (actual != nullptr)
    {
        cout << "Proceda a llenar nuevamente la mochila del personaje: " << actual->nombre << endl;

        // Liberamos la memoria de los poderes actuales.
        Implemento *actual_implemento = actual->mochila->implementos;
        while (actual_implemento != nullptr)
        {
            Implemento *siguiente_implemento = actual_implemento->siguiente;
            delete actual_implemento;
            actual_implemento = siguiente_implemento;
        }
        actual->mochila->implementos = nullptr; // la ponemos vacia

        // Liberar mos la memoria de los poderes actuales.
        Poder_magico *actual_poder = actual->mochila->poderes;
        while (actual_poder != nullptr)
        {
            Poder_magico *siguiente_poder = actual_poder->siguiente;
            delete actual_poder;
            actual_poder = siguiente_poder;
        }
        actual->mochila->poderes = nullptr; // la ponemso vacia.

        // se llama a la funcion llenar mochila.
        llenar_mochila(actual, Implementos, poderes);
        cout << "Actualización de la mochila completa para el personaje: " << actual->nombre << endl;
    }
    else
    {
        cout << "El ID " << identificador << " no existe en el equipo.\n";
        cout << "Modificación fallida.\n";
    }
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES PODERES MAGICOS ---------------------------------------

// para crear poderes.
void crear_poder(Poder_magico &lista_poderes)
{
    Poder_magico *nuevo = new Poder_magico;
    cout << "Proceda a llenar las espesificaciones del nuevo poder. \n";
    nuevo->nombre_poder = devolver_string_verificada("Nombre: ");
    // lo coloco sin validar porque imagino que podra colocar numero y eso en la funcion del poder.
    cout << "Funcion: ";
    getline(cin, nuevo->funcion);
    cin.ignore();
    cantidad_poderes += 1;
    nuevo->identificador = cantidad_poderes + regulador_poderes;
    // lo agregamos a la lista enlazada.
    nuevo->siguiente = lista_poderes.siguiente;
    lista_poderes.siguiente = nuevo;
    cout << "El poder: " << nuevo->nombre_poder << "se creo correctamente.\n";
}

void destruir_poder_magico(Poder_magico &lista_poderes)
{
    Poder_magico *actual = lista_poderes.siguiente;
    while (actual != nullptr)
    {
        Poder_magico *eliminar = actual;
        actual = actual->siguiente;
        delete eliminar;
    }
    cantidad_poderes = 0;
    regulador_poderes = 0;
}

// para mostrar los poderes.
void mostrar_poderes(Poder_magico &lista_poderes)
{
    // para ver si hay poderes creados.
    if (cantidad_poderes == 0)
    {
        cout << "No hay poderes disponibles actualmente. \n";
        return;
    }
    Poder_magico *actual = lista_poderes.siguiente;
    cout << "La cantidad de poderes actualmente es: " << cantidad_poderes << endl
         << endl;
    while (actual != nullptr)
    {
        cout << "Nombre: " << actual->nombre_poder << endl;
        cout << "ID: " << actual->identificador << endl;
        cout << "Funcion: " << actual->funcion << endl
             << endl;
        actual = actual->siguiente;
    }
    cout << "No hay mas poderes disponibles.\n";
}

// para encontrar un poder
Poder_magico *encontrar_poder(Poder_magico &lista_poderes, int identificador)
{
    Poder_magico *encontrar = &lista_poderes;
    while (encontrar != nullptr)
    {
        if (encontrar->identificador == identificador)
        {
            return encontrar;
        }
        encontrar = encontrar->siguiente;
    }
    return encontrar = nullptr;
}

// para modificar los poderes.
void modificar_poder(Poder_magico &lista_poderes)
{
    if (cantidad_poderes == 0)
    {
        cout << "No hay poder magicos disponibles para modificar.\n";
        return;
    }
    int identificador = 0;
    Poder_magico *actualizar = nullptr;
    cout << "Los poderes disponibles son: \n";
    mostrar_poderes(lista_poderes);

    identificador = obtener_entero("Coloque el ID del poder magico que desea modificar: ");
    // para valiadar que el poder existe.
    actualizar = encontrar_poder(lista_poderes, identificador);
    if (actualizar == nullptr)
    {
        cout << "El ID que coloco no existe. \n";
        cout << "Modificacion fallida \n";
        return;
    }
    cout << "Proceda a actualizar el poder: " << actualizar->nombre_poder << endl;
    actualizar->nombre_poder = devolver_string_verificada("Nuevo Nombre: ");
    // lo coloco sin validar porque imagino que podra colocar numero y eso en la funcion del poder.
    cout << "Nueva Funcion: ";
    getline(cin, actualizar->funcion);

    cout << "Poder magico: " << actualizar->nombre_poder << " ha sido actualizado correctamente.\n";
}

// para eliminar un poder.
void eliminar_poder(Poder_magico &lista_poder)
{
    if (cantidad_poderes == 0)
    {
        cout << "No hay poderes magicos disponibles para eliminar. \n";
        cout << "Eliminacion fallida. \n";
        return;
    }
    int identificador = 0;
    Poder_magico *validar = nullptr;
    cout << "Los poderes magicos disponibles son: \n";
    mostrar_poderes(lista_poder);
    identificador = obtener_entero("Ingrese el ID del personaje a modificar: ");
    // para validar que e id exista.
    validar = encontrar_poder(lista_poder, identificador);
    // Si no existe:
    if (validar == nullptr)
    {
        cout << "El ID que coloco no existe.\n";
        cout << "Eliminacion fallida.\n";
        return;
    }
    // comienza el procedimiento para borrar el poder.
    Poder_magico *actual = lista_poder.siguiente;
    Poder_magico *anterior = nullptr;
    // en caso de que sea el primero de la lista.
    if (actual->identificador == identificador)
    {
        lista_poder.siguiente = actual->siguiente;
        cout << "el poder magico: " << actual->nombre_poder << " se elimino correctamente.\n ";
        delete actual;
        cantidad_poderes -= 1;
        regulador_poderes += 1;
        return;
    }
    // en caso de que no sea el primero.
    while ((actual != nullptr) && (actual->identificador != identificador))
    {
        anterior = actual;
        actual = actual->siguiente;
    }
    // ahora eliminamos.
    Poder_magico *eliminar = actual;
    anterior->siguiente = actual->siguiente;
    cout << "El poder magico: " << eliminar->nombre_poder << " se elimino correctamente.\n";
    delete eliminar;
    cantidad_poderes -= 1;
    regulador_poderes += 1;
}

// Pendiente revisar, mejorar e implementar en el programa
void poder_vigor_enano(personaje *lista_personajes_jugar)
{

    // Creacion del poder magico:

    // nuevo->nombre_poder = "Vigor de Enanos";
    // nuevo->funcion = "Poder magico que recupera la salud de todos los enanos.
    // Aumenta su salud en 100 y su fortaleza en 1000";

    personaje *actual_personaje = lista_personajes_jugar;
    bool modificado = false;

    if (actual_personaje == nullptr)
    {
        cout << "No hay personajes disponibles en el equipo" << endl;
    };
    while (actual_personaje != nullptr)
    {
        // SOLO FUNCIONA SI ENANO ESTA BIEN ESCRITO. "Enano" !!!!!!
        if (actual_personaje->tipo->nombre_especie == "Enano")
        {
            modificado = true;
            // Le recupera toda la vida (La devuelve a su valor original)
            actual_personaje->vitalidad = actual_personaje->tipo->salud;
            cout << "La vitalidad del personaje" << actual_personaje->nombre << "ha sido regenerada por completo (" << actual_personaje->vitalidad << ")" << endl;
            ;

            // Le aumenta la vida en 100
            actual_personaje->vitalidad += 100;
            cout << "La vitalidad del personaje" << actual_personaje->nombre << "ha sido aumentada en 100 (" << actual_personaje->vitalidad << ")" << endl;

            // Le aumenta la fortaleza en 1000
            actual_personaje->fortaleza += 1000;
            cout << "La fuerza del personaje ha sido aumentada en 1000 (" << actual_personaje->fortaleza << ")";

            actual_personaje = actual_personaje->siguiente;
        };
    };
    if (modificado == true)
    {
        cout << "Los personajes Enanos han sido modificados" << endl;
    }
    else
    {
        cout << "No hay personajes Enanos en el equipo" << endl;
    }
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES PERSONAJES JUGAR --------------------------------------

// para elegir el personaje y llenar la mochila.
void eleccion_personaje(personaje *&lista_jugar, personaje &heroes, Implemento &implementos, Poder_magico &poderes)
{
    if (cantidad_personaje_heroe <= 0)
    {
        cout << "No hay heroes disponibles para elegir.\n";
        cout << "Eleccion de personaje fallida \n";
        return;
    }
    if (cantidad_personajes_jugar == 4)
    {
        cout << "Ya tienes la cantidad de personajes maxima.\n";
        return;
    }
    if (cantidad_implementos == 0)
    {
        cout << "No hay implementos disponibles para equipar a los heroes.\n";
        cout << "Eleccion de personaje fallida.\n";
        return;
    }

    int identificador;
    personaje *seleccionado = nullptr;

    cout << "Los heroes disponibles son: \n";
    mostrar_personajes(heroes, 2);
    cout << "Coloque el ID del heroe que desea agregar al equipo: ";
    identificador = obtener_entero("");

    seleccionado = encontrar_personaje(heroes, identificador);

    if (seleccionado == nullptr)
    {
        cout << "El ID: " << identificador << " no existe.\n";
        cout << "Eleccion de personaje fallida.\n";
        return;
    }
    cantidad_personajes_jugar += 1;
    // se copia para evitar probles (daba error si no lo hacia asi)
    personaje *nuevo_para_jugar = new personaje;
    nuevo_para_jugar->nombre = seleccionado->nombre;
    nuevo_para_jugar->tipo = seleccionado->tipo;
    nuevo_para_jugar->identificador = cantidad_personajes_jugar + regulador_personajes_jugar; // Nuevo ID para el equipo
    nuevo_para_jugar->mochila = new mochila;
    nuevo_para_jugar->mochila->implementos = nullptr; // porsia
    nuevo_para_jugar->mochila->poderes = nullptr;
    nuevo_para_jugar->siguiente = nullptr;

    cout << "Proceda a llenar la mochila del personaje: " << nuevo_para_jugar->nombre << endl
         << endl;
    llenar_mochila(nuevo_para_jugar, implementos, poderes);

    // verrr la logica de incertar en un poco diferente a las demas funciones. los pone a final.
    // Insertar el nuevo personaje en la lista de personajes para jugar
    if (*&lista_jugar == nullptr)
    {
        *&lista_jugar = nuevo_para_jugar;
    }
    else
    {
        personaje *ultimo = *&lista_jugar;
        while (ultimo->siguiente != nullptr)
        {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = nuevo_para_jugar;
    }
    cout << "El heroe " << nuevo_para_jugar->nombre << " se agrago al equipo.\n";
}

// para mostrar con copia para no destruir la lista al mometo de mostrar.
void mostrar_personajes_jugar(personaje *lista_personajes_jugar)
{
    personaje *actual_personaje = lista_personajes_jugar;
    while (actual_personaje != nullptr)
    {
        cout << "Nombre: " << actual_personaje->nombre << endl;
        cout << "ID: " << actual_personaje->identificador << endl;
        cout << "Especie: " << actual_personaje->tipo->nombre_especie << endl; // Asumiendo que 'tipo' es un puntero a Especie
        // para la mochila.
        cout << "Objetos de la mochila: " << endl;

        Implemento *actual_implemento = actual_personaje->mochila->implementos;
        cout << "  Implementos: ";
        if (actual_implemento == nullptr)
        {
            cout << "Ninguno" << endl;
        }
        else
        {
            cout << endl;
            while (actual_implemento != nullptr)
            {
                cout << "    - " << actual_implemento->nombre_implemento << endl;
                actual_implemento = actual_implemento->siguiente;
            }
        }

        // Mostrar poderes de la mochila
        Poder_magico *actual_poder = actual_personaje->mochila->poderes;
        cout << "  Poderes Magicos: ";
        if (actual_poder == nullptr)
        {
            cout << "Ninguno" << endl;
        }
        else
        {
            cout << endl;
            while (actual_poder != nullptr)
            {
                cout << "    - " << actual_poder->nombre_poder << endl;
                actual_poder = actual_poder->siguiente;
            }
        }
        cout << endl;
        actual_personaje = actual_personaje->siguiente; // Pasa al siguiente personaje
    }
    cout << "\n No hay mas personajes en el equipo\n\n ";
}

void eliminar_personaje_jugar(personaje *&lista_jugar)
{
    if (cantidad_personajes_jugar == 0)
    {
        cout << "No hay personajes disponibles para eliminar. \n";
        cout << "Eliminacion fallida.\n";
        return;
    }

    int identificador = 0;
    cout << "Los personajes en el equipo son: \n";
    mostrar_personajes_jugar(lista_jugar);
    cout << "Coloque el ID del personaje que desea eliminar: ";
    identificador = obtener_entero("");

    personaje *actual = lista_jugar;
    personaje *anterior = nullptr;

    while (actual != nullptr && actual->identificador != identificador)
    {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual != nullptr)
    {
        // Liberar la memoria de la mochila del personaje a eliminar
        // implemento
        Implemento *actual_implemento = actual->mochila->implementos;
        while (actual_implemento != nullptr)
        {
            Implemento *siguiente_implemento = actual_implemento->siguiente;
            delete actual_implemento;
            actual_implemento = siguiente_implemento;
        }
        // poder magico.
        Poder_magico *actual_poder = actual->mochila->poderes;
        while (actual_poder != nullptr)
        {
            Poder_magico *siguiente_poder = actual_poder->siguiente;
            delete actual_poder;
            actual_poder = siguiente_poder;
        }

        // Eliminar el nodo del personaje de la lista
        // en caso sea el primer nodo.
        if (anterior == nullptr)
        {
            lista_jugar = actual->siguiente;
        }
        else
        { // en caso de que no lo sea
            anterior->siguiente = actual->siguiente;
        }

        cout << "Eliminacion del personaje " << actual->nombre << " exitosa.\n";
        delete actual;
        cantidad_personajes_jugar--;
    }
    else
    {
        cout << "El ID " << identificador << " No existe en el equipo. \n";
        cout << "Eliminacion fallida.\n";
    }
}

// para destrir todooo, los personajes con sus mochilas.
void destruir_personaje_jugar(personaje *&lista_jugar)
{
    personaje *actual = lista_jugar;
    while (actual != nullptr)
    {
        // para liberar la memoria de la mochila.
        // Implementos.
        Implemento *actual_implemento = actual->mochila->implementos;
        while (actual_implemento != nullptr)
        {
            Implemento *siguiente_implemento = actual_implemento->siguiente;
            delete actual_implemento;
            actual_implemento = siguiente_implemento;
        }
        // poderes.
        Poder_magico *actual_poder = actual->mochila->poderes;
        while (actual_poder != nullptr)
        {
            Poder_magico *siguiente_poder = actual_poder->siguiente;
            delete actual_poder;
            actual_poder = siguiente_poder;
        }
        personaje *eliminar = actual;
        actual = actual->siguiente;
        delete eliminar;
    }
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES MAPA ---------------------------------------

// Hay que modificar TOODOOO esto

// para el mapa.

// crear sala con sus adyacencias.
void crear_sala(sala &lista_sala)
{
    sala *nueva_sala = new sala;
    int controlador = 0;       // para controlar si el usuario quiere colocar adyacencias.
    int controlador_bucle = 0; // para controlar el bucle.

    cout << "Proceda a llenar los datos de la sala.\n";
    cantidad_salas += 1;
    nueva_sala->nombre = devolver_string_verificada("Nombre: ");
    nueva_sala->distancia = obtener_entero("Distancia: "); // Corrección: asignación del valor
    nueva_sala->energia_necesaria = obtener_entero("Energia necesaria para ir a la sala: ");
    nueva_sala->adyacencia_siguiente = nullptr; // Inicializar la lista de adyacencias
    nueva_sala->cantidad_adyacencias = 0;
    nueva_sala->identificador_sala = cantidad_salas + regulador_salas;

    do
    {
        cout << "Desea agregar adyacencias a la sala " << nueva_sala->nombre << ":\n";
        cout << "1. Si.\n";
        cout << "2. No.\n";
        controlador = obtener_entero("");
        if (controlador == 1)
        {
            cantidad_adyacencias += 1;
            adyacencia *nueva_adyacencia = new adyacencia;
            cout << "Proceda a llenar los datos de la nueva adyacencia. \n";
            nueva_adyacencia->nombre = (nueva_sala->nombre) + (". ") + (devolver_string_verificada("Nombre: "));
            nueva_adyacencia->distancia = obtener_entero("Distancia: ");
            nueva_adyacencia->energia_requerida = obtener_entero("Energía necesaria para ir a la adyacencia: ");
            nueva_adyacencia->identificador_adyacencia = cantidad_adyacencias + regulador_adyacencias; // ID único

            // lo colocamos en la lsita enlazada, parecido a lo de la mochila.
            nueva_adyacencia->siguiente = nueva_sala->adyacencia_siguiente; // Nuevo nodo apunta al antiguo primero
            nueva_sala->adyacencia_siguiente = nueva_adyacencia;            // La cabeza ahora es el nuevo nodo
            nueva_sala->cantidad_adyacencias += 1;                          // le sumams a la acntidad de adyacencias

            cout << "La adyacencia " << nueva_adyacencia->nombre << " se agregó correctamente.\n";
        }
        else if (controlador == 2)
        {
            controlador_bucle = 2; // Salir del bucle
            cout << "La cantidad de adyacencias que colocó fue: " << nueva_sala->cantidad_adyacencias << endl;
        }
        else
        {
            cout << "Opción inválida, el número: " << controlador << " no es una opción.\n";
            cout << "Intente nuevamente.\n";
        }
    } while (controlador_bucle != 2);

    // Colocamos la nueva sala en la lista de salas.
    nueva_sala->sala_siguiente = lista_sala.sala_siguiente;
    lista_sala.sala_siguiente = nueva_sala;
    // La coloca al inicio de la lista.
    cout << "La sala " << nueva_sala->nombre << " con " << nueva_sala->cantidad_adyacencias << " adyacencias se agregó correctamente.\n";
    // listo.
}

// para mostra el mapa.
void mostra_mapa(sala lista_salas)
{ // una copia para no tenre priblrmas a la hora de las direcciones.
    if (cantidad_salas == 0)
    {
        cout << " No hay salas para mostrar.\n";
        return;
    }

    sala *actual = lista_salas.sala_siguiente;
    while (actual != nullptr)
    {
        cout << "Sala Numero " << actual->identificador_sala << endl;
        cout << "Nombre: " << actual->nombre << endl;
        // cout << "ID: " << actual->identificador_sala << endl;
        cout << "Distancia: " << actual->distancia << endl;
        cout << "Energia necesaria para para ir: " << actual->energia_necesaria << endl;
        if (actual->cantidad_adyacencias != 0)
        {
            cout << "Cantidad de salas adyacentes: " << actual->cantidad_adyacencias << endl;
            cout << "\nAdyacencias: \n";
            adyacencia *adyacencia_actual = actual->adyacencia_siguiente;
            while (adyacencia_actual != nullptr)
            {

                cout << "Adyacencia Numero " << adyacencia_actual->identificador_adyacencia << endl;
                cout << "Nombre: " << adyacencia_actual->nombre << endl;
                // cout << "ID: : " << adyacencia_actual->identificador_adyacencia << endl;
                cout << "Distancia: " << adyacencia_actual->distancia << endl;
                cout << "Energia necesaria para para ir: " << adyacencia_actual->energia_requerida << endl;
                cout << "--------------\n";
                adyacencia_actual = adyacencia_actual->siguiente;
            }
            cout << "La sala " << actual->nombre << " no tiene mas adyacencias.\n\n";
        }
        else
        {
            cout << "La sala no tiene adyacencias.\n";
        }
        cout << "--------------\n";
        actual = actual->sala_siguiente;
    }
    cout << "No hay mas salas disponibles> \n";
}

// para encontrar una sal.
sala *encontrar_sala(sala &lista_sala, int identificador)
{
    sala *buscar = lista_sala.sala_siguiente;
    while ((buscar != nullptr))
    {
        if (buscar->identificador_sala == identificador)
        {
            return buscar;
        }
        buscar = buscar->sala_siguiente;
    }
    return buscar = nullptr; // si no lo encuentra retorna nul.
}

// para encontra adyacencia.
adyacencia *encontra_adyacencia(adyacencia *&lista_adyacencias, int identificador)
{
    adyacencia *actual = lista_adyacencias; // Comenzar desde la cabeza de la lista (esto me confunde fulll)
    while (actual != nullptr)
    {
        if (actual->identificador_adyacencia == identificador)
        {
            return actual;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

void modificar_sala(sala &lista_sala)
{
    if (cantidad_salas == 0)
    {
        cout << " No hay salas para modificar.\n";
        return;
    }

    int identificador;
    int opcion_adyacencia;
    sala *modificar = nullptr;

    cout << "Las salas disponibles son: \n";
    mostra_mapa(lista_sala);
    cout << "Coloque el ID de la sala que desea modificar: ";
    identificador = obtener_entero("");

    modificar = encontrar_sala(lista_sala, identificador);
    if (modificar == nullptr)
    { // para ver si si existe la sala
        cout << "El ID " << identificador << " no existe.\n";
        cout << "Modificación fallida.\n";
        return;
    }

    // Procedemos a la modificación de los datos básicos de la sala.
    cout << "Proceda a modificar la sala: " << modificar->nombre << endl;
    modificar->nombre = devolver_string_verificada("Nuevo nombre: ");
    modificar->distancia = obtener_entero("Nueva distancia: ");
    modificar->energia_necesaria = obtener_entero("Nueva energía necesaria para ir: ");

    cout << "\nModificacion de Adyacencias para la sala: " << modificar->nombre << "\n";
    do
    {
        cout << "\n¿Qué desea hacer con las adyacencias?\n";
        cout << "1. Modificar adyacencias existentes.\n";
        cout << "2. Agregar nueva adyacencia.\n";
        cout << "3. No modificar adyacencias y finalizar.\n";
        opcion_adyacencia = obtener_entero("");

        if (opcion_adyacencia == 1)
        {
            if (modificar->adyacencia_siguiente == nullptr)
            {
                cout << "No hay adyacencias para modificar.\n";
            }
            else
            {
                // para identificar cual es la adyacencia que desea modificar.
                int identificador = 0;
                adyacencia *adyacencia_actual = modificar->adyacencia_siguiente;
                cout << "Las adyacencias disponibles para modificar son: \n";
                while (adyacencia_actual != nullptr)
                { // para mostrar las adyacecias.
                    cout << "Nombre" << adyacencia_actual->nombre << endl;
                    cout << "ID: " << adyacencia_actual->identificador_adyacencia;
                    cout << "Distancia: " << adyacencia_actual->distancia << endl;
                    cout << "energia necesaria para ir: " << adyacencia_actual->energia_requerida << endl;
                    cout << "---------otra--------\n";
                    adyacencia_actual = adyacencia_actual->siguiente;
                }
                adyacencia_actual = modificar->adyacencia_siguiente; // la ponemos en el principio para pasarla a la funcion.
                adyacencia *modificar_adyacencia = nullptr;
                cout << "coloque el identificador de la adyacencia que desea moificar: ";
                identificador = obtener_entero("");
                modificar_adyacencia = encontra_adyacencia(adyacencia_actual, identificador);

                if (modificar_adyacencia == nullptr)
                {
                    cout << "El ID " << identificador << " no exite";
                    cout << "Modificacion de las adyacencias de la sala " << modificar->nombre << " fallida.\n";
                    return;
                }
                cout << "Proceda a modificar la adyacencia: " << modificar_adyacencia->nombre;
                modificar_adyacencia->nombre = (modificar->nombre) + (". ") + (devolver_string_verificada("Nuevo nombre:"));
                modificar_adyacencia->distancia = obtener_entero("Nueva distancia: ");
                modificar_adyacencia->energia_requerida = obtener_entero("Nueva energía necesaria para ir a la adyacencia:");

                cout << "actualizacion de la adyacencia " << modificar_adyacencia->nombre << " exitosa.\n";
            }
        }
        else if (opcion_adyacencia == 2)
        {
            adyacencia *nueva_adyacencia = new adyacencia;
            cout << "Proceda a llenar los datos de la nueva adyacencia. \n";
            nueva_adyacencia->nombre = (modificar->nombre) + (". ") + (devolver_string_verificada("Nombre:"));
            nueva_adyacencia->distancia = obtener_entero("Distancia: ");
            nueva_adyacencia->energia_requerida = obtener_entero("Energía necesaria para ir a la adyacencia:");
            nueva_adyacencia->identificador_adyacencia = modificar->cantidad_adyacencias + 1; // ID único para esta sala
            nueva_adyacencia->siguiente = modificar->adyacencia_siguiente;
            modificar->adyacencia_siguiente = nueva_adyacencia;
            modificar->cantidad_adyacencias++;
            cout << "La adyacencia " << nueva_adyacencia->nombre << " se agregó correctamente.\n";
        }
        else if (opcion_adyacencia == 3)
        {
            cout << "Modificación de adyacencias finalizada.\n";
        }
        else
        {
            cout << "Opción inválida.\n";
        }
    } while (opcion_adyacencia != 3);

    cout << "Sala " << modificar->nombre << " modificada exitosamente.\n";
}

// para eliminar eliminar salas y adyacencias
void eliminar_sala_adyacencia(sala &lista_sala)
{
    if (cantidad_salas == 0)
    {
        cout << " No hay salas para eliminar.\n";
        return;
    }
    int identificador = 0;
    sala *validar_existencia = nullptr;

    cout << "Las salas disponibles son: \n";
    mostra_mapa(lista_sala);
    cout << "Coloque el identificador de la sala que quiere borrar: ";
    identificador = obtener_entero("");
    validar_existencia = encontrar_sala(lista_sala, identificador);
    // validamos de que exista la sala.
    if (validar_existencia == nullptr)
    {
        cout << "El ID " << identificador << " no existe.\n ";
        cout << "Eliminacion fallida. \n";
        return;
    }

    int controlador = 0;
    // para ver que quiere hacer.
    do
    {
        cout << "Desea eliminar la sala o una adyacencia de la sala.\n";
        cout << "1. Eliminar sala.\n";
        cout << "2. Eliminar adyacencia de la sala.\n";
        controlador = obtener_entero("(1 o 2): ");
    } while (controlador != 1 && controlador != 2);

    if (controlador == 1)
    { // Eliminar sala
        sala *actual = lista_sala.sala_siguiente;
        sala *anterior = &lista_sala; // apunta a la cabeza de la lista.

        while (actual != nullptr && actual->identificador_sala != identificador)
        {
            anterior = actual;
            actual = actual->sala_siguiente;
        }

        // liberamos memoria dinamica de las adyacencias.
        if (actual != nullptr)
        {
            // Liberar adyacencias de la sala
            adyacencia *eliminar_adyacencia = actual->adyacencia_siguiente;
            while (eliminar_adyacencia != nullptr)
            {
                adyacencia *siguiente = eliminar_adyacencia->siguiente;
                delete eliminar_adyacencia;
                eliminar_adyacencia = siguiente;
            }

            //  en caso de que sea el primer elemento de la lista. (si no cambio la direccion a la que apunta).
            if (anterior == &lista_sala)
            {
                lista_sala.sala_siguiente = actual->sala_siguiente;
            }
            else
            { // en caso no sea el primer elemnto (en el caso de que si cambio la direccion a la que apunta.)

                anterior->sala_siguiente = actual->sala_siguiente;
            }
            // procedemos a eliminar.
            cout << "La sala " << actual->nombre << " se eliminó correctamente.\n";
            delete actual;
            cantidad_salas -= 1;
            regulador_salas += 1;
        }
        else
        {
            // porcia , esta vina esta loca. ( no deberia pasar.)
            cout << "Error: Sala con ID " << identificador << " no encontrada\n";
            return;
        }
    }
    else
    { // eliminar adyacencia de la sala
        // en caso no existan adyacencias en la sala.
        if (validar_existencia->cantidad_adyacencias == 0)
        {
            cout << "La sala " << validar_existencia->nombre << " no tiene adyacencias.\n";
            cout << "Eliminación de las adyacencias fallida.\n";
            return;
        }

        adyacencia *actual_adyacencia = validar_existencia->adyacencia_siguiente;
        adyacencia *anterior_adyacencia = nullptr;
        int identificador_adyacencia = 0;

        cout << "Las adyacencias disponibles son: \n";
        // mostramos las adyacencias disponibles.
        while (actual_adyacencia != nullptr)
        {
            cout << "Nombre: " << actual_adyacencia->nombre << endl;
            cout << "ID: " << actual_adyacencia->identificador_adyacencia << endl;
            cout << " Distancia: " << actual_adyacencia->distancia << endl;
            cout << "energia necesaria para ir: " << actual_adyacencia->energia_requerida << endl;
            cout << "---------otra--------\n";

            actual_adyacencia = actual_adyacencia->siguiente;
        }

        cout << "Coloque el identificador de la adyacencia a eliminar: ";
        identificador_adyacencia = obtener_entero("");

        // inicializamos en la primera.
        actual_adyacencia = validar_existencia->adyacencia_siguiente;
        while (actual_adyacencia != nullptr && actual_adyacencia->identificador_adyacencia != identificador_adyacencia)
        {
            anterior_adyacencia = actual_adyacencia;
            actual_adyacencia = actual_adyacencia->siguiente;
        }

        // si si encontro la adyacencia.
        if (actual_adyacencia != nullptr)
        {
            // igual que antes, si es la primera adyacencia. (si no cambio la direccion de memoria)
            if (anterior_adyacencia == nullptr)
            {
                validar_existencia->adyacencia_siguiente = actual_adyacencia->siguiente;
            }
            else
            { // en caso no sea el primer elemento.
                anterior_adyacencia->siguiente = actual_adyacencia->siguiente;
            }
            // procedemos a leinar.
            cout << "La adyacencia " << actual_adyacencia->nombre << " se eliminó correctamente de la sala " << validar_existencia->nombre << ".\n";
            delete actual_adyacencia;
            validar_existencia->cantidad_adyacencias--;
        }
        else
        {
            // en caso no se encuetre. no deberia pasar creo.
            cout << "El ID de la adyacencia " << identificador_adyacencia << " no se encontró en la sala " << validar_existencia->nombre << ".\n";
            cout << "Eliminacion  de la adyacencia fallida.\n";
            return;
        }
    }
}

// para destruir todo.
void destruir_mapa(sala &lista_mapa)
{
    sala *actual = lista_mapa.sala_siguiente; // primer elemento.
    while (actual != nullptr)
    {
        // Para eliminar las adyacencias.
        adyacencia *adyacencia_actual = actual->adyacencia_siguiente;
        while (adyacencia_actual != nullptr)
        {
            adyacencia *eliminar = adyacencia_actual;
            adyacencia_actual = adyacencia_actual->siguiente;
            delete eliminar;
        }
        sala *eliminar_sala = actual;
        actual = actual->sala_siguiente;
        delete eliminar_sala;
    }
}

//-----------------------------------------------------------------------------------------------------
//----------------------------------- FUNCIONES JUGABILIDAD -------------------------------------------

// Aqui va todo lo que sea juego como tal, menu de acciones, para usar objetos, etc

//-----------------------------------------------------------------------------------------------------
//------------------------------ EJECUCION DEL PROGRAMA -----------------------------------------------

int main()
{

    Lista_especie tipoEspecieOrco;  // lista enlazada que contiene todos los tipos de orcos.
    Lista_especie tipoEspecieHeroe; // lista enlazada que contiene todos los tipos de heroes.

    personaje personajes_orco; // lista enlazada de personajes orcos.
    personaje personajes_hero; // lista enlazada de personajes heroes.

    Implemento lista_implementos;      // lista de implementos
    Poder_magico lista_podere_magicos; // lista de los poderes.

    // lo plantee diferente por que me explotaba y me tenia locooooooooooooo.
    personaje *personajes_jugar = nullptr; // aqui estan los 4 personajes que el usuaro coloque para jugar.

    sala mapa; // tipo sala ya que sera el nodo a la cabeza de la sala

    int opcion_principal = 0;
    int opcion_interna = 0;

    cout << "¡¡¡ Bienvenido al juego Khazad-Dum !!!" << endl;
    cout << "Se recomienda visitar todos los menus en orden ascendente para una mejor experiencia de juego." << endl;
    cout << "¿Que desea hacer? \n";

    // Menu mientras.
    do
    {
        cout << "\n MENU PRINCIPAL \n";
        cout << "---------------------\n";
        cout << "1. Menu de Especies de Orcos" << "\n";
        cout << "2. Menu de Especies de Heroes y Personajes" << "\n";
        cout << "---------------------\n";
        cout << "3. Menu de Implementos" << "\n";
        cout << "4. Menu de Poderes Magicos \n";
        cout << "5. Seleccion de Personajes.\n"; // aqui hay que darle dos veces a la opcion salir
        cout << "---------------------\n";
        cout << "6. Editar Mapa.\n";
        cout << "7. Salir de Menu Principal" << "\n";
        cout << "---------------------\n";
        opcion_principal = obtener_opcion();

        // Switch para Menu Principal
        switch (opcion_principal)
        {
        // Menu Orcos
        case 1:
            do
            {
                cout << "\n MENU ORCOS\n";
                cout << "---------------------\n";
                cout << "1. Agregar una especie de Orco. \n";
                cout << "2. Actualizar los datos de un tipo de Orco. \n";
                cout << "3. Mostrar los tipos de Orcos disponibles. \n";
                cout << "4. Eliminar un tipo de especie. \n";
                cout << "------------------------\n";
                cout << "5. Crear personajes Orco. \n";
                cout << "6. Mostrar personajes Orco. \n";
                cout << "7. Modificar personajes Orco. \n";
                cout << "8. Eliminar personajes Orco.  \n";
                cout << "------------------------\n";
                cout << "9.Salir al menu principal.\n";
                cout << "---------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    Crear_tipo(tipoEspecieOrco, 1);
                    break;
                case 2:

                    actualizar_tipo(tipoEspecieOrco, 1);
                    break;
                case 3:
                    mostrar_lista(tipoEspecieOrco, 1);
                    break;
                case 4:

                    eliminar_elemento_lista(tipoEspecieOrco, personajes_orco, 1);
                    break;
                case 5:
                    crear_personaje(personajes_orco, tipoEspecieOrco, 1);
                    break;

                case 6:
                    mostrar_personajes(personajes_orco, 1);
                    break;
                case 7:
                    actualizar_personaje(personajes_orco, tipoEspecieOrco, 1);
                    break;
                case 8:
                    borrar_personaje(personajes_orco, 1);
                    break;
                case 9:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 9); // Mientras no se salga del menu interno de orcos.
            break;

        // Menu Heroes
        case 2:
            do
            {
                cout << "\n MENU DE HEROES Y PERSONAJES\n";
                cout << "---------------------\n";
                cout << "1. Agregar una especie de Heroe \n";
                cout << "2. Actualizar los datos de un tipo de Heroe \n";
                cout << "3. Mostrar los tipos de Heroes disponibles \n";
                cout << "4. Eliminar un tipo de especie \n"; // Ya no sale del programa sino del menu de creacion, pq luego vendra el juego como tal.
                cout << "---------------------\n";
                cout << "5. Crear personaje.\n";
                cout << "6. Mostrar personajes. \n";
                cout << "7. Modificar personaje \n";
                cout << "8. Eliminar personaje  \n";
                cout << "---------------------\n";
                cout << "9. Salir al menu principal  \n";
                cout << "---------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    Crear_tipo(tipoEspecieHeroe, 2);
                    break;
                case 2:

                    actualizar_tipo(tipoEspecieHeroe, 2);
                    break;
                case 3:
                    mostrar_lista(tipoEspecieHeroe, 2);
                    break;
                case 4:
                    eliminar_elemento_lista(tipoEspecieHeroe, personajes_hero, 2);
                    break;
                case 5:
                    crear_personaje(personajes_hero, tipoEspecieHeroe, 2);
                    break;
                case 6:
                    mostrar_personajes(personajes_hero, 2);
                    break;
                case 7:
                    actualizar_personaje(personajes_hero, tipoEspecieHeroe, 2);
                    break;
                case 8:
                    borrar_personaje(personajes_hero, 2);
                    break;
                case 9:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 9); // Mientras no se salga del menu interno de heroes.
            break;

        // Menu Implementos
        case 3:
            do
            {
                cout << "\n MENU DE IMPLEMENTOS\n";
                cout << "------------------------\n";
                cout << "1. Agregar un implemento. \n";
                cout << "2. Actualizar un implemento. \n";
                cout << "3. Mostrar los implementos. \n";
                cout << "4. Eliminar un implemento \n";
                cout << "------------------------\n";
                cout << "5. Salir al menu principal  \n";
                cout << "------------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    crear_implemento(lista_implementos);
                    break;
                case 2:
                    modificar_implemento(lista_implementos);
                    break;
                case 3:
                    mostrar_implementos(lista_implementos);
                    break;
                case 4:
                    borrar_implementos(lista_implementos);
                    break;
                case 5:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 5);
            break;

        case 4:
            do
            {
                cout << "\n MENU DE PODERES MAGICOS\n";
                cout << "---------------------\n";
                cout << "1. Agregar poder magico. \n";
                cout << "2. Actualizar poder magico. \n";
                cout << "3. Mostrar los poderes magicos. \n";
                cout << "4. Eliminar un poder magico. \n";
                cout << "---------------------\n";
                cout << "5. Salir al menu principal  \n";
                cout << "---------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    crear_poder(lista_podere_magicos);
                    break;
                case 2:
                    modificar_poder(lista_podere_magicos);
                    break;
                case 3:
                    mostrar_poderes(lista_podere_magicos);
                    break;
                case 4:
                    eliminar_poder(lista_podere_magicos);
                    break;
                case 5:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 5);
            break;

        case 5:
            do
            {
                cout << "\n MENU DE SELECCION DE PERSONAJE\n";
                cout << "---------------------\n";
                cout << "1. Elegir personaje y objetos \n"; // elige el personaje y llena la mochila.
                cout << "2. Mostrar personaje del equipo. \n";
                cout << "3. Modificar mochila. \n";
                cout << "4. Eliminar personaje. \n";
                cout << "---------------------\n";
                cout << "5. Salir al menu principal  \n";
                cout << "---------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    eleccion_personaje(personajes_jugar, personajes_hero, lista_implementos, lista_podere_magicos);
                    break;
                case 2:
                    mostrar_personajes_jugar(personajes_jugar);
                    break;
                case 3:
                    modificar_mochila(personajes_jugar, lista_implementos, lista_podere_magicos);
                    break;
                case 4:
                    eliminar_personaje_jugar(personajes_jugar);
                    break;
                case 5:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 5);
            break;

        case 6:
            do
            {
                cout << "\n MENU DE MAPAS:\n";
                cout << "---------------------\n";
                cout << "1. Agregar sala al mapa \n"; // elige el personaje y llena la mochila.
                cout << "2. Mostrar mapa. \n";
                cout << "3. Modificar mapa. \n";
                cout << "4. Eliminar sala del mapa o su adyacencia. \n";
                cout << "---------------------\n";
                cout << "5. Salir al menu principal  \n";
                cout << "---------------------\n";
                opcion_interna = obtener_opcion();

                switch (opcion_interna)
                {
                case 1:
                    crear_sala(mapa);
                    break;
                case 2:
                    mostra_mapa(mapa);
                    break;
                case 3:
                    modificar_sala(mapa);
                    break;
                case 4:
                    eliminar_sala_adyacencia(mapa);
                    break;
                case 5:
                    cout << "\nSaliendo al Menu Principal... \n";
                    break;
                default:
                    cout << "Invalido. Ingrese una opcion valida \n";
                    break;
                }
            } while (opcion_interna != 5);
            break;

        case 7:
            cout << "\nSaliendo del Menu Principal... \n";
            cout << "Entrando al Juego... \n";
            cout << "\nJuego en desarrollo, paciencia por favor... \n";
            break;

        // Default del Menu Principal:
        default:
            cout << "Invalido. Ingrese una opcion valida";
            break;
        }

    } while (opcion_principal != 7);

    // Dejar esto al final del programa para que se destruyan las listas enlazadas.
    // Hay que añadir todos los tipos de listas que se vayan creando.
    // Destructor de especies
    destruir_lista_especie(tipoEspecieHeroe);
    destruir_lista_especie(tipoEspecieOrco); // se destruye la lista al final del programa.

    // Destructor de personajes
    destruir_lista_personajes(personajes_orco);
    destruir_lista_personajes(personajes_hero);

    // Destructor de implementos, personajes para jugar, poderes y mapa
    destruir_implementos(lista_implementos);

    destruir_poder_magico(lista_podere_magicos);

    destruir_personaje_jugar(personajes_jugar);

    destruir_mapa(mapa);

    cout << "Gracias por usar el programa. \n";
    return 0;
}
// fin del programa.
