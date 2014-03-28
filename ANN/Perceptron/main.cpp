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


struct charDotMatrix{
    char** dotMatrix;
};

struct rowCharsSelection{
    int row;
    int charWidth;    //{width, height}
    int charHeight;
    struct charDotMatrix *charVector;
};
using namespace std;
void readBMP(string, char**);
void bmp_vector(string);
void retrieveRowSelection(rowCharsSelection row, char **dotMatrix);

int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    char** dotMatrix; //Para generar la matriz dinámica
    rowCharsSelection rowSelection[2] = {
        {4,32,32,new charDotMatrix[16]},
        {5,32,32,new charDotMatrix[16]}
    };
    
    cout << "Introduce el nombre del archivo fuente (*.bmp) en la ruta ../images/: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";    //Ruta completa del archivo
    
    readBMP(fullPath, dotMatrix); //Obtener la imagen en una matriz de puntos
    retrieveRowSelection(rowSelection[0],dotMatrix);
    //Solo para ver si funciona esta cosa :P
    for(int i = 0; i<32*8; i++)
    {
        for(int j=0;j<32*5;j++)
        {
            cout << dotMatrix[i][j];
        }
        cout << endl;
    }
    return 0;
}

void retrieveRowSelection(rowCharsSelection row, char **dotMatrix){
    int matrixRowInit = row.charHeight*(row.row-1); //Equivalente en la fila de la matriz
    int matrixRowEnd = matrixRowInit + row.charHeight;//Fila final de la búsqueda
    char matrix[32][32];
    for(int i = matrixRowInit; i < matrixRowEnd ; i++)
    {
        for(int x = 0; x<15;x++)
        {
            ofstream fileOutput;
            string filename = "fontFiles/char.txt";
            fileOutput.open(filename.c_str());
            int ref = 32*x;
            string content= "";
            for(int j=ref; j<(ref+32);j++)
                 content += dotMatrix[i][j];
            fileOutput<<content<<endl;
            
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
        cout<<endl;
    }
    fclose(f);
}

