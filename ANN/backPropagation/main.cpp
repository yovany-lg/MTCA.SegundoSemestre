/* 
 * File:   main.cpp
 * Author: yovany
 *
 * Created on May 11, 2014, 8:45 PM
 */

#include <cstdlib>
#include <cstdlib> // rand(), srand() prototypes
#include <ctime> // time() prototype
#include <iostream>
#include <cmath>

struct Layer{
    int neurons;
    float **weightMatrix;   //En la capa de entrada el apuntador esta a nulo
    float **outputVector;   //En la capa de entrada se apunta al vector de entrada
    Layer *nextLayer, *prevLayer;
};

struct Pattern{
    float inputs[5];
    float outputs[5];
};

using namespace std;

void weightMatrixInit(int neurons, int inputs, float ***weightMatrix);
void addLayer(int neurons, int inputs, Layer** layersList);
void initialize(int layers,int neurons[],Layer** layersList);
void addInputLayer(int neurons, Layer** layersList);
void feedforward(Pattern **trainingPattern, Layer** layersList);
void getOutputVector(Layer** currentLayer);
float getLocalField(float **inputVector, float **weightVector, int inputs);
float getFunctionSignal(float localField);
void getInputPattern(Pattern **trainingPattern, Layer** inputLayer);
void trainingPatternsIterations(Layer** layersList, Pattern **trainingPatterns, int nPatterns);
/*
 * 
 */
int main(int argc, char** argv) {
    Layer* layersList = NULL;     // layersList: Entrada <-> Oculta <-> Salida -> NULL
    
    int layers = 3;
    int neurons[3] = {5,6,5}; // Formato Vector de neuronas [nSalida,nOculta1,nEntradas,...]
    Pattern p1 = {{0.01,0.02,0.02,0.02,0.4},{0.1699,0.2762,0.2346,0.0854,0.3524}}, 
            p2 = {{0.01,0.02,0.02,0.02,1.0},{0.2599,0.3591,0.2397,0.1065,0.6190}}, 
            p3 = {{0.01,0.02,0.02,0.04,0.4},{0.3082,0.3783,0.2973,0.1846,0.3825}},
            p4 = {{0.01,0.02,0.04,0.04,1.0},{0.3879,0.4130,0.4132,0.2237,0.6677}},
            p5 = {{0.01,0.02,0.04,0.08,0.4},{0.4644,0.4785,0.4478,0.2544,0.3863}},
            p6 = {{0.02,0.04,0.04,0.08,1.0},{0.5022,0.5035,0.4884,0.2557,0.8191}},
            p7 = {{0.02,0.04,0.02,0.04,0.4},{0.5419,0.5438,0.2311,0.1968,0.4483}},
            p8 = {{0.02,0.04,0.02,0.04,1.0},{0.5489,0.5755,0.282,0.2256,0.9603}},
            p9 = {{0.02,0.04,0.02,0.08,0.4},{0.5829,0.7141,0.3188,0.4167,0.4942}},
            p10 = {{0.02,0.04,0.04,0.08,1.0},{0.6158,0.8271,0.5982,0.5275,1}};
    
    Pattern * trainingPatterns[10] = {&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10};
    initialize(layers,neurons,&layersList);
    trainingPatternsIterations(&layersList,trainingPatterns,10);
    
    return 0;
}

//-- Inicializar Capas de la Red
void initialize(int layers,int neurons[],Layer** layersList){
    // Formato Vector de neuronas [nSalida,nOculta1,nEntradas,...]
    for(int i = 0; i < layers; i++) 
    {
        if (i == (layers-1))    //Para la capa de entrada
        {
            addInputLayer(neurons[i],layersList);
        }else{
            addLayer(neurons[i],neurons[i+1],layersList);
        }
    }
}

void addLayer(int neurons, int inputs, Layer** layersList){
    Layer *newLayer = new Layer;
    float *outputVector = new float[neurons];
    
    newLayer->neurons = neurons;
    weightMatrixInit(neurons, inputs, &(newLayer->weightMatrix));
    newLayer->outputVector = &outputVector;    //Vector de salida de las neuronas
    
    //Orden en la lista de Capas
    newLayer->prevLayer = NULL;
    newLayer->nextLayer = *layersList;
    if(*layersList != NULL)
        (*layersList)->prevLayer = newLayer;    
    *layersList = newLayer;
}

void addInputLayer(int neurons, Layer** layersList){
    Layer *newLayer = new Layer;
    float *outputVector = new float[neurons];
    
    newLayer->neurons = neurons;
    newLayer->weightMatrix = NULL;   //En capa de entrada no se necesita
    newLayer->outputVector = &outputVector;    //El vector de salida tomará la dirección del vector del patrón de entrada
    
    //Orden en la lista de Capas
    newLayer->prevLayer = NULL;
    newLayer->nextLayer = *layersList;
    if(*layersList != NULL)
        (*layersList)->prevLayer = newLayer;
    *layersList = newLayer;
}

//-- Inicializar Matriz de pesos
void weightMatrixInit(int neurons, int inputs, float ***weightMatrix){
    float randStep = (float)2/100; //Pasos discretos
    srand(time(NULL)); // seed random-number generator
    
    *weightMatrix = new float*[neurons]; 
    for(int i = 0 ; i<neurons;i++)
    {
        (*weightMatrix)[i] = new float[inputs+1]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(inputs+1);j++)
        {
            float randValue = (rand() % 100)*randStep - 1;
            (*weightMatrix)[i][j] = randValue;   //Inicializacion de los pesos y el bias        
        }
    }
}

void trainingPatternsIterations(Layer** layersList, Pattern **trainingPatterns, int nPatterns){
    for(int i = 0; i< nPatterns;i++){
        feedforward(&(trainingPatterns[i]),layersList);
    }
}

void feedforward(Pattern **trainingPattern, Layer** layersList){
    Layer *currentLayer = *layersList;  //La primer capa (Capa de Entrada)... layersList: Entrada <-> Oculta <-> Salida -> NULL
    getInputPattern(trainingPattern,&currentLayer);
    
    while(currentLayer->nextLayer != NULL){ //Mientras no halla más capas
        currentLayer = currentLayer->nextLayer;
        getOutputVector(&currentLayer);
    }
}

void getInputPattern(Pattern **trainingPattern, Layer** inputLayer){
    for(int i = 0; i < (*inputLayer)->neurons; i++)
        (*(*inputLayer)->outputVector)[i] = (*trainingPattern)->inputs[i];
}

void getOutputVector(Layer** currentLayer){
    Layer *prevLayer = (*currentLayer)->prevLayer;//Para obtener inf. de la capa anterior
    float localField = 0;
    for(int i = 0; i < (*currentLayer)->neurons; i++){  //Obtener la salida por cada neurona
        //Weighted Input Signal
        localField = getLocalField(prevLayer->outputVector,&((*currentLayer)->weightMatrix[i]),prevLayer->neurons);
        (*(*currentLayer)->outputVector)[i] = getFunctionSignal(localField);
    }
}

float getLocalField(float **inputVector, float **weightVector, int inputs){
    float y_in = 1*(*weightVector)[0]; //Se calcula la aportación del BIAS
    for(int i = 0; i < inputs; i++){ 
        y_in += ((*inputVector)[i]) * ((*weightVector)[i+1]);  //Desplazado para no considerar al BIAS
    }
    return y_in;
}

//-- Aplicando la Función de Activación
float getFunctionSignal(float localField){
    return 1/(1+exp(-localField));
}