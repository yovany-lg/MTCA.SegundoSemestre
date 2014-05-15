/* 
 * File:   main.cpp
 * Author: yovany
 *
 * Created on May 11, 2014, 8:45 PM
 */

#include <cstdlib> // rand(), srand() prototypes
#include <ctime> // time() prototype
#include <iostream>
#include <cmath>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>


struct Layer{
    int neurons;
    float **weightMatrix;   //En la capa de entrada el apuntador esta a nulo
    float *outputVector;   //En la capa de entrada se apunta al vector de entrada
    float *localFields;
    float *localGradients;
    Layer *nextLayer, *prevLayer;
};

struct Pattern2{
    float inputs[5];
    float targets[5];
};

struct Pattern{
    float inputs[2];
    float targets[1];
};

template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

using namespace std;

void weightMatrixInit(int neurons, int inputs, float ***weightMatrix);
void addLayer(int neurons, int inputs, Layer** layersList);
void initialize(int layers,int neurons[],Layer** layersList);
void addInputLayer(int neurons, Layer** layersList);
Layer* feedforward(Pattern **trainingPattern, Layer** layersList);
void getOutputVector(Layer** currentLayer);
float getLocalField(float **inputVector, float **weightVector, int inputs);
float getFunctionSignal(float localField);
float getFunctionSignal2(float localField);
void getInputPattern(Pattern **trainingPattern, Layer** inputLayer);
void trainingPatternsIterations(Layer** layersList, Pattern **trainingPatterns, float learningRate, int nPatterns, int iterations);
float errorBackPropagation(Layer **outputLayer, Pattern **inputPattern, float learningRate);
float getOutputLayerGradients(Layer **currentLayer, Pattern **inputPattern);
float backPropagatedGradient(Layer **currentLayer, int currentNeuron);
void getLocalGradients(Layer **currentLayer);
void printWeightMatrix(int neurons, int inputs, float ***weightMatrix);
void outputLayerWeightUpdates(Layer **outputLayer, float learningRate);
void weightUpdates(Layer **currentLayer, float learningRate);
string weightVectorToString(float **weightVector,int inputs);
string vectorToString(float **vector,int size);
/*
 * 
 */
int main(int argc, char** argv) {
    Layer* layersList = NULL;     // layersList: Entrada <-> Oculta <-> Salida -> NULL
    
    int layers = 3;
    int neurons[3] = {1,4,2}; // Formato Vector de neuronas [nSalida,nOculta1,nEntradas,...]
    Pattern2 p1 = {{0.01,0.02,0.02,0.02,0.4},{0.1699,0.2762,0.2346,0.0854,0.3524}}, 
            p2 = {{0.01,0.02,0.02,0.02,1.0},{0.2599,0.3591,0.2397,0.1065,0.6190}}, 
            p3 = {{0.01,0.02,0.02,0.04,0.4},{0.3082,0.3783,0.2973,0.1846,0.3825}},
            p4 = {{0.01,0.02,0.04,0.04,1.0},{0.3879,0.4130,0.4132,0.2237,0.6677}},
            p5 = {{0.01,0.02,0.04,0.08,0.4},{0.4644,0.4785,0.4478,0.2544,0.3863}},
            p6 = {{0.02,0.04,0.04,0.08,1.0},{0.5022,0.5035,0.4884,0.2557,0.8191}},
            p7 = {{0.02,0.04,0.02,0.04,0.4},{0.5419,0.5438,0.2311,0.1968,0.4483}},
            p8 = {{0.02,0.04,0.02,0.04,1.0},{0.5489,0.5755,0.282,0.2256,0.9603}},
            p9 = {{0.02,0.04,0.02,0.08,0.4},{0.5829,0.7141,0.3188,0.4167,0.4942}},
            p10 = {{0.02,0.04,0.04,0.08,1.0},{0.6158,0.8271,0.5982,0.5275,1}};
    
    Pattern2 * trainingPatterns[10] = {&p1,&p2,&p3,&p4,&p5,&p6,&p7,&p8,&p9,&p10};
    
    Pattern xor1 = {{0,0},{-1}}, 
            xor2 = {{0,1},{1}}, 
            xor3 = {{1,0},{1}},
            xor4 = {{1,1},{-1}};
    
    Pattern * trainingPatterns2[10] = {&xor1,&xor2,&xor3,&xor4};
    
    initialize(layers,neurons,&layersList);
    trainingPatternsIterations(&layersList,trainingPatterns2,0.1,4,2000);
    
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
    
    newLayer->neurons = neurons;
    weightMatrixInit(neurons, inputs, &(newLayer->weightMatrix));
    newLayer->outputVector = new float[neurons];    //Vector de salida de las neuronas
    newLayer->localFields = new float[neurons];    //Vector de localFields: Entradas antes de aplicar la función de activación
    newLayer->localGradients = new float[neurons];    //Vector de gradientes locales
    
    //Orden en la lista de Capas
    newLayer->prevLayer = NULL;
    newLayer->nextLayer = *layersList;
    if(*layersList != NULL)
        (*layersList)->prevLayer = newLayer;    
    *layersList = newLayer;
}

