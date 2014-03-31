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
using namespace std;

struct charSelection{
    int row,col;
    char charDescription[5];
    int charWidth;    //{width, height}
    int charHeight;
    char *dotVector;
};

void readBMP(string, char**);
void bmp_vector(string);
void retrieveCharSelection(charSelection,char**);
void printCharSelection(charSelection);
void freeDotMatrixMem(char**);
//void digitTrainingVectorInit(charSelection**,int,int);
//void digitVectorInit(charSelection *,int,int);

int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    char** dotMatrix; //Para generar la matriz dinámica
    int width =32, height = 32;
    int dotVectorSize = width*height;
    charSelection digit1= {4,1,"0",width,height,new char[dotVectorSize]},digit2= {4,2,"0",width,height,new char[dotVectorSize]},
        digit3= {4,3,"0",width,height,new char[dotVectorSize]},digit4= {4,4,"0",width,height,new char[dotVectorSize]},
        digit5= {4,5,"0",width,height,new char[dotVectorSize]},digit6= {4,6,"0",width,height,new char[dotVectorSize]},
        digit7= {4,7,"0",width,height,new char[dotVectorSize]},digit8= {4,8,"0",width,height,new char[dotVectorSize]},
        digit9= {4,9,"0",width,height,new char[dotVectorSize]},digit10= {4,10,"0",width,height,new char[dotVectorSize]};
    charSelection charA= {5,2,"0",width,height,new char[dotVectorSize]},charB= {5,3,"0",width,height,new char[dotVectorSize]},
        charC= {5,4,"0",width,height,new char[dotVectorSize]},charD= {5,5,"0",width,height,new char[dotVectorSize]},
        charE= {5,6,"0",width,height,new char[dotVectorSize]},charF= {5,7,"0",width,height,new char[dotVectorSize]},
        charG= {5,8,"0",width,height,new char[dotVectorSize]},charH= {5,9,"0",width,height,new char[dotVectorSize]},
        charI= {5,10,"0",width,height,new char[dotVectorSize]},charJ= {5,11,"0",width,height,new char[dotVectorSize]},
        charK= {5,12,"0",width,height,new char[dotVectorSize]},charL= {5,13,"0",width,height,new char[dotVectorSize]},
        charM= {5,14,"0",width,height,new char[dotVectorSize]},charN= {5,15,"0",width,height,new char[dotVectorSize]},
        charO= {5,16,"0",width,height,new char[dotVectorSize]},charP= {6,1,"0",width,height,new char[dotVectorSize]},
        charQ= {6,2,"0",width,height,new char[dotVectorSize]},charR= {6,3,"0",width,height,new char[dotVectorSize]},
        charS= {6,4,"0",width,height,new char[dotVectorSize]},charT= {6,5,"0",width,height,new char[dotVectorSize]},
        charU= {6,6,"0",width,height,new char[dotVectorSize]},charV= {6,7,"0",width,height,new char[dotVectorSize]},
        charW= {6,8,"0",width,height,new char[dotVectorSize]},charX= {6,9,"0",width,height,new char[dotVectorSize]},
        charY= {6,10,"0",width,height,new char[dotVectorSize]},charZ= {6,11,"0",width,height,new char[dotVectorSize]};
    charSelection char_a= {7,2,"0",width,height,new char[dotVectorSize]},char_b= {7,3,"0",width,height,new char[dotVectorSize]},
        char_c= {7,4,"0",width,height,new char[dotVectorSize]},char_d= {7,5,"0",width,height,new char[dotVectorSize]},
        char_e= {7,6,"0",width,height,new char[dotVectorSize]},char_f= {7,7,"0",width,height,new char[dotVectorSize]},
        char_g= {7,8,"0",width,height,new char[dotVectorSize]},char_h= {7,9,"0",width,height,new char[dotVectorSize]},
        char_i= {7,10,"0",width,height,new char[dotVectorSize]},char_j= {7,11,"0",width,height,new char[dotVectorSize]},
        char_k= {7,12,"0",width,height,new char[dotVectorSize]},char_l= {7,13,"0",width,height,new char[dotVectorSize]},
        char_m= {7,14,"0",width,height,new char[dotVectorSize]},char_n= {7,15,"0",width,height,new char[dotVectorSize]},
        char_o= {7,16,"0",width,height,new char[dotVectorSize]},char_p= {8,1,"0",width,height,new char[dotVectorSize]},
        char_q= {4,2,"0",width,height,new char[dotVectorSize]},char_r= {8,3,"0",width,height,new char[dotVectorSize]},
        char_s= {4,2,"0",width,height,new char[dotVectorSize]},char_t= {8,5,"0",width,height,new char[dotVectorSize]},
        char_u= {4,2,"0",width,height,new char[dotVectorSize]},char_v= {8,7,"0",width,height,new char[dotVectorSize]},
        char_w= {4,2,"0",width,height,new char[dotVectorSize]},char_x= {8,9,"0",width,height,new char[dotVectorSize]},
        char_y= {4,2,"0",width,height,new char[dotVectorSize]},char_z= {8,11,"0",width,height,new char[dotVectorSize]};
    charSelection * digitVector[10] = {&digit1,&digit2,&digit3,&digit4,&digit5,&digit6,&digit7,&digit8,&digit9,&digit10};
    charSelection * upperCharVector[26] = {&charA,&charB,&charC,&charD,&charE,&charF,&charG,&charH,&charI,&charJ,&
        charK,&charL,&charM,&charN,&charO,&charP,&charQ,&charR,&charS,&charT,&charU,&charV,&charW,&charX,&charY,&charZ};
    charSelection * lowerCharVector[26] = {&char_a,&char_b,&char_c,&char_d,&char_e,&char_f,&char_g,&char_h,&char_i,&char_j,
        &char_k,&char_l,&char_m,&char_n,&char_o,&char_p,&char_q,&char_r,&char_s,&char_t,&char_u,&char_v,&char_w,&char_x,&char_y,&char_z};

//    string trainingImages[3] = {"Cambria","Calibri","TimesNewRoman"};
    
    cout << "Introduce el nombre del archivo \"*.bmp\" en la ruta \"../images/\" para entrenamiento: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";    //Ruta completa del archivo
    readBMP(fullPath, dotMatrix); //Obtener la imagen en una matriz de puntos
    retrieveCharSelection(*(lowerCharVector[15]),dotMatrix);
    printCharSelection(*(lowerCharVector[15]));        
    
    return 0;
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
        cout << charSelected.dotVector[i];
        if((i%charSelected.charWidth) ==0)
            cout << endl;
    }
}


void retrieveCharSelection(charSelection charSelected, char **dotMatrix){
    int matrixRowInit = charSelected.charHeight*(charSelected.row-1); //Equivalente en la fila de la matriz
    int matrixRowEnd = matrixRowInit + charSelected.charHeight;//Fila final de la búsqueda
    int matrixColInit = charSelected.charWidth*(charSelected.col-1); //Equivalente en la fila de la matriz
    int matrixColEnd = matrixColInit + charSelected.charWidth;//Fila final de la búsqueda
    int index =0; //dotVectorSize = charSelected.charWidth*charSelected.charWidth;
    
//    charSelected.dotVector = new char[dotVectorSize];
    for(int i = matrixRowInit; i < matrixRowEnd ; i++)
    {
        
        for(int j=matrixColInit; j<matrixColEnd;j++)
        {
             charSelected.dotVector[index] += dotMatrix[i][j];
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
                dotMatrix[width-i-1][(int)j/3] = ' ';
            else
                dotMatrix[width-i-1][(int)j/3] = '*';
        }
    }
    fclose(f);
}

