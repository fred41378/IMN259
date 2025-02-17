#include "MImage.h"


/* 
	Correlation
*/

int main(int argc, char*argv[])
{
	MImage img;
	MImage corr;

	if(argc<3){
		cout << "Erreur: arguments manquants. Arguments a fournir: image correlationImage" << endl;
		exit(1);
	}
	
	/* read input image */
    img.LoadImage(argv[1]);
    corr.LoadImage(argv[2]);

	/* Correlate img with corr */
	img.CorrelationFilter(corr);
	img.Rescale();
	
	/* Save  */
	img.SaveImage("outCorr.ppm", PPM_RAW);

	return 0;
}
