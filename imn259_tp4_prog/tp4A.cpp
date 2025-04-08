#include "MImage.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>


/* 
	Edge detection by thresholding the gradient magnitude 
*/
int main(int argc, char*argv[])
{
	MImage img;
		
	if(argc<3){
		cout << "Erreur: arguments manquants. Arguments a fournir: image threshold" << endl;
		exit(1);
	}
	
	/* Load input image */
	img.LoadImage(argv[1]);
	
	/* Edge detection */
	img.EdgeDetec(strtof(argv[2], nullptr));
	img.SaveImage("outEdgeDetec.pgm",PGM_RAW);
	
	return 0;
}
