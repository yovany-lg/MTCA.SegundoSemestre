/* 
 * File:   main.cpp
 * Author: Yovany
 *
 * Created on 27 de marzo de 2014, 08:39 AM
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>

//Un patch para soportar la función to_string
template <typename T> std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

struct charSelection{
    int row,col;
    char charDescription[5];
    int charWidth;    //{width, height}
    int charHeight;
    int *dotVector;
};

using namespace std;

void readBMP(string filename, char*** dotMatrix);
void bmp_vector(string);
void retrieveCharSelection(charSelection *charSelected, char **dotMatrix);
void printCharSelection(charSelection);
void freeDotMatrixMem(char**);

void perceptronInit(int **inputVector,int ***weightMatrix,int **desired,int **currentOutput,int inputs,int neurons);
void trainNeuralNetwork(int **inputVector,int **weightMatrix,int **desired,int **currentOutput,int rate,int inputs,int neurons,int threshold);
int weightUpdates(int response,int desired,int **inputVector,int **weightVector,int rate,int inputs);
int outputByUnit(int **inputVector,int **weightVector,int inputs);
int activationFunction(int ,int );
void getPatternVector(charSelection *pattern,int **inputVector,int inputs);
void getDesiredOutput(int **desired,int neurons,int current);
string weightVectorToString(int **weightVector,int inputs);
string vectorToString(int **vector,int size);
//void digitTrainingVectorInit(charSelection**,int,int);
//void digitVectorInit(charSelection *,int,int);

int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    char** dotMatrix; //Para generar la matriz dinámica
    int width =32, height = 32;
    int dotVectorSize = width*height;
    charSelection digit1= {4,1,"0",width,height,new int[dotVectorSize]},digit2= {4,2,"0",width,height,new int[dotVectorSize]},
        digit3= {4,3,"0",width,height,new int[dotVectorSize]},digit4= {4,4,"0",width,height,new int[dotVectorSize]},
        digit5= {4,5,"0",width,height,new int[dotVectorSize]},digit6= {4,6,"0",width,height,new int[dotVectorSize]},
        digit7= {4,7,"0",width,height,new int[dotVectorSize]},digit8= {4,8,"0",width,height,new int[dotVectorSize]},
        digit9= {4,9,"0",width,height,new int[dotVectorSize]},digit10= {4,10,"0",width,height,new int[dotVectorSize]};
/*    charSelection charA= {5,2,"0",width,height,new int[dotVectorSize]},charB= {5,3,"0",width,height,new int[dotVectorSize]},
        charC= {5,4,"0",width,height,new int[dotVectorSize]},charD= {5,5,"0",width,height,new int[dotVectorSize]},
        charE= {5,6,"0",width,height,new int[dotVectorSize]},charF= {5,7,"0",width,height,new int[dotVectorSize]},
        charG= {5,8,"0",width,height,new int[dotVectorSize]},charH= {5,9,"0",width,height,new int[dotVectorSize]},
        charI= {5,10,"0",width,height,new int[dotVectorSize]},charJ= {5,11,"0",width,height,new int[dotVectorSize]},
        charK= {5,12,"0",width,height,new int[dotVectorSize]},charL= {5,13,"0",width,height,new int[dotVectorSize]},
        charM= {5,14,"0",width,height,new int[dotVectorSize]},charN= {5,15,"0",width,height,new int[dotVectorSize]},
        charO= {5,16,"0",width,height,new int[dotVectorSize]},charP= {6,1,"0",width,height,new int[dotVectorSize]},
        charQ= {6,2,"0",width,height,new int[dotVectorSize]},charR= {6,3,"0",width,height,new int[dotVectorSize]},
        charS= {6,4,"0",width,height,new int[dotVectorSize]},charT= {6,5,"0",width,height,new int[dotVectorSize]},
        charU= {6,6,"0",width,height,new int[dotVectorSize]},charV= {6,7,"0",width,height,new int[dotVectorSize]},
        charW= {6,8,"0",width,height,new int[dotVectorSize]},charX= {6,9,"0",width,height,new int[dotVectorSize]},
        charY= {6,10,"0",width,height,new int[dotVectorSize]},charZ= {6,11,"0",width,height,new int[dotVectorSize]};
    charSelection char_a= {7,2,"0",width,height,new int[dotVectorSize]},char_b= {7,3,"0",width,height,new int[dotVectorSize]},
        char_c= {7,4,"0",width,height,new int[dotVectorSize]},char_d= {7,5,"0",width,height,new int[dotVectorSize]},
        char_e= {7,6,"0",width,height,new int[dotVectorSize]},char_f= {7,7,"0",width,height,new int[dotVectorSize]},
        char_g= {7,8,"0",width,height,new int[dotVectorSize]},char_h= {7,9,"0",width,height,new int[dotVectorSize]},
        char_i= {7,10,"0",width,height,new int[dotVectorSize]},char_j= {7,11,"0",width,height,new int[dotVectorSize]},
        char_k= {7,12,"0",width,height,new int[dotVectorSize]},char_l= {7,13,"0",width,height,new int[dotVectorSize]},
        char_m= {7,14,"0",width,height,new int[dotVectorSize]},char_n= {7,15,"0",width,height,new int[dotVectorSize]},
        char_o= {7,16,"0",width,height,new int[dotVectorSize]},char_p= {8,1,"0",width,height,new int[dotVectorSize]},
        char_q= {4,2,"0",width,height,new int[dotVectorSize]},char_r= {8,3,"0",width,height,new int[dotVectorSize]},
        char_s= {4,2,"0",width,height,new int[dotVectorSize]},char_t= {8,5,"0",width,height,new int[dotVectorSize]},
        char_u= {4,2,"0",width,height,new int[dotVectorSize]},char_v= {8,7,"0",width,height,new int[dotVectorSize]},
        char_w= {4,2,"0",width,height,new int[dotVectorSize]},char_x= {8,9,"0",width,height,new int[dotVectorSize]},
        char_y= {4,2,"0",width,height,new int[dotVectorSize]},char_z= {8,11,"0",width,height,new int[dotVectorSize]};*/
    charSelection * digitVector[10] = {&digit1,&digit2,&digit3,&digit4,&digit5,&digit6,&digit7,&digit8,&digit9,&digit10};
    /*charSelection * upperCharVector[26] = {&charA,&charB,&charC,&charD,&charE,&charF,&charG,&charH,&charI,&charJ,&
        charK,&charL,&charM,&charN,&charO,&charP,&charQ,&charR,&charS,&charT,&charU,&charV,&charW,&charX,&charY,&charZ};
    charSelection * lowerCharVector[26] = {&char_a,&char_b,&char_c,&char_d,&char_e,&char_f,&char_g,&char_h,&char_i,&char_j,
        &char_k,&char_l,&char_m,&char_n,&char_o,&char_p,&char_q,&char_r,&char_s,&char_t,&char_u,&char_v,&char_w,&char_x,&char_y,&char_z};*/

    cout << "Introduce el nombre del archivo \"*.bmp\" en la ruta \"../images/\" para entrenamiento: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";    //Ruta completa del archivo

    readBMP(fullPath, &dotMatrix); //Obtener la imagen en una matriz de puntos

    int *inputVector,**weightMatrix,*desired,*currentOutput,inputs=32*32,neurons=3,learningRate = 0.1,threshold =0;

    perceptronInit(&inputVector,&weightMatrix,&desired,&currentOutput,inputs,neurons);

    cout<<"\n---Entrenando Red Neuronal---\n";
    for(int x =0;x<100;x++)
    {
        for(int i = 0;i < neurons;i++){
            retrieveCharSelection(digitVector[i],dotMatrix);
            cout<<"\n--Patron de Entrenamiento\n";
            printCharSelection(*(digitVector[i]));
            getPatternVector(digitVector[i],&inputVector,inputs);
            getDesiredOutput(&desired,neurons,i);
            cout<<"\n- Salida Esperada: "<<vectorToString(&desired,neurons);
            trainNeuralNetwork(&inputVector,weightMatrix,&desired,&currentOutput,learningRate,inputs,neurons,threshold);
            cout<<"\n- Salida Obtenida: "<<vectorToString(&currentOutput,neurons);
        }
//        getchar();
    }
    
    return 0;
}

