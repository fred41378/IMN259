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

    // if (ff == PGM_ASCII || ff == PPM_RAW)
    //     inFile.open(fileName.c_str(), ios::in);
    // else
    //     inFile.open(fileName.c_str(), ios::in | ios::binary);

    if (ff == PGM_RAW || ff == PPM_RAW)
        inFile.open(fileName.c_str(), ios::in | ios::binary);
    else
        inFile.open(fileName.c_str(), ios::in);

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
        outFile.open(fileName, ios::out | ios::binary);
    else
        outFile.open(fileName, ios::out);

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

/*
    save the MAGNITUDE of the current spectral image
*/
void MImage::SaveSpectralImage(const string fileName, FILE_FORMAT ff, bool logTransform) {

    ofstream outFile;
    if (ff == PGM_RAW)
        outFile.open(fileName, ios::out | ios::binary);
    else
        outFile.open(fileName, ios::out);

    switch (ff) {
        case PGM_ASCII:
            outFile << "P2\n" << m_width << " " << m_height << "\n" << "255" << "\n";

        for (int m = 0; m < m_height; ++m) {
            for (int n = 0; n < m_width; ++n) {
                float real = GetColor(n,m,0);
                float imag = GetColor(n,m,1);
                float mag = sqrt(real*real + imag*imag);

                mag = mag/static_cast<float>(m_height*m_width);

                if (logTransform == true) {
                    mag = 250.0f*log(mag + 1);
                }

                int valeur = static_cast<int>(mag);
                if (valeur > 255) valeur = 255;

                outFile << valeur << " ";
            }
            outFile << "\n";
        }
        break;
        case PGM_RAW:
            outFile << "P5\n" << m_width << " " << m_height << "\n" << "255" << "\n";
        for (int m = 0; m < m_height; ++m) {
            for (int n = 0; n < m_width; ++n) {
                float real = GetColor(n,m,0);
                float imag = GetColor(n,m,1);
                float mag = sqrt(real*real + imag*imag);

                mag = mag/static_cast<float>(m_height*m_width);

                if (logTransform == true) {
                    mag = 250.0f*log(mag + 1);
                }
                int valeur = static_cast<int>(mag);
                if (valeur > 255) valeur = 255;

                outFile.write((char*)&valeur, 1);
            }
        }
        break;
    }
    outFile.close();
}

