#include <iostream>
#include <cstring>
using namespace std;

#pragma region Estructuras

struct Fila
{
    int codigo;
    char nombre[50];
    char nombreCorrelativa[50] = "Ninguna";
    char cursadaAprobada[3] = "-";
    char finalAprobado[3] = "-";
};

struct NodoCorrelativa
{
    int codMateria;
    bool cursadaAprobada;
    bool finalAprobado;
    NodoCorrelativa *sig;
};

struct Materia
{
    int codigo;
    char nombre[50];
    NodoCorrelativa *correlativas;
    Materia *sig;
};

struct NodoBusqueda
{
    Materia *materia;
    NodoBusqueda *sig;
};

#pragma endregion Estructuras

#pragma region Funciones
// Funcionales
void simplificarString(char[]);
void cargarMateria(FILE *, Materia *&);
void cargarCorrelativas(FILE *, Materia *&);
void obtenerCorrelativas(Materia *, Materia *, Materia *&);
Materia convertirFilaMateria(Fila, Materia *);
NodoCorrelativa convertirFilaCorrelativa(Materia *, Fila);
Materia *importarMaterias(char[]);
int contarCorrelativas(Materia *, Materia *);

// Funciones de Busqueda
void buscarArbol(Materia *);
Materia *buscarNombre(Materia *, char[]);
Materia *busquedaNombre(Materia *, char[]);
Materia *busquedaCodigo(Materia *, int);

// Funciones de Impresion
void mostrarArbolMateria(Materia *, Materia *);
void mostrarArbol(Materia *);
void mostrarCorrelativas(Materia *, Materia *, int, char[]);

// Funciones de Listas
void agregarMateria(Materia *&, Materia);
void agregarCorrelativa(NodoCorrelativa *&, NodoCorrelativa);
void borrarMateria(Materia *&, Materia);
void borrarIguales(Materia *&, Materia *);
void duplicarLista(Materia *&, Materia *);
void borrarLista(Materia *&);
void borrarNodoLista(NodoBusqueda *&);
void agregarNodo(NodoBusqueda *&, Materia *);

#pragma endregion Funciones

const int DISTANCIA_RAMAS = 8;

int main()
{
    // Importar registros de Fila
    char ruta[] = "./";
    char nombreArchivo[] = "Materias.bin";
    Materia *listaMaterias = importarMaterias(strcat(ruta, nombreArchivo));

    if (!listaMaterias)
    {
        cout << "ERROR 2: El Archivo no se pudo abrir" << endl;
        return 2;
    }

    int desicion = 1;
    do
    {
        cout << "Que desea hacer?" << endl
             << "1.Buscar una materia" << endl
             << "2.Mapa de todas las materias" << endl
             << "3.Salir" << endl;
        cin >> desicion;

        switch (desicion)
        {
        case 1:
        {
            buscarArbol(listaMaterias);
            break;
        }

        case 2:
        {
            // TODO: Mostrar el Arbol de todas las materias(Empezar de las que mas tienen correlativas)
            mostrarArbol(listaMaterias);
            break;
        }

        case 3:
        {
            // Salir del Programa
            cout << "Adios!! :D" << endl;
            borrarLista(listaMaterias);
            break;
        }

        default:
            cout << "Error 1 : Ingrese un valor valido porfavor(1, 2, 3)" << endl;
            break;
        }
    } while (desicion != 3);

    return 0;
}

