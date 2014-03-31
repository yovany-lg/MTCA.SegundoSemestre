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

struct charSelection{
    int row,col;
    char charDescription[5];
    int charWidth;    //{width, height}
    int charHeight;
    char *dotMatrix;
};

using namespace std;
void readBMP(string, char**);
void bmp_vector(string);
void retrieveCharSelection(charSelection,char**);
void printCharSelection(charSelection);
//void digitTrainingVectorInit(charSelection**,int,int);
//void digitVectorInit(charSelection *,int,int);

int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    char** dotMatrix; //Para generar la matriz dinámica
    int width =32, height = 32;
    charSelection chars= {4,2,"0",width,height,new char[width*height]};
    int dotVectorSize = width*height;
    charSelection digitVector1[10]= {
            {4,1,"0",width,height,new char[dotVectorSize]},
            {4,2,"1",width,height,new char[dotVectorSize]},
            {4,3,"2",width,height,new char[dotVectorSize]},
            {4,4,"3",width,height,new char[dotVectorSize]},
            {4,5,"4",width,height,new char[dotVectorSize]},
            {4,6,"5",width,height,new char[dotVectorSize]},
            {4,7,"6",width,height,new char[dotVectorSize]},
            {4,8,"7",width,height,new char[dotVectorSize]},
            {4,9,"8",width,height,new char[dotVectorSize]},
            {4,10,"9",width,height,new char[dotVectorSize]},
        };
    charSelection digitVector2[10]= {
            {4,1,"0",width,height,new char[dotVectorSize]},
            {4,2,"1",width,height,new char[dotVectorSize]},
            {4,3,"2",width,height,new char[dotVectorSize]},
            {4,4,"3",width,height,new char[dotVectorSize]},
            {4,5,"4",width,height,new char[dotVectorSize]},
            {4,6,"5",width,height,new char[dotVectorSize]},
            {4,7,"6",width,height,new char[dotVectorSize]},
            {4,8,"7",width,height,new char[dotVectorSize]},
            {4,9,"8",width,height,new char[dotVectorSize]},
            {4,10,"9",width,height,new char[dotVectorSize]},
        };
    charSelection *digitsVector[2];
    digitsVector = new charSelection*[2] = {digitVector1,digitVector2};
    
    
    //digitTrainingVectorInit(digitsVector,width,height);
    cout << "Introduce el nombre del archivo fuente (*.bmp) en la ruta ../images/: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";    //Ruta completa del archivo
    
    readBMP(fullPath, dotMatrix); //Obtener la imagen en una matriz de puntos
    retrieveCharSelection(digitsVector[1][3],dotMatrix);
    //retrieveCharSelection(chars,dotMatrix);
    //Solo para ver si funciona esta cosa :P
    printCharSelection(digitsVector[1][3]);
    
    
    
    
    return 0;
}


void printCharSelection(charSelection charSelected){
    int endDotVector = charSelected.charHeight*charSelected.charWidth;
    for(int i = 0; i<endDotVector; i++)
    {
        cout << charSelected.dotMatrix[i];
        if((i%charSelected.charWidth) ==0)
            cout << endl;
    }
}


void retrieveCharSelection(charSelection charSelected, char **dotMatrix){
    int matrixRowInit = charSelected.charHeight*(charSelected.row-1); //Equivalente en la fila de la matriz
    int matrixRowEnd = matrixRowInit + charSelected.charHeight;//Fila final de la búsqueda
    int matrixColInit = charSelected.charWidth*(charSelected.col-1); //Equivalente en la fila de la matriz
    int matrixColEnd = matrixColInit + charSelected.charWidth;//Fila final de la búsqueda
    //row.dotMatrix = new char[row.charWidth*row.charHeight];
    int index =0;
    for(int i = matrixRowInit; i < matrixRowEnd ; i++)
    {
        
        for(int j=matrixColInit; j<matrixColEnd;j++)
        {
             charSelected.dotMatrix[index] = dotMatrix[i][j];
             index++;
        }
    }
}


void readBMP(string filename, char** dotMatrix)
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
    *dotMatrix = new char[width];
    for(int x = 0; x<width;x++)
        dotMatrix[x] = new char[height];
    
    cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;
    
    //Tamaño de datos a leer considerando el tamaño de la información del
    //color del pixel
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
                dotMatrix[width-i-1][(int)j/3] = ' ';
            else
                dotMatrix[width-i-1][(int)j/3] = '*';
        }
//        cout<<endl;
    }
    fclose(f);
}

