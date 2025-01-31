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

    if (fileName.empty()) {
        fatal_error("Error: Please specify a filename");
    }
    FILE_FORMAT ff = PGM_ASCII;
    char tmpBuf[500];
    ifstream inFile;
    int maxVal, val, r_val, g_val, b_val;;
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

        case '3':
            ff = PPM_ASCII;
        num_channels = 3;
        break;

        case '5':
            ff = PGM_RAW;
            num_channels = 1;
            break;

        case '6':
            ff = PPM_RAW;
        num_channels = 3;
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

        case PPM_ASCII:
            for (int y = 0; y < m_height; y++)
                for (int x = 0; x < m_width; x++) {
                    inFile >> r_val >> g_val >> b_val;
                    at(x, y).r = (float) r_val * 255.f / maxVal;
                    at(x, y).g = (float) g_val * 255.f / maxVal;
                    at(x, y).b = (float) b_val * 255.f / maxVal;
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

        case PPM_RAW:
            inFile.get();
            for (int y = 0; y < m_height; y++)
                for (int x = 0; x < m_width; x++) {
                    unsigned char rBit = inFile.get();
                    unsigned char gBit = inFile.get();
                    unsigned char bBit = inFile.get();
                    at(x, y).r = (float)(rBit) * 255.f / maxVal;
                    at(x, y).g = (float)(gBit) * 255.f / maxVal;
                    at(x, y).b = (float)(bBit) * 255.f / maxVal;
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

    ofstream outFile;
    if (ff == PGM_RAW || ff == PPM_RAW)
        outFile.open(fileName, ios::binary);
    else
        outFile.open(fileName);

    switch (ff) {
        case PGM_ASCII:
            outFile << "P2\n" << m_width << " " << m_height << "\n" << "255" << "\n";
            for (int y = 0; y < m_height; y++) {
                for (int x = 0; x < m_width; x++) {
                    outFile << (int)at(x, y).r << " ";
                }
                outFile << "\n";
            }
        break;

        case PPM_ASCII:
            outFile << "P3\n" << m_width << " " << m_height << "\n" << "255" << "\n";
            for (int y = 0; y < m_height; y++) {
                for (int x = 0; x < m_width; x++) {
                    outFile << (int)at(x, y).r << " " << (int)at(x, y).g << " " << (int)at(x, y).b << " ";
                }
                outFile << "\n";
        }
        break;

        case PGM_RAW:
            outFile << "P5\n" << m_width << " " << m_height << "\n" << "255" << "\n";
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                unsigned char val = (unsigned char)at(x, y).r;
                outFile.write((char*)&val, 1);
            }
        }
        break;
        case PPM_RAW:
            outFile << "P6\n" << m_width << " " << m_height << "\n" << "255" << "\n";
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                unsigned char r = (unsigned char)at(x, y).r;
                unsigned char g = (unsigned char)at(x, y).g;
                unsigned char b = (unsigned char)at(x, y).b;

                outFile.write((char*)&r, 1);
                outFile.write((char*)&g, 1);
                outFile.write((char*)&b, 1);
            }
        }
        break;
    }
    outFile.close();
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

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // On vérifie si l'image est en gris ou en couleurs
            if (m_num_channels == 1)
            {
                // On inverse le gris
                at(x,y).r = 255 - at(x,y).r;
            }
            else
            {
                // On inverse chaques couleurs
                at(x,y).r = 255 - at(x,y).r;
                at(x,y).g = 255 - at(x,y).g;
                at(x,y).b = 255 - at(x,y).b;
            }
        }
    }
}

/*
	Every pixel with an intensity > 'tvalue' are set to 255.  The other ones are set to 0.
*/
void MImage::Threshold(float tvalue) {

    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            // On vérifie si l'image est en gris ou en couleurs
            if (m_num_channels == 1)
            {
                //On applique le seuil à chaque pixel
                if (at(x,y).r > tvalue) {
                    at(x,y).r = 255;
                }
                else {
                    at(x,y).r = 0;
                }
            }
            else
            {
                // On applique le seuil à chaque couleur de pixel
                if (((at(x, y).r + at(x, y).g + at(x, y).b) / 3) < tvalue) {
                    at(x, y).r = 0;
                    at(x, y).g = 0;
                    at(x, y).b = 0;
                }
                else {
                    at(x, y).r = 255;
                    at(x, y).g = 255;
                    at(x, y).b = 255;
                }
            }

        }
    }
}

/*
	Rescale the image between 0 and 255
*/
void MImage::Rescale() {
    // Valeurs extremes temporaires
    float minVal = std::numeric_limits<float>::max();
    float maxVal = std::numeric_limits<float>::lowest();

    /*
     Pour chaques pixels, on regarde quel est le min et le max
     entre le pixel actuel et le précedent.
     (Initialement, on prend les valeurs extrêmes temporaires)
    */
    for(int i = 0; i < m_num_pixels; ++i) {
        if(m_num_channels == 1) {
            minVal = std::min(minVal, m_imgbuf[i].r);
            maxVal = std::max(maxVal, m_imgbuf[i].r);
        }
        else {
            minVal = std::min({minVal, m_imgbuf[i].r, m_imgbuf[i].g, m_imgbuf[i].b});
            maxVal = std::max({maxVal, m_imgbuf[i].r, m_imgbuf[i].g, m_imgbuf[i].b});
        }
    }

    // Puis, on applique la formule du recalage avec les valeurs min et max
    for(int i = 0; i < m_num_pixels; ++i) {
        if(m_num_channels == 1) {
            m_imgbuf[i].r = ((m_imgbuf[i].r - minVal) / maxVal - minVal) * 255.f;
        }
        else {
            m_imgbuf[i].r = ((m_imgbuf[i].r - minVal) / maxVal - minVal) * 255.f;
            m_imgbuf[i].g = ((m_imgbuf[i].g - minVal) / maxVal - minVal) * 255.f;
            m_imgbuf[i].b = ((m_imgbuf[i].b - minVal) / maxVal - minVal) * 255.f;
        }
    }
}

/*
	Compute the average intensity/color value of the image.
	For color images, the "channel" param specifies which channel to process.
	For grayscale images, omit this param
*/
float MImage::Average(int channel = 0) const {
    float total = 0.0f;
    for(int i = 0; i < m_num_pixels; ++i) {
        total += GetColorFlat(i, channel);
    }
    return total / m_num_pixels;
}

/*
	Compute the global contrast of the image
	For color images, the "channel" param specifies which channel to process.
	For grayscale images, omit this param
*/
float MImage::Contrast(int channel = 0) const {
    float total = 0.0f;
    float moy = Average(channel);
    for(int i = 0; i < m_num_pixels; ++i) {
        total += pow(GetColorFlat(i, channel) - moy, 2) ;
    }
    return sqrt(total / m_num_pixels);
}
