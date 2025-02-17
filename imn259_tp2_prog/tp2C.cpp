#include "MImage.h"

/* 
	Rectangle high-pass and low-pass filter
*/
int main(int argc, char*argv[])
{
	MImage img1,img2;

	if(argc<3){
        cout << "Erreur: arguments manquants. Arguments a fournir: image FilterRadius" << endl;
		exit(1);
	}

	/* Load input image */
	img1.LoadImage(argv[1]);
	img2 = img1;

	/* Compute FFT */
	img1.CyclRecal();
	img2.CyclRecal();
    img1.FFT();
    img2.FFT();

	/* Ideal low-pass filter */
	img1.SaveSpectralImage("outFFT.pgm", PGM_RAW, true);
	img1.SpectralIdealLowPassFilter(strtof(argv[2], nullptr));
	img1.SaveSpectralImage("outFFT_lowPass.pgm", PGM_RAW, true);
	img1.IFFT();
	img1.CyclRecal();
	img1.Rescale();
	img1.SaveImage("out_lowPass.pgm", PGM_RAW);

	/* Ideal high-pass filter */
	img2.SpectralIdealHighPassFilter(strtof(argv[2], nullptr));
	img2.SaveSpectralImage("outFFT_highPass.pgm", PGM_RAW, true);
	img2.IFFT();
	img2.CyclRecal();
	img2.Rescale();
	img2.SaveImage("out_highPass.pgm", PGM_RAW);

	return 0;
}
