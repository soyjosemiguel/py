//Incluimos las librerias basicas.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Definimos el tamaño maximo de elementos en el arreglo de personas.
#define amount 200

//Definimos la estructura de los usuarios de la aplicacion.
struct persona{
    int key;
    int age;
    char genero[20];
    float peso;
    float altura;
    float musculo;
    float grasa;
    float tiempo;
    int dormir;
    int activo;
    int trabajo;
    struct persona *siguiente;
};


//Estructura de la actividad fisica.
struct fisica{
    char tipo[30];
    int horas;
    float distancia; //Si aplica.
    float calorias; //Se debe de hacer un estimado dependiendo de la actividad que haga la persona.
    struct fisica *izquierda;
    struct fisica *derecha;
};


//Declaramos el arreglo de estructuras.
struct persona* lista[amount];


//Funcion para iniciar la tabla de hash en null.
void inicio(){
    for (int i = 0; i < amount; i ++){
        lista[i] = NULL;
    }
}


//Funcion para encriptar los elementos en la tabla de hash.
int hash(int key){
    return key % amount;
}


//Funcion para insertar los elementos en la tabla de hash.
void insert(int key, int age, char genero[], float peso, float altura, float musculo, float grasa, float tiempo, int dormir, int activo, int trabajo){
    struct persona *nuevo = malloc(sizeof(struct persona));
    if (nuevo == NULL){
        printf("Error al momento de asignar memoria para una nueva persona\n");
        exit(EXIT_FAILURE);
    }
    nuevo->key = key;
    nuevo->age = age;
    strcpy(nuevo->genero,genero);
    nuevo->peso = peso;
    nuevo->altura = altura;
    nuevo->musculo = musculo;
    nuevo->grasa = grasa;
    nuevo->tiempo = tiempo;
    nuevo->dormir = dormir;
    nuevo->activo = activo;
    nuevo->trabajo = trabajo;
    int index = hash(key);
    if (lista[index] == NULL){
        lista[index] = nuevo;
    }
    else{
        //Manejo la colision.
        struct persona *actual = lista[index];
        while (actual != NULL){
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}


//Funcion para mostrar los elementos de la tabla de hash.
void show (){
    for (int i = 0; i < amount; i ++){
        struct persona *actual = lista[i];
        while (actual != NULL){
            printf("%d \n", actual->age);
            printf("%s \n", actual->genero);
            printf("%f \n", actual->peso);
            printf("%f \n", actual->altura);
            printf("%f \n", actual->musculo);
            printf("%f \n", actual->grasa);
            printf("%f \n", actual->tiempo);
            printf("%d \n", actual->dormir);
            printf("%d \n", actual->activo);
            printf("%d \n", actual->trabajo);
            actual = actual->siguiente;
        }
    }
}

//Funcion para crear un nuevo nodo de un arbol.
struct fisica *creado_arbol(char tipo[], int horas, float distancia, float calorias){
    struct fisica *nuevo = malloc(sizeof(struct fisica));
    if (nuevo == NULL){
        printf("Error al momento de asignar memoria para un nuevo nodo\n");
        exit(EXIT_FAILURE);
    }
    strcpy(nuevo->tipo, tipo);
    nuevo->horas = horas;
    nuevo->distancia = distancia;
    nuevo->calorias = calorias;
    nuevo->derecha = nuevo->izquierda = NULL;
    return nuevo;
}

//Funcion para insertar un elemento en el arbol.
struct fisica *insertar(struct fisica *raiz, char tipo[], int horas, float distancia, float calorias){
    if (raiz == NULL){
        return creado_arbol( tipo, horas, distancia, calorias);
    }
    if (strcmp(tipo, raiz->tipo) < 0){
        raiz->izquierda = insertar(raiz->izquierda, tipo, horas, distancia, calorias);
    }
    else if (strcmp(tipo, raiz->tipo) > 0){
        raiz->derecha = insertar(raiz->derecha, tipo, horas, distancia, calorias);
    }
    return raiz;
}

//Funcion para mostrar los elementos del arbol.
void show_arbol(struct fisica* raiz) {
    if (raiz == NULL) {
        printf("Árbol vacío\n");
        return;
    }

    printf("Tipo: %s\n", raiz->tipo);
    printf("Horas: %d\n", raiz->horas);
    printf("Distancia: %.2f\n", raiz->distancia);
    printf("Calorías: %.2f\n", raiz->calorias);

    if (raiz->izquierda != NULL) {
        printf("Hijo izquierdo:\n");
        show_arbol(raiz->izquierda);
    }

    if (raiz->derecha != NULL) {
        printf("Hijo derecho:\n");
        show_arbol(raiz->derecha);
    }
}


//Funcion para limpiar el buffer.
void limpio() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//Funcion para calcular las calorias quemadas dependiendo de que deporte practica el atleta
float calorias_quemadas(struct fisica **raiz, char tipo[],char genero[], float peso, int edad, float altura, int horas){
    float quemado;
    float tmb;
    float factor;
    //Calculamos el factor de actividad dependiendo de que deporte practica.
    if (strcmp(tipo, "Caminata") == 0 || strcmp(tipo, "Yoga") == 0){
        factor = 1.375;
    }
    else if (strcmp(tipo, "Running") == 0 || strcmp(tipo, "Natacion") == 0 || strcmp(tipo, "Ciclismo") == 0){
        factor = 1.55;
    }
    else if (strcmp(tipo, "Futbol") == 0 || strcmp(tipo, "Artes marciales") == 0 || strcmp(tipo, "Tenis") == 0){
        factor = 1.725;
    }
    else if (strcmp(tipo, "Crossfit") == 0){
        factor = 1.9;
    }
    if (strcmp(genero, "Hombre") == 0){
        tmb = 66 + (6.2 * peso) + (12.7 * altura) - (6.8 * edad);
    }
    else if (strcmp (genero, "Mujer") == 0){
        tmb = 655 + (4.35 * peso) + (4.7 * altura) - (4.7 * edad);
    }
    quemado = tmb * factor * horas;
    return quemado;
}

struct fisica *raiz = NULL;
//Funcion para el registro de la actividad fisica hecha por el atleta.
struct fisica *registro_actividad(char genero[], float peso, int age, float altura){
    char tipo[30];
    int valor;
    int horas;
    float distancia;
    float calorias;
    struct fisica *raiz = NULL; // Inicializa la raíz del árbol a NULL

    // Pedimos el deporte que practica el atleta.
    printf("Ingrese que deporte practica \n");
    printf("1) Running \n");
    printf("2) Caminata \n");
    printf("3) Natacion \n");
    printf("4) Yoga \n");
    printf("5) Crossfit \n");
    printf("6) Pilates \n");
    printf("7) Tenis \n");
    printf("8) Futbol \n");
    printf("9) Artes marciales \n");
    printf("10) Ciclismo \n");
    scanf("%d", &valor);
    if (valor == 1){
        strcpy(tipo, "Running");
    }
    else if (valor == 2){
        strcpy(tipo, "Caminata");
    }
    else if (valor == 3){
        strcpy(tipo, "Natacion");
    }
    else if (valor == 4){
        strcpy(tipo, "Yoga");
    }
    else if (valor == 5){
        strcpy(tipo, "Crossift");
    }
    else if (valor == 6){
        strcpy(tipo, "Pilates");
    }
    else if (valor == 7){
        strcpy(tipo, "Tenis");
    }
    else if (valor == 8){
        strcpy(tipo, "Futbol");
    }
    else if (valor == 9){
        strcpy(tipo, "Artes marciales");
    }
    else if (valor == 10){
        strcpy(tipo, "Ciclismo");
    }
    printf("Cuantos horas practica %s al dia\n", tipo);
    scanf("%d", &horas);
    while (horas <= 0){
        printf("Ingrese una cantidad valida de horas\n");
        scanf("%d", &horas);
    }
    if (strcmp(tipo, "Running") == 0 || strcmp(tipo, "Caminata") == 0 || strcmp(tipo, "Ciclismo") == 0){
        printf("Ingrese los kilometros recorridos--> ");
        scanf("%f", &distancia);
        while (distancia <= 0){
            printf("Ingrese una distancia valida--> ");
            scanf("%f", &distancia);
        }
    }
    else{
        distancia = 0; // Para los deportes que no aplica la distancia.
    }
    
    // Para el calculo de las calorias quemadas dependiendo de que deporte practica el atleta.
    calorias = calorias_quemadas(tipo, genero, peso, age, altura, horas);
    // Insertamos los elementos en el arbol.
    *raiz = insertar(&raiz, tipo, horas, distancia, calorias);
    return raiz;
}

//Comienzo del programa principal.
int main (void){
    inicio();
    insert(12, 18, "Hombre", 65.4 , 1.75, 78.5, 29.4, 12.4, 6, 3, 10);
    show();
    struct fisica *raiz = NULL;
    registro_actividad("Hombre", 65.4, 18, 175);
    show_arbol(raiz);
    return 0;
}
