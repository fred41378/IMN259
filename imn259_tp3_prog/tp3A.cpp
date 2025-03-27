#include "MImage.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <math.h>

/*
 
	Histogram equalization

*/
int main(int argc, char*argv[])
{
	MImage img1;
	
	if(argc<2){
        cout << "Erreur: arguments manquants. Arguments a fournir: image" << endl;
		exit(1);
	}
	
	/* Load input image */
	img1.LoadImage(argv[1]);
	
	/* Histogram equalization */
	img1.HistogramEqualization();
	img1.SaveImage("outHistEq.pgm",PGM_RAW);
	
	return 0;
}
