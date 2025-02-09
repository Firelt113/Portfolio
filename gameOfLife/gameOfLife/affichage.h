#pragma once
#include <Windows.h>
#define RAMPLIE 0x000000
#define VIDE 0x000001
#define  WINWIDE 2000
#define  WINHEIGHT 1080
#define  ENVIE 0x00FF00
#define  MORT 0x000000

#define VITTESSE(x) (x*3);


typedef struct BUFFER {
	;
	void* memory;
	int width;
	int height;
}BUFFER;

typedef struct Image {
	int** tab;
	int pixheigt;
	int pixwide;
	int maxheight;
	int maxwide;
};

typedef unsigned int u32;

//0x000000 => VIDE
//0x000000 => RAMPLIE
typedef int RAMPLISSAGE;

void ClearScreen(BUFFER buffer,unsigned int couleur);

int OuDansBuffer(BUFFER buffer, int x, int y);

void EcrireDansBuffer(BUFFER buffer, int PosPixel, unsigned int couleur);

void Line(BUFFER buffer, int x1, int y1, int x2, int y2, unsigned int couleur);

void rectangle(BUFFER buffer ,RAMPLISSAGE ramplissage, int x1, int y1, int x2, int y2, unsigned int couleur);

void cercle(BUFFER buffer, int x, int y, int r, unsigned int couleur);

static int** cree_tableau(void);

Image cree_image(int pix_whide, int pix_heigt);

void ecrire_point(BUFFER buffer, Image image, tagPOINT point, unsigned int couleur);

int verification_nb_blanc(Image image, int x, int y);

void delay(unsigned int nb_tmp);

void copierlimage(Image image, Image image_copie);

void ImageDansBuffer(Image image, BUFFER buffer, tagRECT rectangle);