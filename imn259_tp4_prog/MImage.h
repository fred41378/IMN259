#ifndef IMAGE_CLASS_H__
#define IMAGE_CLASS_H__

#include <cmath>
#include <cassert>
#include <cstring>
#include <string>
#include <iostream>
#include <limits>

using namespace std;

#define SWAP(a, b) tempr=(a);(a)=(b);(b)=tempr

enum FILE_FORMAT {
    PGM_RAW, PGM_ASCII, PPM_RAW, PPM_ASCII
}; //P2,P5,P3,P6

struct RGBPixel {
    float r; /* RED   Note: r contains the intensity value when image is a gray-scale image */
    float g; /* GREEN Note: 0 for a gray-scale image */
    float b; /* BLUE  Note: 0 for a gray-scale image */
};

class MImage {

private:
    int m_width; /* number of columns */
    int m_height; /* number of rows */
    int m_num_channels; /* number of channels (1 for grayscale, 3 for color) */
    int m_num_pixels;

    RGBPixel *m_imgbuf; // array of RGBPixel

    // 2D element access
    RGBPixel& at(int const x, int const y)
    {
        assert(x >= 0 && x < m_width && y >= 0 && y < m_height);

        return m_imgbuf[m_width * y + x];
    }

    // 2D const element access (exactly the same as above, but forbids any modification to the matrix)
    const RGBPixel& at(int const x, int const y) const
    {
        assert(x >= 0 && x < m_width && y >= 0 && y < m_height);

        return m_imgbuf[m_width * y + x];
    }

public:

    /* Constructors/destructor */
    MImage();
    MImage(int width, int height, int channels);
    MImage(const MImage& other);
    ~MImage();
    MImage& operator=(const MImage& other);
    void Allocate(int width, int height, int channels);
    void Free();

    /* Various functions */
    int GetWidth() const { return m_width; };
    int GetHeight() const { return m_height; };
    int GetNumChannels() const { return m_num_channels; };
    float GetColor(int x, int y) const { return at(x, y).r; };
    float GetColor(int x, int y, int z) const {
        if (z == 0) return at(x, y).r;
        else if (z == 1) return at(x, y).g;
        else return at(x, y).b;
    };
    // one-dimensional indexing
    float GetColorFlat(int index, int z) const {
        if (z == 0) return m_imgbuf[index].r;
        else if (z == 1) return m_imgbuf[index].g;
        else return m_imgbuf[index].b;
    };

    void SetColor(float val, int x, int y) {
        at(x, y).r = at(x, y).g = at(x, y).b = val;
    };
    void SetColor(float val, int x, int y, int z) {
        if (z == 0) at(x, y).r = val;
        else if (z == 1) at(x, y).g = val;
        else at(x, y).b = val;
    };

    bool IsEmpty() const {
        return m_imgbuf == nullptr || m_width <= 0 || m_height <= 0 || m_num_channels <= 0;
    };
    bool SameSize(const MImage &c) {
        return m_width == c.m_width && m_height == c.m_height && m_num_channels == c.m_num_channels;
    };

    /* IO operations */
    void LoadImage(const string imageName);
    void SaveImage(const string imageName, FILE_FORMAT format);
    void SaveSpectralImage(const string imageName, FILE_FORMAT format, bool logTransform);

    /* various */
    float Average(int c) const;
    float Contrast(int c) const;

    /* Point operations */
    void Invert();
    void Threshold(float value);
    void Rescale();
    void CyclRecal();
    void HistogramEqualization();
    void GammaCorrect(float gamma);

    /* Spatial filters */
    void CorrelationFilter(const MImage &corrImg);
    void LowpassGaussianFilter(float sigma);
    void AverageFilter(int halfwinsize);
    void NormGradientFilter(bool radical);
    void LaplacianFilter();

    /* Spectral filters */
    void SpectralIdealLowPassFilter(float radius);
    void SpectralIdealHighPassFilter(float radius);
    void SpectralAverageFilter(int halfwinsize);

    /* Feature extraction */
    void EdgeDetec(float threshold);
    void ZeroCrossing(float sigma, float threshold);
    void KMeansSegmentation();

    /* FFT operations */
    void FFT();
    void IFFT();
};

#endif
