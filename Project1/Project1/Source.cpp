#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include "ray.h"


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

float hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0f* dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - 4.0f * a*c;

	if (discriminant < 0.0f) {
		return -1.0;
	}
	else {
		return(-b - sqrt(discriminant)) / (2.0f*a);
	}
}

vec3 color(const ray& r) {
	float des = 2.0;
	float t = hit_sphere(vec3(0, 0, -des), 1, r);
		if (t > 0.0f) {
			vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -des));
			//t = (t > 1) ? 1.0 : t;
			return  0.5f * vec3(N.x()+1, N.y()+1, N.z()+1);
		}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5f*(unit_direction.y() + 1.0f);
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
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);


	pict.pixels = NULL;
	pict.width = 200;
	pict.height = 100;
	int nx = 200;
	int ny = 100;
	create_ppm(&pict, pict.width, pict.height);
	int y = 0;
	for (int j =ny - 1;j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u*horizontal + v*vertical);

			vec3 col = color(r);

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
