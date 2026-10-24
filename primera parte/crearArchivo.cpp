 #include <iostream>
 #include <limits> //Libreria para poder leer los espacios
using namespace std;

struct Materia{
    int codigo;
    char nombre[50];
    char nombreCorrelativa[50] = "Ninguna";
    char cursadaAprobada[3] = "-";
    char finalAprobado[3] = "-";
};

void completarCorrelativa(Materia&,FILE*);

void completarMaterias(FILE*);

int main(){

    char nombre[50] = "..\\Materias.bin";
    FILE* archivo = fopen(nombre, "wb");

    if(!archivo){
        cout << "ERROR 1: No se pudo abrir el archivo:"<<endl;
        return 1;
    }

    completarMaterias(archivo);

    fclose(archivo);
    

    return 0;
}

void completarMaterias(FILE* archivo){
    int m=1;
    char matRta = 'Y';

    while (matRta == 'Y')
    {
        Materia materia;

        cout<<"--MATERIA "<<m <<"---"<<endl;

        materia.codigo = m;
        
        cout<<"  Nombre de la Materia: ";
        cin >> ws;
        cin.getline(materia.nombre, 50);

        completarCorrelativa(materia,archivo);

        m++;
        cout<<"  Agregar otra materia?"<<endl<<"  Y.Si"<<endl<<"  N.No"<<endl<<"  ";
        cin>>matRta;
    }
}


void completarCorrelativa(Materia& materia,FILE* archivo){
    char rta;
    
    cout<<"  Tiene correlativas?"<<endl<<"  Y.Si"<<endl<<"  N.No"<<endl<<"  ";
    cin>>rta;
    
    if(rta == 'N'){
        fwrite(&materia,sizeof(Materia),1,archivo);
        return;
    }

    int n = 1;
    while(rta == 'Y'){

            cout<<" L CORRELATIVA " << n <<"---"<<endl;

            cout<<"      Nombre de la correlativa: ";
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cin.getline(materia.nombreCorrelativa, 50);

            cout<<"      La Cursada es Requerida?: ";
            cin >> materia.cursadaAprobada;

            cout<<"      El Final es Requerido?: ";
            cin >> materia.finalAprobado;

            fwrite(&materia,sizeof(Materia),1,archivo);

            n++;
            cout<<"      Agregar otra correlativa?"<<endl<<"      Y.Si"<<endl<<"      N.No"<<endl<<"      ";
            cin>>rta;
    }
        
}