/* =================================================================================
====================================================================================
===========================           Various            ===========================
====================================================================================
====================================================================================*/
/*
    Compute the average intensity/color value of the image.
    For color images, the "channel" param specifies which channel to process.
    For grayscale images, omit this param
*/
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
            // cout << at(x, y).r << " " << at(x, y).g << " " << at(x, y).b << "\n";
            if (m_num_channels == 1)
            {
                //On applique le seuil à chaque pixel
                if (at(x,y).r < tvalue) {
                    at(x,y).r = 0;
                }
                else {

                    at(x,y).r = 255;
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

    float range = maxVal - minVal;

    // Puis, on applique la formule du recalage avec les valeurs min et max
    for(int i = 0; i < m_num_pixels; ++i) {
        if(m_num_channels == 1) {
            m_imgbuf[i].r = ((m_imgbuf[i].r - minVal) / range) * 255.f;
        }
        else {
            m_imgbuf[i].r = ((m_imgbuf[i].r - minVal) / range) * 255.f;
            m_imgbuf[i].g = ((m_imgbuf[i].g - minVal) / range) * 255.f;
            m_imgbuf[i].b = ((m_imgbuf[i].b - minVal) / range) * 255.f;
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

/*
    multiply the image by -1^(x+y)
*/
void MImage::CyclRecal() {
    for (int m = 0; m < m_height; ++m) {
        for (int n = 0; n < m_width; ++n) {
            at(n,m).r *= static_cast<float>(pow(-1,(n+m)));
        }
    }
}

/*
    Histogram equalization
*/
void MImage::HistogramEqualization() {
    int L = 256;
    float h[L] = {0.0f};
    float p[L] = {0.0f};
    float T[L] = {0.0f};
    float pixelsTotals = static_cast<float>(m_width * m_height);
    MImage g(m_width,m_height,m_num_channels);
    for (int k = 0; k < L - 1; ++k) {
        h[k] = 0;
    }
    for (int n = 0; n < m_height; ++n) {
        for (int m = 0; m < m_width; ++m) {
            int valeur = static_cast<int>(at(m, n).r);
            h[valeur] += 1;
        }
    }
    for (int k = 0; k < L - 1; ++k) {
        p[k] = h[k]/ pixelsTotals;
    }
    T[0] = p[0];
    for (int k = 1; k < L; ++k) {
        T[k] = T[k-1] + p[k];
    }
    for (int n = 0; n < m_height; ++n) {
        for (int m = 0; m < m_width; ++m) {
            int valeur = static_cast<int>(at(m, n).r);
            float nouvelleValeur = T[valeur] * static_cast<float>(L - 1);
            g.at(m, n).r = nouvelleValeur;
        }
    }

    *this = g;
}

/*
    Gamma correction of the current image
*/
void MImage::GammaCorrect(float gamma) {
    // *************** TODO ****************
}


/* =================================================================================
====================================================================================
======================          Spatial filters               ======================
====================================================================================
====================================================================================*/
/*
    Compute the correlation between the current image and 'corrImg'
*/
void MImage::CorrelationFilter(const MImage &corrImg) {
    //On crée une image temporaire avec les dimensions de l'image en paramètre
    MImage temp(m_width, m_height, m_num_channels);

    //On la vide
    for (int y = 0; y < m_height; y++) {
        for (int x = 0; x < m_width; x++) {
            temp.SetColor(0, x, y);
        }
    }

    //Tous les pixels de f sauf les bords
    for (int m = corrImg.GetHeight()/2; m < m_height-corrImg.GetHeight()/2; ++m) {
        for (int n = corrImg.GetWidth()/2; n < m_width-corrImg.GetWidth()/2; ++n) {
            //Tous les pixels de h
            for (int k = -corrImg.GetHeight()/2; k < corrImg.GetHeight()/2; ++k) {
                for (int l = -corrImg.GetWidth()/2; l < corrImg.GetWidth()/2; ++l) {
                    //On applique l'algo de la corrélation à chaque pixel de chaques couleurs
                    // f[k,l]*h[m+k,n+l]
                    temp.at(n,m).r += at(n + l, m + k).r * corrImg.at(corrImg.GetWidth()/2 + l,corrImg.GetHeight()/2 + k).r;
                    temp.at(n,m).g += at(n + l, m + k).g * corrImg.at(corrImg.GetWidth()/2 + l,corrImg.GetHeight()/2 + k).g;
                    temp.at(n,m).b += at(n + l, m + k).b * corrImg.at(corrImg.GetWidth()/2 + l,corrImg.GetHeight()/2 + k).b;
                }
            }
            //Division par le nombre de pixels dans le masque h
            temp.at(n,m).r = temp.at(n,m).r / (float)(corrImg.GetWidth() * corrImg.GetHeight());
            temp.at(n,m).g = temp.at(n,m).g / (float)(corrImg.GetWidth() * corrImg.GetHeight());
            temp.at(n,m).b = temp.at(n,m).b / (float)(corrImg.GetWidth() * corrImg.GetHeight());
        }
    }

    //On recopie les résultats de l'image temporaire dans l'image de base
    for (int m = 0; m < m_height; ++m) {
        for (int n = 0; n < m_width; ++n) {
            at(n, m).r = temp.at(n, m).r;
            at(n, m).g = temp.at(n, m).g;
            at(n, m).b = temp.at(n, m).b;
        }
    }
}

/*
    Implements a low-pass Gaussian filter, sigma is the Gaussian
    standard-deviation
    The mask filter must have a size of : 6*sigma+1 x 6*sigma+1
*/
void MImage::LowpassGaussianFilter(float sigma) {
    // *************** TODO ***************
}

/*
    Computes the local average gray-scale value over a window of size
    (halfwinsize*2+1) x (halfwinsize*2+1)
*/
void MImage::AverageFilter(int halfwinsize) {
    // *************** TODO ***************
}


/* =================================================================================
====================================================================================
======================          Spectral filters               =====================
====================================================================================
====================================================================================*/
/*
    Removes the frequencie located at an euclidean distance > radius
    It is assumed that the current image is a SPECTRAL image.
*/
void MImage::SpectralIdealLowPassFilter(float radius) {
    //On trouve le centre en x et en y
    int centreX = m_width / 2;
    int centreY = m_height / 2;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            //À chaques pixels, on calcule sa coordonnée par rapport au centre
            int dx = x - centreX;
            int dy = y - centreY;
            //Ensuite, on calcule la distance par rapport au centre
            float distance = sqrt(static_cast<float>(dx * dx + dy * dy));

            //Si la distance est à l'extérieur du rayon, on met les valeurs du pixel à 0.
            if (distance > radius) {
                at(x, y).r = 0.0f;
                at(x, y).g = 0.0f;
            }
        }
    }
}



/*
    Removes the frequencies located at an euclidean distance < radius
    It is assumed that the current image is a SPECTRAL image.
*/
void MImage::SpectralIdealHighPassFilter(float radius) {
    //On trouve le centre en x et en y
    int centreX = m_width / 2;
    int centreY = m_height / 2;

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            //À chaques pixels, on calcule sa coordonnée par rapport au centre
            int dx = x - centreX;
            int dy = y - centreY;
            //Ensuite, on calcule la distance par rapport au centre
            float distance = sqrt(static_cast<float>(dx * dx + dy * dy));

            //Si la distance est à l'intérieur du rayon, on met les valeurs du pixel à 0.
            if (distance < radius) {
                at(x, y).r = 0.0f;
                at(x, y).g = 0.0f;
            }
        }
    }
}