void buscarArbol(Materia *listaMaterias)
{
    // Busqueda de una materia por consola
    Materia *materia = NULL;
    int rta = 0;
    do
    {
        cout << "Buscar materias por Codigo o por palabra Clave?" << endl
             << "1.Palabra clave" << endl
             << "2.Codigo" << endl;
        cin >> rta;

        switch (rta)
        {
        case 1:
        {
            // Opcion 1: Por palabra clave
            char nombre[50];
            cout << "Ingrese una palabra clave de la materia que desea buscar: ";
            cin >> nombre;
            materia = buscarNombre(listaMaterias, nombre);
            break;
        }

        case 2:
        {
            // Opcion 2: Por Codigo
            int codigo = 0;
            cout << "Ingrese el codigo de la materia que desea buscar: ";
            cin >> codigo;
            materia = busquedaCodigo(listaMaterias, codigo);
            break;
        }

        default:

            // Respuesta inválida
            cout << "Error 1: Ingrese un valor valido porfavor(1,2)" << endl
                 << endl;
            break;
        }
        if (!materia)
        {
            cout << "ERROR 3: Materia no encontrada, Ingrese una materia valida" << endl
                 << endl;
            rta = 0;
        }
    } while (rta != 1 && rta != 2);

    // Muestra el arbol de una materia por consola
    mostrarArbolMateria(materia, listaMaterias);
}

void mostrarArbol(Materia *lista)
{
    Materia *lista_Aux = NULL;
    duplicarLista(lista_Aux, lista);
    while (lista_Aux != NULL)
    {
        cout << endl;
        // buscar la materia con mas correlativas
        Materia *maxCorrelativas = lista_Aux;

        Materia *materia = lista_Aux;
        while (materia != NULL)
        {
            int correlativas = contarCorrelativas(lista, materia);
            if (correlativas > contarCorrelativas(lista, maxCorrelativas))
            {
                maxCorrelativas = materia;
            }

            materia = materia->sig;
        }

        // graficar su arbol
        mostrarArbolMateria(maxCorrelativas, lista);

        // Obtener lista de las materias que son correlativas a maxCorrelativas
        Materia *listaCorrelativas = NULL;
        agregarMateria(listaCorrelativas, *maxCorrelativas);
        obtenerCorrelativas(lista, maxCorrelativas, listaCorrelativas);

        // Elimino las coincidentes entre la lista auxiliar y las correlativas
        borrarIguales(lista_Aux, listaCorrelativas);

        // Repetir pero sin las coincidentes
    }

    borrarLista(lista_Aux);
    return;
}

Materia *importarMaterias(char nombre[])
{

    Materia *lista = NULL;

    FILE *archivo = fopen(nombre, "rb");

    if (!archivo)
    {
        return NULL;
    }

    while (!feof(archivo))
    {
        // Importar las Materias
        cargarMateria(archivo, lista);
    }

    archivo = fopen(nombre, "rb");

    while (!feof(archivo))
    {
        // Importar las correlativas
        cargarCorrelativas(archivo, lista);
    }

    fclose(archivo);
    return lista;
}

void cargarMateria(FILE *archivo, Materia *&lista)
{
    // Cargar en estructura Fila
    Fila fila;
    fread(&fila, sizeof(Fila), 1, archivo);

    Materia materia = convertirFilaMateria(fila, lista);

    if (busquedaCodigo(lista, materia.codigo))
    {
        return;
    }

    agregarMateria(lista, materia);
}

void cargarCorrelativas(FILE *archivo, Materia *&lista)
{
    // Cargar en estructura Fila
    Fila fila;
    fread(&fila, sizeof(Fila), 1, archivo);

    // buscar materia con el mismo codigo
    Materia *Twin = busquedaCodigo(lista, fila.codigo);

    if (Twin != NULL && busquedaNombre(lista, fila.nombreCorrelativa) != NULL)
    {
        // Mismo codigo entonces adjuntar una nueva Correlativa a la lista de la materia
        NodoCorrelativa correlativa = convertirFilaCorrelativa(lista, fila);
        agregarCorrelativa(Twin->correlativas, correlativa);
    }
}

Materia convertirFilaMateria(Fila fila, Materia *lista)
{
    // Cargar Codigo y nombre en la materia
    Materia materia;

    materia.codigo = fila.codigo;
    char nombre[50];
    strcpy(nombre, fila.nombre);
    simplificarString(nombre);
    strcpy(materia.nombre, nombre);
    materia.correlativas = NULL;

    return materia;
}

