#include "MImage.h"

#include <fstream>
#include <algorithm>

using namespace std;

void fatal_error(string message)
{
    cout << message << endl;
    exit(1); // abort execution
}

/*
	Default Constructor 
*/
MImage::MImage()
{
    m_width = 0;
    m_height = 0;
    m_num_channels = 0;
    m_num_pixels = 0;
    m_imgbuf = nullptr;
}

/*
	Constructor
*/
MImage::MImage(int width, int height, int channels) {
    m_imgbuf = nullptr;
    Allocate(width, height, channels);
}

/*
	Copy constructor (the rule of three)
*/
MImage::MImage(const MImage& other)
{
    m_imgbuf = nullptr;
    Allocate(other.m_width, other.m_height, other.m_num_channels);

    // Copy values
    memcpy(m_imgbuf, other.m_imgbuf, sizeof(RGBPixel) * m_num_pixels);
}

/*
	Destructor (the rule of three)
*/
MImage::~MImage()
{
    Free();
}

/*
	Operator = (the rule of three)
*/

MImage& MImage::operator=(const MImage& other)
{
    Allocate(other.m_width, other.m_height, other.m_num_channels);

    // Copy values
    memcpy(m_imgbuf, other.m_imgbuf, sizeof(RGBPixel) * m_num_pixels);

    return *this;
}

/*
	Allocate the memory to store an width x height image
*/
void MImage::Allocate(int width, int height, int channels)
{
    m_width = width;
    m_height = height;
    m_num_channels = channels;
    m_num_pixels = m_width * m_height;

    Free();
    m_imgbuf = new RGBPixel[m_num_pixels];
}

/*
	Free the memory used to store the image
*/
void MImage::Free()
{
    if (m_imgbuf != nullptr)
        delete[] m_imgbuf;
}

/* =================================================================================
====================================================================================
======================                    I/O                 ======================
====================================================================================
====================================================================================*/

/*
	load a pgm/ppm image
*/
void MImage::LoadImage(const string fileName) {
// *************** TODO ***************
// ajouter le code pour gérer les formats
// PPM_ASCII et PPM_RAW.  La version actuelle
// du code ne gère que PGM_ASCII et PGM_RAW
// *************** TODO ***************

    if (fileName.empty()) {
        fatal_error("Error: Please specify a filename");
    }
    FILE_FORMAT ff = PGM_ASCII;
    char tmpBuf[500];
    ifstream inFile;
    int maxVal, val;
    char valRaw;
    unsigned char color;
    int width, height, num_channels;

    inFile.open(fileName.c_str(), ios::in);
    if (!inFile.is_open()) {
        fatal_error("Error: can\'t open file '" + fileName + "'");
    }

    inFile.getline(tmpBuf, 500);
    switch (tmpBuf[1]) {
        case '2':
            ff = PGM_ASCII;
            num_channels = 1;
            break;

        case '5':
            ff = PGM_RAW;
            num_channels = 1;
            break;

        default:
            fatal_error("LoadImage Error: Unsupported file type");
            break;
    }

    int nbComm = 0;
    inFile.getline(tmpBuf, 500);
    while (tmpBuf[0] == '#') {
        nbComm++;
        inFile.getline(tmpBuf, 500);
    }
    inFile.close();

    if (ff == PGM_ASCII)
        inFile.open(fileName.c_str(), ios::in);
    else
        inFile.open(fileName.c_str(), ios::in | ios::binary);

    inFile.getline(tmpBuf, 500);
    while (nbComm > 0) {
        nbComm--;
        inFile.getline(tmpBuf, 500);
    }

    inFile >> width;
    inFile >> height;
    inFile >> maxVal;
    Allocate(width, height, num_channels);

    switch (ff) {

        case PGM_ASCII:
            for (int y = 0; y < m_height; y++)
                for (int x = 0; x < m_width; x++) {
                    inFile >> val;
                    at(x, y).r = (float) val * 255.f / maxVal;
                }
            break;

        case PGM_RAW:
            inFile.get(); /* get the \n */
            for (int y = 0; y < m_height; y++)
                for (int x = 0; x < m_width; x++) {
                    int valColor = inFile.get();
                    at(x, y).r = (float) (valColor) * 255.f / maxVal;
                }
            break;


    }

    inFile.close();
    cout << "File opened successfully : " << fileName << endl;
}

/*
	save a pgm/ppm image
*/
void MImage::SaveImage(const string fileName, FILE_FORMAT ff) {
    // *************** TODO ***************
}


/* =================================================================================
====================================================================================
======================           Point Operations             ======================
====================================================================================
====================================================================================*/



/*
	Invert every the intensity/color of every pixel
		Intensity = 255 - oldIntensity
*/
void MImage::Invert() {
    // *************** TODO ***************
}

/*
	Every pixel with an intensity > 'tvalue' are set to 255.  The other ones are set to 0.
*/
void MImage::Threshold(float tvalue) {
    // *************** TODO ***************
}

/*
	Rescale the image between 0 and 255
*/
void MImage::Rescale() {
    // *************** TODO ***************
}

/*
	Compute the average intensity/color value of the image.
	For color images, the "channel" param specifies which channel to process.
	For grayscale images, omit this param
*/
float MImage::Average(int channel = 0) const {
    // *************** TODO ***************
    return -1.0f;
}

/*
	Compute the global contrast of the image
	For color images, the "channel" param specifies which channel to process.
	For grayscale images, omit this param
*/
float MImage::Contrast(int channel = 0) const {
    // *************** TODO ***************
    return -1.0f;
}