/*
    Apply an average low-pass filter via an FFT
*/
void MImage::SpectralAverageFilter(int halfwinsize) {
    // *************** TODO ***************
}


/* =================================================================================
====================================================================================
=======================          FFT operations              =======================
====================================================================================
====================================================================================*/
void fourn(float data[], unsigned long nn[], int ndim, int isign) {
    int idim;
    unsigned long i1, i2, i3, i2rev, i3rev, ip1, ip2, ip3, ifp1, ifp2;
    unsigned long ibit, k1, k2, n, nprev, nrem, ntot;
    float tempi, tempr;
    double theta, wi, wpi, wpr, wr, wtemp;

    for (ntot = 1, idim = 1; idim <= ndim; idim++)
        ntot *= nn[idim];
    nprev = 1;
    for (idim = ndim; idim >= 1; idim--) {
        n = nn[idim];
        nrem = ntot / (n * nprev);
        ip1 = nprev << 1;
        ip2 = ip1 * n;
        ip3 = ip2 * nrem;
        i2rev = 1;
        for (i2 = 1; i2 <= ip2; i2 += ip1) {
            if (i2 < i2rev) {
                for (i1 = i2; i1 <= i2 + ip1 - 2; i1 += 2) {
                    for (i3 = i1; i3 <= ip3; i3 += ip2) {
                        i3rev = i2rev + i3 - i2;
                        SWAP(data[i3], data[i3rev]);
                        SWAP(data[i3 + 1], data[i3rev + 1]);
                    }
                }
            }
            ibit = ip2 >> 1;
            while (ibit >= ip1 && i2rev > ibit) {
                i2rev -= ibit;
                ibit >>= 1;
            }
            i2rev += ibit;
        }
        ifp1 = ip1;
        while (ifp1 < ip2) {
            ifp2 = ifp1 << 1;
            theta = isign * 6.28318530717959 / (ifp2 / ip1);
            wtemp = sin(0.5 * theta);
            wpr = -2.0 * wtemp * wtemp;
            wpi = sin(theta);
            wr = 1.0;
            wi = 0.0;
            for (i3 = 1; i3 <= ifp1; i3 += ip1) {
                for (i1 = i3; i1 <= i3 + ip1 - 2; i1 += 2) {
                    for (i2 = i1; i2 <= ip3; i2 += ifp2) {
                        k1 = i2;
                        k2 = k1 + ifp1;
                        tempr = (float) wr * data[k2] - (float) wi * data[k2 + 1];
                        tempi = (float) wr * data[k2 + 1] + (float) wi * data[k2];
                        data[k2] = data[k1] - tempr;
                        data[k2 + 1] = data[k1 + 1] - tempi;
                        data[k1] += tempr;
                        data[k1 + 1] += tempi;
                    }
                }
                wr = (wtemp = wr) * wpr - wi * wpi + wr;
                wi = wi * wpr + wtemp * wpi + wi;
            }
            ifp1 = ifp2;
        }
        nprev *= n;
    }
}


