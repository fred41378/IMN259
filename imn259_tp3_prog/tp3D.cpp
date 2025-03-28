#include "MImage.h"

/* 
	Average Filter
*/
int main(int argc, char*argv[])
{
	MImage img;
	MImage corr;
	
	if(argc<3){
		cout << "Erreur: arguments manquants. Arguments a fournir: image halfwinsize" << endl;
		exit(1);
	}
	
	/* Read input image */
	img.LoadImage(argv[1]);
	
	/* Apply average filter */
	img.AverageFilter(atoi(argv[2]));
	
	/* Save */
	if (img.GetNumChannels() == 1) {
		img.SaveImage("outAverage.ppm", PGM_RAW);
	}
	else if (img.GetNumChannels() == 3) {
		img.SaveImage("outAverage.ppm", PPM_RAW);
	}

	return 0;
}