void addInputLayer(int neurons, Layer** layersList){
    Layer *newLayer = new Layer;
    
    newLayer->neurons = neurons;
    newLayer->weightMatrix = NULL;   //En capa de entrada no se necesita
    //Fue mejor crear un vector independiente que se actualizará
    newLayer->outputVector = new float[neurons];    //El vector de salida tomará la dirección del vector del patrón de entrada
    
    //Orden en la lista de Capas
    newLayer->prevLayer = NULL;
    newLayer->nextLayer = *layersList;
    if(*layersList != NULL)
        (*layersList)->prevLayer = newLayer;
    *layersList = newLayer;
}

//-- Inicializar Matriz de pesos
void weightMatrixInit(int neurons, int inputs, float ***weightMatrix){
    float randStep = (float)2/1000; //Pasos discretos
    srand(time(NULL)); // seed random-number generator
    
    *weightMatrix = new float*[neurons]; 
    for(int i = 0 ; i<neurons;i++)
    {
        (*weightMatrix)[i] = new float[inputs+1]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(inputs+1);j++)
        {
            float randValue = (rand() % 1000)*randStep - 1;
            (*weightMatrix)[i][j] = randValue;   //Inicializacion de los pesos y el bias        
        }
    }
    printWeightMatrix(neurons,inputs,weightMatrix);
}

void trainingPatternsIterations(Layer** layersList, Pattern **trainingPatterns, float learningRate, int nPatterns, int iterations){
    Layer *outputLayer = NULL;
    float globalErrorEnergy = 0;
    for(int k = 0; k < iterations; k++)//Iteraciones
    {
        for(int i = 0; i< nPatterns;i++){
            outputLayer = feedforward(&(trainingPatterns[i]),layersList);   //Retorna la dirección de la capa de salida.
            globalErrorEnergy += errorBackPropagation(&outputLayer,&(trainingPatterns[i]),learningRate);
        }
        //globalErrorEnergy = globalErrorEnergy/((float)nPatterns);
        cout<<"-- Error Cuadratico Global en la Iteracion "+to_string(k+1)+": "<<globalErrorEnergy<<"\n\n";
        globalErrorEnergy = 0;
    }
}

Layer* feedforward(Pattern **trainingPattern, Layer** layersList){
    Layer *currentLayer = *layersList;  //La primer capa (Capa de Entrada)... layersList: Entrada <-> Oculta <-> Salida -> NULL
    getInputPattern(trainingPattern,&currentLayer);

    while(currentLayer->nextLayer != NULL){ //Mientras no halla más capas
        currentLayer = currentLayer->nextLayer;
        getOutputVector(&currentLayer);
    }
    return currentLayer;
}

void getInputPattern(Pattern **trainingPattern, Layer** inputLayer){
    for(int i = 0; i < (*inputLayer)->neurons; i++)
        (*inputLayer)->outputVector[i] = (*trainingPattern)->inputs[i];
//    cout<<vectorToString(&((*inputLayer)->outputVector), (*inputLayer)->neurons);
}

void getOutputVector(Layer** currentLayer){
    Layer *prevLayer = (*currentLayer)->prevLayer;//Para obtener inf. de la capa anterior
    float localField = 0;
    for(int i = 0; i < (*currentLayer)->neurons; i++){  //Obtener la salida por cada neurona
        //Weighted Input Signal
        localField = getLocalField(&(prevLayer->outputVector),&((*currentLayer)->weightMatrix[i]),prevLayer->neurons);
        (*currentLayer)->outputVector[i] = getFunctionSignal2(localField);
        (*currentLayer)->localFields[i] = localField;
    }
//    cout<<vectorToString(&((*currentLayer)->localFields), (*currentLayer)->neurons)<<"\n";    
}

float getLocalField(float **inputVector, float **weightVector, int inputs){
    float y_in = 1*(*weightVector)[0]; //Se calcula la aportación del BIAS
    for(int i = 0; i < inputs; i++){ 
        y_in += ((*inputVector)[i]) * ((*weightVector)[i+1]);  //Desplazado para no considerar al BIAS
    }
    return y_in;
}

//-- Aplicando la Función de Activación Sigmoide
float getFunctionSignal(float localField){
    return 1/(1+exp(-localField));
}

//-- Aplicando la Función de Activación Sigmoide Bipolar
float getFunctionSignal2(float localField){
    return 2.0*getFunctionSignal(localField)-1;
}

//-- Aplicando la Derivada de la Función de Activación
float getPrimeFunctionSignal(float localField){
    float functionSignal = getFunctionSignal(localField);
    return functionSignal/(1-functionSignal);
}

//-- Aplicando la Derivada de la Función de Activación Sigmoide Bipolar
float getPrimeFunctionSignal2(float localField){
    float functionSignal = getFunctionSignal2(localField);
    return (1.0/2)*(1+functionSignal)*(1-functionSignal);
}

