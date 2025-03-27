#include "MImage.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include <math.h>

/* 
 	Gamma correction
*/
int main(int argc, char*argv[])
{
	MImage img;
	
	if(argc<3){
		cout << "Erreur: arguments manquants. Arguments a fournir: image gamma" << endl;
		exit(1);
	}
	
	/* Lire les images d'entree */
	img.LoadImage(argv[1]);
	
	/* Apply gamma correction */
	img.GammaCorrect(strtof(argv[2], nullptr));
	img.SaveImage("outGammaCorrected.ppm",PPM_RAW);

	return 0;
}