NodoCorrelativa convertirFilaCorrelativa(Materia *lista, Fila fila)
{
    // Buscar la Correlativa por nombre y cargarlo en la correlativa
    Materia *Aux = busquedaNombre(lista, fila.nombreCorrelativa);

    // Si existe la correlativa la agregamos, si no, seguira siendo NULL
    if (Aux)
    {
        // cargar Datos en la correlativa
        NodoCorrelativa correlativa;

        correlativa.codMateria = Aux->codigo;

        if (strchr(fila.cursadaAprobada, 'S') || strchr(fila.cursadaAprobada, 's'))
        {
            correlativa.cursadaAprobada = true;
        }
        else
        {
            correlativa.cursadaAprobada = false;
        }

        if (strchr(fila.finalAprobado, 'S') || strchr(fila.finalAprobado, 's'))
        {
            correlativa.finalAprobado = true;
        }
        else
        {
            correlativa.finalAprobado = false;
        }

        return correlativa;
    }
}

Materia *buscarNombre(Materia *lista, char nombre[])
{
    NodoBusqueda *listaAux = NULL;
    // simplificar el nombre
    simplificarString(nombre);

    // Recorrer la lista en busca del nombre
    Materia *Aux = lista;
    while (Aux != NULL)
    {
        if (strstr(Aux->nombre, nombre) != NULL)
        {
            // si encuentra el nombre devolver el puntero
            agregarNodo(listaAux, Aux);
        }
        Aux = Aux->sig;
    }
    if (listaAux == NULL)
    {
        // si no lo encuentra devolver NULL
        borrarNodoLista(listaAux);
        return NULL;
    }
    else if (listaAux->sig == NULL)
    {
        // si no hay mas coincidencias devolver el primer nodo
        Materia *Aux = listaAux->materia;
        borrarNodoLista(listaAux);
        return Aux;
    }

    cout << "Principales coincidencias: " << endl;
    NodoBusqueda *aux = listaAux;
    while (aux)
    {

        cout << aux->materia->codigo << ": " << aux->materia->nombre << endl;

        aux = aux->sig;
    }

    Materia *materia = NULL;

    do
    {
        int codigo = 0;
        cout << "Ingrese el codigo de la materia que desea buscar: ";
        cin >> codigo;

        bool flag = false;
        aux = listaAux;
        while (aux)
        {
            if (codigo == aux->materia->codigo)
                flag = true;
            aux = aux->sig;
        }

        if (flag)
            materia = busquedaCodigo(lista, codigo);

        if (!materia)
        {
            cout << endl
                 << "ERROR 3: Materia no encontrada, Ingrese una materia valida" << endl
                 << endl;
        }
    } while (!materia);

    return materia;
}

Materia *busquedaNombre(Materia *lista, char nombre[])
{
    // simplificar el nombre
    simplificarString(nombre);

    // Recorrer la lista en busca del nombre
    Materia *Aux = lista;
    while (Aux != NULL)
    {
        if (strcmp(Aux->nombre, nombre) == 0)
        {
            // si encuentra el nombre devolver el puntero
            return Aux;
        }
        Aux = Aux->sig;
    }
    // si no lo encuentra devolver NULL
    return NULL;
}

Materia *busquedaCodigo(Materia *lista, int codigo)
{
    Materia *Aux = lista;
    while (Aux != NULL)
    {
        if (codigo == Aux->codigo)
        {
            return Aux;
        }
        Aux = Aux->sig;
    }
    return NULL;
}

