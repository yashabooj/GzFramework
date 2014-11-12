/*	YASH ABOOJ
CSCI 580: 3-D Graphics and Rendering
Homework 6: Aniti-Aliasing using Jittered Sub-Sampling
abooj@usc.edu */

/* CS580 Homework 6 */

/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"

int bilinInterpolate(GzColor *image, GzColor K, float u, float v, int xs, int ys);
int linearInterpolate(float x, GzColor color);

GzColor	*image;
int xs, ys;
int reset = 1;
int lowerLim = 0, upperLim = 2;
GzColor look_up[11] = 
{ { 1.0, 0.0, 0.0 }, { 0.33, 0.33, 1.0 }, { 1.0, 1.0, 0.0 }, { 0.67, 0.67, 1.0 }, { 0.0, 1.0, 0.0 }, { 1.0, 0.5, 0.5 }, 
  { 0.0, 1.0, 1.0 }, { 0.67, 1.0, 0.67 }, { 0.0, 0.0, 1.0 }, { 0.33, 1.0, 0.33 }, { 0.4, 0.4, 1.0 } };

class Complex
{
public:
	float m_real;
	float m_imag;

public:
	Complex() {
		m_real = 0.0;
		m_imag = 0.0;
	}

	Complex(float real, float imag) {
		m_real = real;
		m_imag = imag;
		
	}

	Complex operator+(Complex &c) {
		return Complex(c.m_real + m_real, c.m_imag + m_imag);
	}

	Complex operator*(Complex &c) {
		return Complex(c.m_real * m_real - c.m_imag * m_imag, c.m_real * m_imag + c.m_imag * m_real);
	}

	void operator=(Complex &c) {
		m_real = c.m_real;
		m_imag = c.m_imag;
	}

	float length() {
		return sqrtf(m_real * m_real + m_imag * m_imag);
	}
};

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

  GzTextureIndex uv;
  uv[U] = u * (float)(xs - 1);
  uv[V] = v * (float)(ys - 1);

  bilinInterpolate(image, color, uv[U], uv[V], xs, ys);

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */
}


/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	//Complex cC(-0.1011, 0.9563), cX(u, v);
	//Complex cC(-0.123, 0.745), cX(2*(u-0.5), 2*(v-0.5));
	Complex cC(-0.72375, 0.26805), cX(2*(u-0.5), 2*(v-0.5));
	int N = 10;
	float len;

	for (int i = 0; i < N; i++) {
		Complex cTemp1 = (cX * cX);
		Complex cTemp2 = (cTemp1 + cC);

		len = cTemp2.length();
		cX = cTemp2;
		if ((len < lowerLim) || (len > upperLim))
			break;
		//cX = cTemp2;
	}
	len = cX.length();
	linearInterpolate(len, color);

	return GZ_SUCCESS;
}

/* Bilinear Interpolation */
int bilinInterpolate(GzColor *image, GzColor K, float u, float v, int xs, int ys)
{
	int p, q;
	float a, b;

	p = (int)v;         // input coordinates
	q = (int)u;
	a = v - (float)p;   // fractional part of input coordinates
	b = u - (float)q;

	// take care of out of bound coordinates
	if (p >= ys)
		p = ys - 1;
	else if (p < 0)
		p = 0;
	if (q >= xs)
		q = xs - 1;
	else if (q < 0)
		q = 0;

	// boundary pixels
	if ((p == ys - 1) && (q == xs - 1))
	{
		K[0] = image[p * xs + q][0];   K[1] = image[p * xs + q][1];   K[2] = image[p * xs + q][2];
	}
	else if (p == ys - 1)
	{
		K[0] = ((1 - b)*image[p * xs + q][0] + b*image[p * xs + (q + 1)][0]);
		K[1] = ((1 - b)*image[p * xs + q][1] + b*image[p * xs + (q + 1)][1]);
		K[2] = ((1 - b)*image[p * xs + q][2] + b*image[p * xs + (q + 1)][2]);
	}
	else if (q == xs - 1)
	{
		K[0] = ((1 - a)*image[p * xs + q][0] + a*image[(p + 1) * xs + q][0]);
		K[1] = ((1 - a)*image[p * xs + q][1] + a*image[(p + 1) * xs + q][1]);
		K[2] = ((1 - a)*image[p * xs + q][2] + a*image[(p + 1) * xs + q][2]);
	}
	// bilinear interpolation
	else
	{
		K[0] = ((1 - a)*(1 - b)*image[p * xs + q][0] + (1 - a)*b*image[p * xs + (q + 1)][0] + a*(1 - b)*image[(p + 1) * xs + q][0] + a*b*image[(p + 1) * xs + (q + 1)][0]);
		K[1] = ((1 - a)*(1 - b)*image[p * xs + q][1] + (1 - a)*b*image[p * xs + (q + 1)][1] + a*(1 - b)*image[(p + 1) * xs + q][1] + a*b*image[(p + 1) * xs + (q + 1)][1]);
		K[2] = ((1 - a)*(1 - b)*image[p * xs + q][2] + (1 - a)*b*image[p * xs + (q + 1)][2] + a*(1 - b)*image[(p + 1) * xs + q][2] + a*b*image[(p + 1) * xs + (q + 1)][2]);
	}

	return GZ_SUCCESS;
}

int linearInterpolate(float x, GzColor color) {
	int xRange;
	
	if (x < lowerLim)	x = lowerLim;
	if (x > upperLim)	x = upperLim;

	float stepSize = (upperLim - lowerLim) / 10.0;

	if (x == lowerLim)
		xRange = 0;
	else
		xRange = (int)((x - lowerLim) / stepSize);

	float a = (x - (lowerLim + xRange * stepSize)) / stepSize;
	float b = (lowerLim + (xRange + 1.0) * stepSize - x) / stepSize;

	for (int i = 0; i < 3; i++) {
		color[i] = a * look_up[xRange + 1][i] + b * look_up[xRange][i];
		if (color[i] > 1.0)	color[i] = 1.0;
		if (color[i] < 0.0)	color[i] = 0.0;
	}

	return GZ_SUCCESS;
}