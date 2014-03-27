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

using namespace std;
unsigned char* readBMP(string);
void bmp_vector(string);

/*
 * 
 */
int main(int argc, char** argv) {
    
    string imageName = "",fullPath ="images/";
    unsigned char* vector; 
    cout << "Introduce el nombre del archivo *.bmp en la ruta ../images/: ";
    cin >> imageName;
    fullPath+= imageName+".bmp";
    vector = readBMP(fullPath);
    cout << strlen((char*)vector);
    return 0;
}

void bmp_vector(string filePath){
    ifstream fin;
    fin.open(filePath.c_str(),ios_base::in|ios_base::binary);
    char header[54];
    if (!fin) // open attempt failed
    {
        cout << "No se pudo leer el archivo";
    }
    fin.read(header,54); 
/*    
    char ch;

    while (fin.get()) // read character from file and
        cout << ch; // write it to screen
    
    cout << "Done\n";
    fin.close();
*/    
//    return 0;    
}

unsigned char* readBMP(string filename)
{
    int i;
    FILE* f = fopen(filename.c_str(), "rb");
    
    if(f == NULL)
    {
        cout << "No se pudo abrir el archivo";
        return 0;
    }
    unsigned char header[54];

    fread(header, sizeof(unsigned char), 54, f); // read the 54-byte header

    // extract image height and width from header
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    cout << endl;
    cout << "  Name: " << filename << endl;
    cout << " Width: " << width << endl;
    cout << "Height: " << height << endl;
    
    int row_padded = (width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    for(int i = 0; i < 32; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, f);
        for(int j = 0; j < 160*3; j += 3)
        {
            // Convert (B, G, R) to (R, G, B)
            tmp = data[j];
            data[j] = data[j+2];
            data[j+2] = tmp;
            if(data[j] > 0 and data[j+1] > 0 and data[j+2] > 0)
                cout<<' ';
            else
                cout<<'*';
            //cout << "R: "<< (int)data[j] << " G: " << (int)data[j+1]<< " B: " << (int)data[j+2]<< endl;
        }
        cout<<endl;
    }

    fclose(f);
    return data;
/*

    int size = 3 * width * height;
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);

    for(i = 0; i < size; i += 3)
    {
            unsigned char tmp = data[i];
            data[i] = data[i+2];
            data[i+2] = tmp;
    }

    return data;
 */
}