void simplificarString(char nombre[])
{

    for (int i = 0; i < strlen(nombre); i++)
    {

        if (nombre[i] == 'á' || nombre[i] == 'Á' || nombre[i] == 'ä' || nombre[i] == 'Ä' ||
            nombre[i] == -96 || nombre[i] == -124 || nombre[i] == -114)
        {
            nombre[i] = 'a';
        }
        else if (nombre[i] == 'é' || nombre[i] == 'É' || nombre[i] == 'ë' || nombre[i] == 'Ë' ||
                 nombre[i] == -126 || nombre[i] == -112 || nombre[i] == -119)
        {
            nombre[i] = 'e';
        }
        else if (nombre[i] == 'í' || nombre[i] == 'Í' || nombre[i] == 'ï' || nombre[i] == 'Ï' ||
                 nombre[i] == -95 || nombre[i] == -117)
        {
            nombre[i] = 'i';
        }
        else if (nombre[i] == 'ó' || nombre[i] == 'Ó' || nombre[i] == 'ö' || nombre[i] == 'Ö' ||
                 nombre[i] == -94 || nombre[i] == -108 || nombre[i] == -103)
        {
            nombre[i] = 'o';
        }
        else if (nombre[i] == 'ú' || nombre[i] == 'Ú' || nombre[i] == 'ü' || nombre[i] == 'Ü' ||
                 nombre[i] == -93 || nombre[i] == -127 || nombre[i] == -102)
        {
            nombre[i] = 'u';
        }
        else if (nombre[i] == 'ñ' || nombre[i] == 'Ñ' ||
                 nombre[i] == -92 || nombre[i] == -91)
        {
            nombre[i] = 'n';
        }
        nombre[i] = tolower(nombre[i]);
    }

    return;
}

void mostrarArbolMateria(Materia *materia, Materia *materias)
{
    cout << endl
         << (char)250 << (char)254 << ' '
         << materia->codigo << " : " << materia->nombre
         << ' ' << (char)254 << (char)250 << endl;

    char prefijo[256];
    for (int i = 0; i < DISTANCIA_RAMAS; i++)
    {
        prefijo[i] = ' ';
    }
    prefijo[DISTANCIA_RAMAS] = '/0';
    mostrarCorrelativas(materia, materias, DISTANCIA_RAMAS, prefijo);
}

void mostrarCorrelativas(Materia *materia, Materia *materias, int i, char prefijo[])
{
    NodoCorrelativa *correlativa = materia->correlativas;

    while (correlativa != NULL)
    {
        Materia *Aux = busquedaCodigo(materias, correlativa->codMateria);

        for (int j = 0; j < i; j++)
        {
            if (prefijo[j] != '/0')
                cout << prefijo[j];
            else
                cout << ' ';
        }
        cout << (char)186 << endl;

        for (int j = 0; j < i; j++)
        {
            if (prefijo[j] != '/0')
                cout << prefijo[j];
            else
                cout << ' ';
        }

        if (correlativa->sig)
        {
            cout << (char)204;
        }
        else
        {
            cout << (char)200;
        }

        cout << correlativa->codMateria << ":" << Aux->nombre << endl;
        for (int j = 0; j < i; j++)
        {
            if (prefijo[j] != '/0')
                cout << prefijo[j];
            else
                cout << ' ';
        }
        if (correlativa->sig)
        {
            cout << (char)186;
        }
        else
        {
            cout << ' ';
        }
        cout << ' ' << (char)195 << "Cursada ";
        (correlativa->cursadaAprobada) ? cout << "Requerida" : cout << "no Requerida";
        cout << endl;

        for (int j = 0; j < i; j++)
        {
            if (prefijo[j] != '/0')
                cout << prefijo[j];
            else
                cout << ' ';
        }
        if (correlativa->sig)
        {
            cout << (char)186;
        }
        else
        {
            cout << ' ';
        }
        cout << ' ' << (char)192 << "Parcial ";
        (correlativa->finalAprobado) ? cout << "Aprobado" : cout << "Pendiendte";
        cout << endl;

        if (Aux->correlativas)
        {
            char sigPrefijo[256];
            strcpy(sigPrefijo, prefijo);

            if (correlativa->sig)
            {
                sigPrefijo[i] = (char)186;
            }
            else
                sigPrefijo[i] = ' ';

            for (int j = 1; j < DISTANCIA_RAMAS; j++)
            {
                sigPrefijo[i + j] = ' ';
            }

            sigPrefijo[i + DISTANCIA_RAMAS] = '/0';

            mostrarCorrelativas(Aux, materias, i + DISTANCIA_RAMAS, sigPrefijo);
        }

        correlativa = correlativa->sig;
    }
    return;
}

