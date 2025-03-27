#include "MImage.h"


/* 
	Low-Pass Gaussian Filter
*/

int main(int argc, char*argv[])
{
	MImage img;
	
	if(argc<3){
        cout << "Erreur: arguments manquants. Arguments a fournir: image sigma" << endl;
		exit(1);
	}
	
	/* Read input image */
	img.LoadImage(argv[1]);
	
	/* Apply Gaussian filter */
	img.LowpassGaussianFilter(strtof(argv[2], nullptr));
	
	/* Save */
	img.SaveImage("outLPG.ppm", PPM_RAW);

	return 0;
}