/*
    Compute the Fourier transform of the current image with the Fast Fourier
    Transform algorithm.  The real part of the FT is copied in
    the RED channel and the imaginary part of the FT in the GREEN
    channel.

    Code adapted from the one by Max Mignotte.

*/
void MImage::FFT() {
    int i, j;
    int posx, posy;

    if (m_width != m_height || !m_width || m_width & (m_width - 1)) {
        fatal_error("MImage::FFT Error:  the image must be square and a power of 2");
    }

    /*Allocate memory */
    float *data = new float[m_width * m_height * 2 + 1];
    unsigned long nn[3];

    /* Fill in data */
    nn[1] = (unsigned long) m_width;
    nn[2] = (unsigned long) m_height;

    for (i = 0; i < m_width; i++)
        for (j = 0; j < m_height; j++) {
            data[2 * (i * m_width + j) + 1] = at(i, j).r;
            data[2 * (i * m_width + j) + 2] = 0;
        }

    /*  GO! */
    fourn(data, nn, 2, 1);

    /* Fill in data and put it back in the image */
    for (i = 0; i < (m_width * m_height); i++) {
        posy = i / m_width;
        posx = i % m_width;
        at(posy, posx).r = data[(2 * i) + 1];
        at(posy, posx).g = data[(2 * i) + 2];
    }

    delete[] data;
}

/*
    Compute the Inverse Fourier transform of the current image with the Fast
    Fourier Transform algorithm.  The real part of the FT is expected to be in
    the RED channel and the imaginary part in the GREEN channel.

    Code adapted from the one by Max Mignotte.

*/
void MImage::IFFT() {
    int i, j;
    int posx, posy;

    if (m_width != m_height || !m_width || m_width & (m_width - 1)) {
        fatal_error("MImage::IFFT Error: the image must be square and a power of 2");
    }

    /*Allocate memory */
    float *data = new float[m_width * m_height * 2 + 1];
    unsigned long nn[3];
    /* Fill in data */
    nn[1] = (unsigned long) m_width;
    nn[2] = (unsigned long) m_height;

    for (i = 0; i < m_width; i++)
        for (j = 0; j < m_height; j++) {
            data[2 * (i * m_width + j) + 1] = at(i, j).r;
            data[2 * (i * m_width + j) + 2] = at(i, j).g;
        }

    /*  GO! */
    fourn(data, nn, 2, -1);

    /* Fill in data and put it back in the image */
    float dims = (float)(m_width * m_height);
    for (i = 0; i < (m_width * m_height); i++) {
        posy = i / m_width;
        posx = i % m_width;

        at(posy, posx).r = data[(2 * i) + 1] / dims;

    }


    delete[] data;
}
