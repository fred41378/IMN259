#include "MImage.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

/* 
	Zero Crossing
*/
int main(int argc, char*argv[])
{
	MImage img;
	MImage imgTest;
	
	if(argc<4){
		cout << "Erreur: arguments manquants. Arguments a fournir: image sigma threshold" << endl;
		exit(1);
	}
	
	/* Load input image */
	img.LoadImage(argv[1]);

	/* Test the Laplacian filter */
	imgTest = img;
	imgTest.LaplacianFilter();
	imgTest.Rescale();
	imgTest.SaveImage("outLaplacian.pgm",PGM_RAW);

	/* Edge detection (Gaussian and Laplacian)*/;
	img.ZeroCrossing(strtof(argv[2], nullptr), strtof(argv[3], nullptr));
	img.SaveImage("outZeroCrossing.pgm",PGM_RAW);
	
	return 0;
}
