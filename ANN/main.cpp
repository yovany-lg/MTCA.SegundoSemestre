#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>

struct charDotMatrix{
    char** dotMatrix;
};

struct charSelection{
    int row,col;
    char charDescription[5];
    int charWidth;    //{width, height}
    int charHeight;
    char** dotMatrix;
};
using namespace std;
void readBMP(string, char**);
void bmp_vector(string);
void retrieveCharSelection(charSelection,char**);

int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    char** dotMatrix; //Para generar la matriz dinámica
    int width =32, height = 32;
    charSelection chars= {4,1,'0',width,height};
    //        {4,1,'0',width,height,&(new char[width])},
    
    cout << "Introduce el nombre del archivo fuente (*.bmp) en la ruta ../images/: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";    //Ruta completa del archivo
    
    readBMP(fullPath, dotMatrix); //Obtener la imagen en una matriz de puntos
    retrieveCharSelection(chars,dotMatrix);
    //Solo para ver si funciona esta cosa :P
    for(int i = 0; i<width; i++)
    {
        for(int j=0;j<height;j++)
        {
            cout << chars.dotMatrix[i][j];
        }
        cout << endl;
    }
    return 0;
}

void retrieveCharSelection(charSelection row, char **dotMatrix){
    int matrixRowInit = row.charHeight*(row.row-1); //Equivalente en la fila de la matriz
    int matrixRowEnd = matrixRowInit + row.charHeight;//Fila final de la búsqueda
    int matrixColInit = row.charWidth*(row.col-1); //Equivalente en la fila de la matriz
    int matrixColEnd = matrixColInit + row.charWidth;//Fila final de la búsqueda
    *row.dotMatrix = new char[row.charWidth];
    for(int i = matrixRowInit; i < matrixRowEnd ; i++)
    {
        
        //Completar la matriz
        row.dotMatrix[matrixRowInit-i] = new char[row.charHeight];        
        for(int j=matrixColInit; j<matrixColEnd;j++)
             row.dotMatrix[matrixRowInit-i][matrixColInit-j] += dotMatrix[i][j];
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

