#include <iostream>
#include <fstream>
#include "videodevice.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

const int FILE_READ_ERROR = 1;
const int PIC_WIDTH = 640;
const int PIC_HEIGHT = 480;
const int HIST_SIZE = 16;
//pic.yuv - output file

using namespace std;

//----------------------GLOBAL VARS--------------------------
unsigned char** g_intensities = new unsigned char* [PIC_HEIGHT];
/*
    need to be inited, doing it in takeGreyScaleFromYUYV
*/

ifstream g_pic;
/*
    using this ifstream to get info from pic.yuv
    and put that info in g_intensities
*/

unsigned int g_histogram[HIST_SIZE];

//----------------------END OF GLOBAL VARS-------------------

void printG_Intensities()
{
    for (int i = 0; i < PIC_HEIGHT; i ++)
    {
        for (int j = 0; j < PIC_WIDTH; j ++)
        {
            cout << hex << (unsigned int)g_intensities[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << dec << "width " << PIC_WIDTH << " : height " << PIC_HEIGHT << endl;
}

void printG_Histogram()
{
    for (int i = 0; i < HIST_SIZE; i ++)
    {
        cout << g_histogram[i] << " ";
    }
}

void takeGreyScaleFromYUYV()
{
    g_pic.open("pic.yuv", ios::binary | ios::in | ios::out);
    if ( (g_pic.rdstate() & std::ifstream::failbit ) != 0 )
    {
        std::cerr << "Error opening file"  << '\n';
        exit(FILE_READ_ERROR);
    }

    //g_pic.unsetf(std::ios::skipws);

    unsigned char tmp;

    for (int i = 0; i < PIC_HEIGHT; i ++)
    {
        g_intensities[i] = new unsigned char [PIC_WIDTH];

        for (int j = 0; j < PIC_WIDTH; j ++)
        {
            g_intensities[i][j] = g_pic.get();
            tmp = g_pic.get(); // skip color byte;
        }
    }

    g_pic.close();
}

void getG_Histogram()
{
    for (int i = 0; i < HIST_SIZE; i ++)
    {
        g_histogram[i] = '\0';
    }

    int index;

    for (int i = 0; i < PIC_HEIGHT; i ++)
    {
        for (int j = 0; j < PIC_WIDTH; j ++)
        {
            index = g_intensities[i][j] / HIST_SIZE;
            g_histogram[index] ++;
        }
    }

    // checking for correctness of number pixels in histogram
    int sum = 0;
    for (int i = 0; i < HIST_SIZE; i ++)
    {
        sum += g_histogram[i];
    }
    assert(sum == PIC_HEIGHT * PIC_WIDTH);
}



int main(int argc,char* argv[]) {
    string dev_name = "/dev/video0";

    string file_name = "pic.yuv";

    if (argc > 1) {
        dev_name = argv[1];

        if (argc > 2)
            file_name = argv[2];
    }

    videodevice *vd = new videodevice();

    vd -> openDevice(dev_name);

    vd -> setFormatYUYV();

    vd -> getFrame(file_name);

    vd -> closeDevice();

    takeGreyScaleFromYUYV();

    getG_Histogram();

    printG_Histogram();
    return 0;
}
