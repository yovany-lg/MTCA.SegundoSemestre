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

struct Layer{
    int neurons;
    float **weightMatrix;
    Layer *nextLayer, *prevLayer;
};

struct Pattern{
    float inputs[5];
    float outputs[5];
};

using namespace std;

void weightMatrixInit(int neurons, int inputs, float ***weightMatrix);
void addLayer(int neurons, int inputs, Layer** layersList);
void initialize(int layers,int neurons[],int inputs,Layer** layersList);
/*
 * 
 */
int main(int argc, char** argv) {
    Layer* layersList = NULL;     // layersList -> Oculta <-> Salida -> NULL
    
    int inputs = 5, layers = 2;
    int neurons[2] = {5,6}; // Formato Vector de neuronas [nSalida,nOculta1,...]
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
    initialize(layers,neurons,inputs,&layersList);
    return 0;
}

//-- Inicializar Capas de la Red
void initialize(int layers,int neurons[],int inputs,Layer** layersList){
    int prevInputs;
    // layersList -> Oculta <-> Salida -> NULL
    // Formato Vector de neuronas [nSalida,nOculta1,...]
    for(int i = 0; i < layers; i++) 
    {
        if(i == (layers - 1))   //La primer capa oculta
            prevInputs = inputs;    //Toma las entradas del vector de entrada
        else
            prevInputs = neurons[i+1]; //Las entradas de la capa actual son las neuronas de la capa anterior (oculta)
        addLayer(neurons[i],prevInputs,layersList);
    }
}

void addLayer(int neurons, int inputs, Layer** layersList)
{
    Layer *newLayer = new Layer;
    newLayer->neurons = neurons;
    weightMatrixInit(neurons, inputs, &(newLayer->weightMatrix));
    newLayer->prevLayer = NULL;
    newLayer->nextLayer = *layersList;
    if(*layersList != NULL)
        (*layersList)->prevLayer = newLayer;    
    *layersList = newLayer;
}

//-- Inicializar Matriz de pesos
void weightMatrixInit(int neurons, int inputs, float ***weightMatrix)
{
    float randStep = (float)2/100; //Pasos discretos
    srand(time(0)); // seed random-number generator
    
    *weightMatrix = new float*[neurons]; 
    for(int i = 0 ; i<neurons;i++)
    {
        (*weightMatrix)[i] = new float[inputs+1]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(inputs+1);j++)
        {
            float randValue = (rand() % 100)*randStep - 1;
            cout<<"Peso Aleatorio>"<<randValue;
            (*weightMatrix)[i][j] = randValue;   //Inicializacion de los pesos y el bias        
        }
    }
}

void trainNN(int inputs,Layer** layersList, Pattern **trainingPatterns){
    
}