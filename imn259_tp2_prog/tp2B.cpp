#include "MImage.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>


/*
	Fourier Transform
*/
int main(int argc, char*argv[])
{
	MImage img1,img2;

	if(argc<2){
		cout << "Erreur: arguments manquants. Arguments a fournir: image" << endl;
		exit(1);
	}

	/* Read the input image */
	img1.LoadImage(argv[1]);
	img2 = img1;

	/* Compute and save the Fourier Transform */
	img1.FFT();
	img1.SaveSpectralImage("outFFT.pgm",PGM_RAW,false);
	img1.SaveSpectralImage("outFFTlogRecal.pgm",PGM_RAW,true);
	img1.IFFT();
	img1.SaveImage("outOriginal.pgm",PGM_RAW);

	/* Cyclic realignment */
	img2.CyclRecal();
	img2.FFT();
	img2.SaveSpectralImage("outFFT_CyclRecal.pgm",PGM_RAW,true);
  	img2.IFFT();

	return 0;
}