float errorBackPropagation(Layer **outputLayer, Pattern **inputPattern, float learningRate){
    Layer *currentLayer = *outputLayer;
    float totalErrorEnergy = getOutputLayerGradients(&currentLayer, inputPattern);   //Gradientes de la capa de salida
    outputLayerWeightUpdates(&currentLayer,learningRate);
    //printWeightMatrix();
    
    while(currentLayer->prevLayer->prevLayer != NULL){ //Miestras la capa anterior no sea la capa de entrada
        currentLayer = currentLayer->prevLayer; //Moverser una capa atrás. Ya se procesó la capa de salida
        getLocalGradients(&currentLayer);
        weightUpdates(&currentLayer,learningRate);
    }
    return totalErrorEnergy;
}

//-- Local Gradients from Output Layer
float getOutputLayerGradients(Layer **currentLayer, Pattern **inputPattern){
    float errorSignal = 0,errorEnergy = 0, totalErrorEnergy = 0;
    for(int i = 0; i < (*currentLayer)->neurons; i++)
    {
        errorSignal = (((*inputPattern)->targets[i]) - (*currentLayer)->outputVector[i]);
        (*currentLayer)->localGradients[i] = errorSignal*getPrimeFunctionSignal2((*currentLayer)->localFields[i]);
        errorEnergy = (1.0/2.0)*errorSignal*errorSignal;
        totalErrorEnergy += errorEnergy;
    }
//    cout<<vectorToString(&((*currentLayer)->localGradients), (*currentLayer)->neurons)<<"\n";    
    return totalErrorEnergy;
}

void getLocalGradients(Layer **currentLayer){
    for(int i = 0; i < (*currentLayer)->neurons; i++){
        (*currentLayer)->localGradients[i] = backPropagatedGradient(currentLayer,i) * getPrimeFunctionSignal2((*currentLayer)->localFields[i]);
    }
//    cout<<vectorToString(&((*currentLayer)->localGradients), (*currentLayer)->neurons)<<"\n";    
}

float backPropagatedGradient(Layer **currentLayer, int currentNeuron){
    Layer *nextLayer = (*currentLayer)->nextLayer;
    float weightedGradientSum = 0;
    for(int i = 0; i < nextLayer->neurons; i++){
        weightedGradientSum += (nextLayer->localGradients[i]) * (nextLayer->weightMatrix[i][currentNeuron+1]);   //currentNeuron+1 => Considerando la posición del BIAS
    }
    return weightedGradientSum;
}

void outputLayerWeightUpdates(Layer **outputLayer, float learningRate){
    Layer *prevLayer = (*outputLayer)->prevLayer;
    for(int i = 0; i < (*outputLayer)->neurons; i++){
        (*outputLayer)->weightMatrix[i][0] = (*outputLayer)->weightMatrix[i][0] + learningRate*((*outputLayer)->localGradients[i]);   //Cambio de pesos en el BIAS
        for(int j = 0; j < prevLayer->neurons; j++) //Entradas de la capa Anterior, el BIAS ya se procesó
            (*outputLayer)->weightMatrix[i][j+1] = (*outputLayer)->weightMatrix[i][j+1] + learningRate*((*outputLayer)->localGradients[i])*(prevLayer->outputVector[j]);
    }
//    printWeightMatrix((*outputLayer)->neurons,prevLayer->neurons,&((*outputLayer)->weightMatrix));
}

void weightUpdates(Layer **currentLayer, float learningRate){
    Layer *prevLayer = (*currentLayer)->prevLayer;
    for(int i = 0; i < (*currentLayer)->neurons; i++){
        (*currentLayer)->weightMatrix[i][0] = (*currentLayer)->weightMatrix[i][0] + learningRate*((*currentLayer)->localGradients[i]);   //Cambio de pesos en el BIAS
        for(int j = 0; j < prevLayer->neurons; j++) //Entradas de la capa Anterior, el BIAS ya se procesó
            (*currentLayer)->weightMatrix[i][j+1] = (*currentLayer)->weightMatrix[i][j+1] + learningRate*((*currentLayer)->localGradients[i])*(prevLayer->outputVector[j]);
    }
//    printWeightMatrix((*currentLayer)->neurons,prevLayer->neurons,&((*currentLayer)->weightMatrix));
}

void printWeightMatrix(int neurons, int inputs, float ***weightMatrix){
    string neuronWeightVector = "", matriz = "";
    cout<<"Matriz de pesos (La posicion 0 es para el BIAS): \n";
    
    for(int i = 0; i < neurons; i++){
        neuronWeightVector += "N"+to_string(i+1)+": ";
        
        for(int j = 0; j < (inputs+1); j++) //Primera posición para el BIAS
            neuronWeightVector += to_string((*weightMatrix)[i][j])+'\t';
            
        matriz += neuronWeightVector+'\n';
        neuronWeightVector = "";
    }
    cout<<matriz;
}

string weightVectorToString(float **weightVector,int inputs){
    string output="";
    for(int i =0;i<(inputs+1);i++)
        if(i == 0)
            output+="B: "+to_string((*weightVector)[i])+',';
        else
            output+=" W("+to_string(i)+"): "+to_string((*weightVector)[i]);
    return output;
}

string vectorToString(float **vector,int size){
    string output="( ";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+" , ";
    return output+")";
}