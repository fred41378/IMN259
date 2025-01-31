#include "MImage.h"

int main(int argc, char *argv[]) {
    MImage img;
    MImage img2;

    if (argc < 2) {
        cout << "Erreur! Veuillez fournir une image en argument." << endl;
        exit(1);
    }

    // Read input image
    img.LoadImage(argv[1]);

    // Compute average intensity
    if (img.GetNumChannels() > 1)
        std::cout << "Average color = (" << img.Average(0) << "," << img.Average(1) << "," << img.Average(2)
                  << ")" << std::endl;
    else
        std::cout << "Average intensity = " << img.Average(0) << std::endl;

    // Compute and print the average contrast
    if (img.GetNumChannels() > 1)
        std::cout << "Average contrast = (" << img.Contrast(0) << "," << img.Contrast(1) << "," << img.Contrast(2)
                  << ")" << std::endl;
    else
        std::cout << "Average contrast = " << img.Contrast(0) << std::endl;

    // Rescale image

    if (img.GetNumChannels() == 1) {
        img.Rescale();
        img.SaveImage("outR.ppm", PGM_RAW);
    }
    else if (img.GetNumChannels() == 3) {
        img.Rescale();
        img.SaveImage("outR.ppm", PPM_RAW);
    }

    // Invert gray scales and color
    if (img.GetNumChannels() == 1) {
        img.Invert();
        img.SaveImage("outI.ppm", PGM_RAW);
    }
    else if (img.GetNumChannels() == 3) {
        img.Invert();
        img.SaveImage("outI.ppm", PPM_RAW);
    }

    // Apply threshold

    if (img.GetNumChannels() == 1) {
        img.Threshold(128);
        img.SaveImage("outT.ppm", PGM_RAW);
    }
    else if (img.GetNumChannels() == 3) {
        img.Threshold(128);
        img.SaveImage("outT.ppm", PPM_RAW);
    }

    return 0;
}