void perceptronInit(int **inputVector,int ***weightMatrix,int **desired,int **currentOutput,int inputs,int neurons){
    *inputVector = new int[inputs+1];
    (*inputVector)[0] = 1; //Entrada por defecto en la primera posicion
    *desired = new int[neurons];//Respuesta deseada en base a las clases
    *currentOutput = new int[neurons];//Respuesta deseada en base a las clases
    *weightMatrix = new int*[neurons]; 
    for(int i = 0 ; i<neurons;i++)
    {
        (*desired)[i] = (*currentOutput)[i] = 0;
        (*weightMatrix)[i] = new int[inputs+1]; //Considerando el bias en la primera posicion
        for(int j = 0; j<(inputs+1);j++)
            (*weightMatrix)[i][j]=0;   //Inicializacion de los pesos y el bias
    }
}

void getPatternVector(charSelection *pattern,int **inputVector,int inputs){
    for(int index = 0;index<inputs;index++)  
        (*inputVector)[index+1] = (*pattern).dotVector[index];//Se omite la entrada por default (bias)
}

void getDesiredOutput(int **desired,int neurons,int current){
    for(int y = 0; y < neurons; y++)
    {
        if(y == current)
            (*desired)[y] = 1;
        else
            (*desired)[y] = -1;
    }
}

