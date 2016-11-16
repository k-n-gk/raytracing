#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "camera.h"
#include "sphere.h"
#include "hitable_list.h"


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

double random() {
	double Rmax = 1.0 / ((double)RAND_MAX + 1);
	return (double)rand() * Rmax;
}

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(random(), random(), random()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 color(const ray& r,hitable *world) {
	hit_record rec;
	if (world->hit(r,0.0,FLT_MAX,rec)) {
		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
			return  0.5f * vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
		}
	vec3 unit_direction = unit_vector(r.direction());
	float t = 0.5*(unit_direction.y() + 1.0);
	return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
}

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
	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1.0f), 0.5f);
	list[1] = new sphere(vec3(0, -100.5, -1.0f), 100);

	hitable *world = new hitable_list(list, 2);
	camera cam;
	pict.pixels = NULL;
	pict.width = 200;
	pict.height = 100;
	int nx = pict.width;
	int ny = pict.height;
	int ns = 100;
	create_ppm(&pict, pict.width, pict.height);
	int y = 0;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random()) / float(nx);
				float v = float(j + random()) / float(ny);
				ray r = cam.get_ray(u, v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world);
			}
			col /= float(ns);
			pict.pixels[i][y].r = col[0];
			pict.pixels[i][y].g = col[1];
			pict.pixels[i][y].b = col[2];

		}
		y++;
	}
	save_to_file("test.ppm", &pict);
	free_ppm(&pict);
	return 0;
}