int contarCorrelativas(Materia *lista, Materia *materia)
{
    NodoCorrelativa *correlativa = materia->correlativas;
    int i = 0;

    while (correlativa != NULL)
    {
        Materia *Aux = busquedaCodigo(lista, correlativa->codMateria);
        i++;

        if (Aux->correlativas)
        {
            i += contarCorrelativas(lista, Aux);
        }

        correlativa = correlativa->sig;
    }
    return i;
}

void agregarCorrelativa(NodoCorrelativa *&lista, NodoCorrelativa correlativa)
{
    NodoCorrelativa *Nuevo = new NodoCorrelativa();
    *Nuevo = correlativa;
    Nuevo->sig = lista;
    lista = Nuevo;
}

void obtenerCorrelativas(Materia *lista, Materia *materia, Materia *&listaCorrelativas)
{

    NodoCorrelativa *correlativa = materia->correlativas;
    while (correlativa != NULL)
    {

        Materia *Aux = busquedaCodigo(lista, correlativa->codMateria);

        if (Aux)
        {
            if (!busquedaCodigo(listaCorrelativas, Aux->codigo))
            {
                agregarMateria(listaCorrelativas, *Aux);
            }
        }

        if (Aux->correlativas)
        {
            obtenerCorrelativas(lista, Aux, listaCorrelativas);
        }

        correlativa = correlativa->sig;
    }
    return;
}

void agregarMateria(Materia *&lista, Materia materia)
{
    Materia *Nuevo = new Materia();
    *Nuevo = materia;
    Nuevo->sig = lista;
    lista = Nuevo;
}

void borrarIguales(Materia *&listaModificada, Materia *lista)
{
    Materia *Aux = lista;
    while (Aux != NULL)
    {

        borrarMateria(listaModificada, *Aux);
        Aux = Aux->sig;
    }
}

void borrarMateria(Materia *&lista, Materia materia)
{
    if (lista->codigo == materia.codigo)
    {
        Materia *eliminado = lista;
        lista = lista->sig;
        delete eliminado;
        return;
    }

    Materia *Aux = lista;
    Materia *anterior = NULL;
    while (Aux != NULL)
    {
        if (Aux->codigo == materia.codigo)
        {
            Materia *eliminado = Aux;
            anterior->sig = Aux->sig;
            Aux = Aux->sig;
            delete eliminado;
            if (!Aux)
            {
                return;
            }
        }

        anterior = Aux;
        Aux = Aux->sig;
    }
}

void duplicarLista(Materia *&listaDestino, Materia *listaRaiz)
{
    Materia *Aux = listaRaiz;
    while (Aux != NULL)
    {
        agregarMateria(listaDestino, *Aux);
        Aux = Aux->sig;
    }
}

void borrarLista(Materia *&lista)
{
    Materia *Aux = lista;
    while (Aux != NULL)
    {
        Materia *eliminado = Aux;
        Aux = Aux->sig;
        delete eliminado;
    }
    lista = Aux;
}

void borrarNodoLista(NodoBusqueda *&lista)
{
    NodoBusqueda *Aux = lista;
    while (Aux != NULL)
    {
        NodoBusqueda *eliminado = Aux;
        Aux = Aux->sig;
        delete eliminado;
    }
    lista = Aux;
}

void agregarNodo(NodoBusqueda *&lista, Materia *puntero)
{
    NodoBusqueda *Nuevo = new NodoBusqueda();
    Nuevo->materia = puntero;
    Nuevo->sig = lista;
    lista = Nuevo;
}