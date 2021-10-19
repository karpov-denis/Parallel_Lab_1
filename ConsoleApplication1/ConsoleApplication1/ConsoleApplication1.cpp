// Parallel1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _USE_MATH_DEFINES
#define test
#include <iostream>
#include <Math.h>
#include "FreeImage.h"
#pragma comment(lib, "FreeImage.lib")
double Gauss(int x, int y, double sigma)
{
       return(exp(-1 * (pow(x, 2) + pow(y, 2))) / (2 * pow(sigma, 2)));
    //   return exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * 3.14 * sigma * sigma);
    //return exp(-(x * x + y * y) / (2 * sigma * sigma));
}


struct Pixel
{
    int R=0;
    int G=0;
    int B=0;
};

double** generatePattern(double sigma, int& n) {
    n = ceil(6 * sigma);
    n += n % 2 == 0 ? 1 : 0;

    double** result = new double* [n];
    for (int i = 0; i < n; i++)
        result[i] = new double[n];

    double sum = 0;
#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            result[i][j] = Gauss((-n / 2 + j), (n / 2 - i),sigma);
            sum += result[i][j];
        }


    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            result[i][j] /= sum;

    return result;
}
int main()
{
    //FreeImage_Initialise();
    int sigma;
    double sgm;
    std::cout << "Enter Sigma" << std::endl;
    std::cin >> sgm;

    double** core = generatePattern(sgm, sigma);

#ifdef test

    for (int i = 0; i < sigma ; i++)
    {
        for (int j = 0; j < sigma ; j++)
        {
            std::cout << std::fixed << core[i][j] << "    ";
        }
        std::cout << std::endl;

    }
    std::cout << std::fixed << "_________________________________________________" << std::endl;
#endif test  
    const char* input_filename = "E:/input.jpeg";
    FREE_IMAGE_FORMAT fmt = FreeImage_GetFileType(input_filename);
    FIBITMAP* A = FreeImage_Load(fmt, input_filename);
    if (A)
    {
        BYTE* data = FreeImage_GetBits(A);
        int height = FreeImage_GetHeight(A);
        int width = FreeImage_GetWidth(A);

        const char* output_filename = "E:/output.jpeg";

#ifdef test
        std::cout << "Image Readed" << std::endl;
        std::cout << "height : " << height << std::endl;
        std::cout << "width : " << width << std::endl;
#endif test  
        Pixel** cur_data = new Pixel * [height];
        for (int i = 0; i < height; i++)
        {
            cur_data[i] = new Pixel[width];
        }
        int a = 0;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                cur_data[i][j].R = data[a++];
                cur_data[i][j].G = data[a++];
                cur_data[i][j].B = data[a++];
            }

        }
       
        BYTE* new_data = new BYTE[(height* width*3)];
        int tek = 0;
        unsigned int time = clock();

        #pragma omp parallel for
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                double sumR = 0;
                double sumG = 0;
                double sumB = 0;

                for (int q = 0; q < sigma; q++)
                {
                        for (int w = 0; w < sigma; w++)
                        {
                            int stepH = q - sigma / 2;
                            int stepW = w - sigma / 2;
                            if (i + stepH >= 0 && j + stepW >= 0 && i + stepH < height && j + stepW < width)
                            {
                            sumR += cur_data[i + stepH][j + stepW].R * core[q][w];
                            sumG += cur_data[i + stepH][j + stepW].G * core[q][w];
                            sumB += cur_data[i + stepH][j + stepW].B * core[q][w];
                            }
                        else
                            {
                            sumR += 0;
                            sumG += 0;
                            sumB += 0;

                            }
                        }
                }

                new_data[i *3* width + (j+1)*3-3] = round(sumR);
                new_data[i *3* width + (j+1)*3-2] = round(sumG);
                new_data[i *3* width + (j+1)*3-1] = round(sumB);
              
            }
        }
        std::cout << clock() - time << " - Work time" << std::endl;
        sigma--;
        int z = FreeImage_GetBPP(A);
    //    z = 32;
        FIBITMAP* newOne = FreeImage_ConvertFromRawBits(new_data, width, height,width*3, z, 255, 255, 255);
        
        FREE_IMAGE_FORMAT out_fif = FreeImage_GetFIFFromFilename(output_filename);
        if (out_fif != FIF_UNKNOWN)
        {
            // then save the file
            FreeImage_Save(out_fif, newOne, output_filename, 0);
        }

        // free the loaded FIBITMAP
        FreeImage_Unload(A);

        delete[] new_data;

        for (int i = 0; i < height; i++)
        {
            delete cur_data[i];
            
        }
        for (int i = 0; i < sigma; i++)
        {
            delete core[i];
        }
        delete[] core;
        delete[] cur_data;
    }


}