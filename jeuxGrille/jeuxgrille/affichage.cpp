#include "affichage.h"
#include <Windows.h>
#include <math.h>

void ClearScreen(BUFFER buffer, unsigned int couleur)
{
	unsigned int* pixel = (unsigned int*)buffer.memory;
	for (int y = 0; y < buffer.height; y++) {
		for (int x = 0; x < buffer.width; x++) {
			*pixel++ = 0x000000;
		}
	}
}

int OuDansBuffer(BUFFER buffer, int x, int y)
{
	int EmpTab;
	if(x > 100 && y > 100 && x < buffer.width - 100 && y < buffer.height-100)
	  return (y  * buffer.width )+ x;

	else {
		return -1;
	}

}

void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur)
{
	if (PosPixel < buffer.height * buffer.width && PosPixel >=0) {
		unsigned int* pixel = (unsigned int*)buffer.memory + PosPixel;
		*pixel = couleur;
	}
}

void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (abs(x1 - x2) >= abs(y1 - y2))
	{
		if (x2 - x1 > 0)
		{
			double a = ((double)y2 - (double)y1) / ((double)x2 - (double)x1);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x1; x < x2; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (x1 - x2 > 0)
		{
			double a;
			if (x1 - x2 == 0)
				a = buffer.height;

			else
				a = (double)y1 - (double)y2 / ((double)x1 - (double)x2);

			double b = (double)y2 - (a * (double)x2);

			for (int x = x2; x < x1; x++)
			{
				int y = a * x + b;
				if (y < buffer.height && x < buffer.width)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	else      //avec y comme independante
	{
		if (y2 - y1 > 0)
		{
			double a = ((double)x2 - (double)x1) / ((double)y2 - (double)y1);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y1; y < y2; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
		else if (y1 - y2 > 0)
		{
			double a = (double)x1 - (double)x2 / ((double)y1 - (double)y2);

			double b = (double)x2 - (a * (double)y2);

			for (int y = y2; y < y1; y++)
			{
				int x = a * y + b;
				if (y < buffer.height && x < buffer.width && y > 0 && x > 0)
					EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}

		}
	}
	
}

void rectangle(BUFFER buffer, RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur)
{
	if (ramplissage == VIDE) {
		Line(buffer, x1, y1, x1, y2, couleur);
		Line(buffer, x1, y1, x2, y1, couleur);
		Line(buffer, x2, y1, x2, y2, couleur);
		Line(buffer, x1, y2, x2, y2, couleur);
	}
	else if(ramplissage == RAMPLIE)
	{
		unsigned int* pixel = (unsigned int*)buffer.memory;
		for (int y = y1; y < y2; y++) {
			for (int x = x1; x < x2; x++) {
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, x, y), couleur);
			}
		}
	}
}

int** cree_tableau()
{
	int** tab = (int**)malloc(WINHEIGHT * sizeof(int*));

	for (int i = 0; i < WINHEIGHT; i++)
	{
		tab[i] = (int*)malloc(WINWIDE * sizeof(int));
	}

	return tab;
}

Image cree_image(int pix_whide, int pix_heigt)
{
	Image image;
	image.pixheigt = pix_heigt;
	image.pixwide = pix_whide;
	image.tab = cree_tableau();
	return image;
}

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur)
{
	for (int i = x - r; i < x + r; i++)
		for (int j = y - r; j < y + r; j++)
		{
			if (sqrt(pow(j - y, 2) + pow(i - x, 2)) <= r)
				EcrireDansBuffer(buffer, OuDansBuffer(buffer, i, j), couleur);
		}
}

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int couleur)
{
	double ratio = (double)image.pixheigt / (double)image.pixwide;

	tagPOINT point1 = { point.x, point.y };
	tagPOINT point2 = { point.x + image.pixwide / 2, point.y + image.pixheigt / 2 };

	while (point1.x < point.x + (image.pixwide/2)) {
		Line(buffer, point1.x, point1.y, point2.x, point2.y, couleur);
		point1.x++;
		point2.x++;

		point1.y = (point.x - point1.x) * ratio + point.y;
		point2.y = (point.x + (image.pixwide/2) - point2.x) * ratio + point.y+ (image.pixheigt/2);
	}
}

int verification_nb_blanc(Image image, int x, int y)
{
	int nb = 0;
	for(int j = y-1; j< y + 2; j++)
		for (int i = x - 1; i < x + 2; i++)
		{
			if ((i != x || j != y)&& image.tab[j][i] == ENVIE)
			{
				nb++;
			}
		}
	return nb;
}

void delay(unsigned int nb_tmp)
{
	int i = 0;
	while (i < nb_tmp)
		i++;;
}

void copierlimage(Image image, Image image_copie)
{
	for(int j = 0; j< WINHEIGHT; j++)
		for (int i = 0; i < WINWIDE; i++) {
			image_copie.tab[j][i] = image.tab[j][i];
		}
}

void ImageDansBuffer(Image image, BUFFER buffer, tagRECT rectanglee)
{
	for (int j = 1; j < (buffer.height  * 2/ image.pixheigt) - 1;j++)
		for (int i =1; i < (buffer.width * 2 / image.pixwide) * 1.05 - 1;i++)
		{
			tagPOINT point;
			
			point.x = (i * ((double)image.pixwide / 2)) + (j * ((double)image.pixwide)/2) - (buffer.width/1.7) ;
			point.y = (buffer.height/2) + (i * ((double)image.pixheigt/2)) - (j * ((double)image.pixheigt/2)) ;

			if (i + rectanglee.left < WINWIDE && j + rectanglee.bottom < WINHEIGHT && j + rectanglee.bottom - (buffer.height / image.pixheigt) >= 0 && i + rectanglee.left - (buffer.width / image.pixwide) >=0 && point.x >= 0 && point.y > 20 && point.y < buffer.height - 20 && point.x < buffer.width - 80) {
				ecrire_point(buffer, image, point,  image.tab[j + rectanglee.bottom - (buffer.height / image.pixheigt)][i + rectanglee.left - (buffer.width / image.pixwide)]);   //EcrireDansBuffer(buffer,OuDansBuffer(buffer,i,j),image.tab[j][i]);
			}
		}
}

int hypotenuse(double x, double y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}