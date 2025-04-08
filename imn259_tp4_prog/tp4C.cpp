#include "MImage.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

/* 
	k-means segmentation
*/
int main(int argc, char*argv[])
{
	MImage img;
	
	if(argc<2){
		cout << "Erreur: arguments manquants. Arguments a fournir: image" << endl;
		exit(1);
	}
	
	/* Read input image */
	img.LoadImage(argv[1]);
	
	/* Segmentation */
	img.KMeansSegmentation(),
	img.SaveImage("outKMeans.pgm",PGM_RAW);
	
	return 0;
}
