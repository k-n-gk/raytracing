#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>


typedef struct _RGB {
	double r;    //red
	double g;    //green
	double b;    //blue
}RGB;

typedef struct _PPM
{
    RGB **pixels;   //pixels[width][height]
    int width;
    int height;
}PPM;


void free_ppm(PPM *ppm)
{
	if (ppm->pixels) {
		if (ppm->pixels[0]) free(ppm->pixels[0]);
		free(ppm->pixels);
	}

	ppm->pixels = NULL;
	ppm->width = 0;
	ppm->height = 0;
}

void create_ppm(PPM *ppm, int width, int height)
{
	int i;
	int size = width*height;

	free_ppm(ppm);

	ppm->width = width;
	ppm->height = height;

	//配列の確保
	ppm->pixels = (RGB**)malloc(ppm->width * sizeof(RGB*));
	ppm->pixels[0] = (RGB*)malloc(size * sizeof(RGB));
	for (i = 1; i<ppm->width; i++) ppm->pixels[i] = ppm->pixels[i - 1] + ppm->height;
}

void save_to_file(char *file_name, PPM *ppm)
{
	int  x, y;
	FILE *fp = fopen(file_name, "w");

	//ファイルフォーマットの書き込み
	fprintf(fp, "P3\n");

	//コメントの書き込み
	fprintf(fp, "# CREATOR: k-n-gk\n");

	//画像サイズと最大値の書き込み
	fprintf(fp, "%d %d\n", ppm->width, ppm->height);
	fprintf(fp, "255\n");

	for (y = 0; y<ppm->height; y++) {
		for (x = 0; x<ppm->width; x++) {
			fprintf(fp, "%d ", (int)(255.99 * ppm->pixels[x][y].r));
			fprintf(fp, "%d ", (int)(255.99 * ppm->pixels[x][y].g));
			fprintf(fp, "%d\n", (int)(255.99 * ppm->pixels[x][y].b));
		}
	}

	fclose(fp);
}

int main() {
	PPM  pict;
	//char file_name[256];

	pict.pixels = NULL;
	pict.width = 300;
	pict.height = 200;
	int nx = 300;
	int ny = 200;
	create_ppm(&pict, pict.width, pict.height);

	for (int j =ny - 1;j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			pict.pixels[i][j].r = float(i) / float(nx);
			pict.pixels[i][j].g = float(j) / float(ny);
			pict.pixels[i][j].b = 0.3;
		}
	}
	save_to_file("test.ppm", &pict);
	free_ppm(&pict);
	return 0;
}