void trainNeuralNetwork(int **inputVector,int **weightMatrix,int **desired,int **currentOutput,int rate,int inputs,int neurons,int threshold){
    int y_outByUnit;
    for(int i = 0; i<neurons;i++){
        y_outByUnit = outputByUnit(inputVector,&(weightMatrix[i]),inputs);
        (*currentOutput)[i]= activationFunction(y_outByUnit,threshold);
        if(weightUpdates((*currentOutput)[i],(*desired)[i],inputVector,&(weightMatrix[i]),rate,inputs) == 1)
            cout<<"\n-Ajustando Pesos de la Neurona: "<<i<<"\n"+weightVectorToString(&(weightMatrix[i]),inputs)+"\n";
        else
            cout<<"\n-La Neurona: "<<i<<" No necesita ajustarse\n";
    }
}

string vectorToString(int **vector,int size){
    string output="( ";
    for(int i =0;i<size;i++)
        output+=to_string((*vector)[i])+" ";
    return output+")";
}

string weightVectorToString(int **weightVector,int inputs){
    string output="";
    for(int i =0;i<(inputs+1);i++)
        if(i == 0)
            output+="Bias: "+to_string((*weightVector)[i]);
        else
            output+=" Weight("+to_string(i)+"): "+to_string((*weightVector)[i]);
    return output;
}

int weightUpdates(int response,int desired,int **inputVector,int **weightVector,int rate,int inputs){
    if(response != desired)
    {
        for(int i =0;i < (inputs+1); i++)//(inputs+1), para considerar al bias
            (*weightVector)[i] = (*weightVector)[i] + rate*desired*((*inputVector)[i]);
            return 1;
    }
    return 0;
}

int outputByUnit(int **inputVector,int **weightVector,int inputs){
    int y_out = 0;
    for(int i = 0; i<(inputs+1);i++){ //(inputs+1), para considerar al bias
        y_out += ((*inputVector)[i]) * ((*weightVector)[i]);
    }
    return y_out;
}

int activationFunction(int y_outByUnit,int threshold){
    if(y_outByUnit > threshold)
        return 1;
    if(((-1)*threshold) <=y_outByUnit <= threshold)
        return 0;
    if(y_outByUnit < ((-1)*threshold))
        return -1;
}



void freeDotMatrixMem(char** dotMatrix){
    for ( int i = 0; i < 512; i++ ) //Falta hacerlo dinámico
    { 
        delete [] dotMatrix[ i ];
    } 
    delete [] dotMatrix;
}

void printCharSelection(charSelection charSelected){
    int endDotVector = charSelected.charHeight*charSelected.charWidth;
    for(int i = 0; i<endDotVector; i++)
    {
        if(charSelected.dotVector[i] == -1 )
            cout << ' ';
        else
            cout << '*';

        if((i%charSelected.charWidth) ==0)
            cout << endl;
    }
}


void retrieveCharSelection(charSelection *charSelected, char **dotMatrix){
    int matrixRowInit = (*charSelected).charHeight*((*charSelected).row-1); //Equivalente en la fila de la matriz
    int matrixRowEnd = matrixRowInit + (*charSelected).charHeight;//Fila final de la búsqueda
    int matrixColInit = (*charSelected).charWidth*((*charSelected).col-1); //Equivalente en la fila de la matriz
    int matrixColEnd = matrixColInit + (*charSelected).charWidth;//Fila final de la búsqueda
    int index =0; //dotVectorSize = charSelected.charWidth*charSelected.charWidth;
    
//    charSelected.dotVector = new char[dotVectorSize];
    for(int x = matrixRowInit; x < matrixRowEnd ; x++)
    {
        for(int y=matrixColInit; y<matrixColEnd;y++)
        {
            if(dotMatrix[x][y] == ' ')
                (*charSelected).dotVector[index] = -1;
            else
                (*charSelected).dotVector[index] = +1;
            index++;
        }
    }
}


void readBMP(string filename, char*** dotMatrix)
{
    FILE* f = fopen(filename.c_str(), "rb");
    unsigned char header[54];
    
    if(f == NULL)
    {
        cout << "No se pudo abrir el archivo";
        return;
    }

    //Extraer el header con la información del archivo
    fread(header, sizeof(unsigned char), 54, f);
    
    //Obtener largo y ancho de la imagen
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    //Generar la matríz dinámica
    *dotMatrix = new char*[width];
    for(int x = 0; x<width;x++)
        (*dotMatrix)[x] = new char[height];
    
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;
    
    //Tamaño de datos a leer considerando el tamaño de la información del color del pixel
    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;
    
    //Recorrer la matriz de pixeles
    for(int i = 0; i < width; i++)
    {           
        //Leer un pixel
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < height*3; j += 3)
        {
            // Convertir de (B, G, R) a (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            //Validar si es un pixel vacío o relleno
            if(data[j] > 0 and data[j+1] > 0 and data[j+2] > 0)
                (*dotMatrix)[width-i-1][(int)j/3] = ' ';
            else
                (*dotMatrix)[width-i-1][(int)j/3] = '*';
        }
    }
    fclose(f);
}

