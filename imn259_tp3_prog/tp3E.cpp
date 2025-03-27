#include "MImage.h"
#include <ctime>


/*
	Spectral average filter
*/
int main(int argc, char*argv[])
{
	MImage img1,img2;
	
	if(argc<3){
        cout << "Erreur: arguments manquants. Arguments a fournir: image halfWinSize" << endl;
		exit(1);
	}
	
	/* Read input image */
    img1.LoadImage(argv[1]);
	img2 = img1;
	
	/* Spectral filter */
	float time0 = (float)(clock())/CLOCKS_PER_SEC;
	img1.SpectralAverageFilter(atoi(argv[2]));
	printf("Time 1 : %f sec.\n",(float)(clock())/CLOCKS_PER_SEC-time0);
	img1.SaveImage("outSpectralAverage.pgm", PGM_RAW);
	
	/* spatial filter*/
	time0 = (float)(clock())/CLOCKS_PER_SEC;
	img2.AverageFilter(atoi(argv[2]));
	printf("Time 2 : %f sec.\n",(float)(clock())/CLOCKS_PER_SEC-time0);
	img2.SaveImage("outSpatialAverage.pgm", PGM_RAW);
	
	return 0;
}
