/*	YASH ABOOJ
CSCI 580: 3-D Graphics and Rendering
Homework 6: Aniti-Aliasing using Jittered Sub-Sampling
abooj@usc.edu */

/* CS580 Homework 6 */

/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"


int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
/* create a framebuffer:
 -- allocate memory for framebuffer : (sizeof)GzPixel x width x height
 -- pass back pointer 
*/

	*framebuffer = new char[width * height * sizeof(GzPixel)];

	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, GzDisplayClass dispClass, int xRes, int yRes)
{

/* create a display:
  -- allocate memory for indicated class and resolution
  -- pass back pointer to GzDisplay object in display
*/
	*display = new GzDisplay;

	(*display)->xres = xRes;
	(*display)->yres = yRes;
	(*display)->dispClass = dispClass;

	return GZ_SUCCESS;
}


int GzFreeDisplay(GzDisplay	*display)
{
/* clean up, free memory */

	delete display;

	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes, GzDisplayClass	*dispClass)
{
/* pass back values for an open display */

	*xRes = (unsigned short)(display->xres);
	*yRes = (unsigned short)(display->yres);
	*dispClass = display->dispClass;

	return GZ_SUCCESS;
}


int GzInitDisplay(GzDisplay	*display)
{
/* set everything to some default values - start a new frame */

	display->fbuf = new GzPixel[display->xres * display->yres];
	//memset(display->fbuf, 0, display->xres * display->yres * sizeof(GzPixel));

	for (int i = 0; i < (display->xres * display->yres); i++)
	{
		/*display->fbuf[i].red = 128;
		display->fbuf[i].green = 112;
		display->fbuf[i].blue = 96;*/
		display->fbuf[i].red = 127;
		display->fbuf[i].green = 111;
		display->fbuf[i].blue = 95;
		display->fbuf[i].z = INT_MAX;
	}

	return GZ_SUCCESS;
}


int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* write pixel values into the display */

	if ((i < 0) || (j < 0) || (i >(display->xres - 1)) || (j >(display->yres - 1)))
		return GZ_SUCCESS;

	if (r > 4095)
		r = 4095;
	if (g > 4095)
		g = 4095;
	if (b > 4095)
		b = 4095;

	display->fbuf[i + j * display->xres].red = (r & 0x0fff) >> 4;
	display->fbuf[i + j * display->xres].green = (g & 0x0fff) >> 4;
	display->fbuf[i + j * display->xres].blue = (b & 0x0fff) >> 4;
	display->fbuf[i + j * display->xres].alpha = a;
	display->fbuf[i + j * display->xres].z = z;

	return GZ_SUCCESS;
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	/* pass back pixel value in the display */
	/* check display class to see what vars are valid */

	if ((i < 0) || (j < 0) || (i >(display->xres - 1)) || (j >(display->yres - 1)))
		return GZ_SUCCESS;

	*r = display->fbuf[i + j * display->xres].red;
	*g = display->fbuf[i + j * display->xres].green;
	*b = display->fbuf[i + j * display->xres].blue;
	*a = display->fbuf[i + j * display->xres].alpha;
	*z = display->fbuf[i + j * display->xres].z;

	return GZ_SUCCESS;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

	/* write pixels to ppm file based on display class -- "P6 %d %d 255\r" */

	fprintf(outfile, "P%d %d %d %d\n", 3, display->xres, display->yres, 255);

	for (int i = 0; i < display->yres; i++)
	{
		for (int j = 0; j < display->xres; j++)
		{
			fprintf(outfile, "%d %d %d ", display->fbuf[j + i * display->xres].red, display->fbuf[j + i * display->xres].green, display->fbuf[j + i * display->xres].blue);
		}
		fprintf(outfile, "\n");
	}

	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{

	/* write pixels to framebuffer: 
		- Put the pixels into the frame buffer
		- Caution: store the pixel to the frame buffer as the order of blue, green, and red 
		- Not red, green, and blue !!!
	*/

	for (int i = 0; i < (display->xres * display->yres); i++)
	{
		framebuffer[3 * i] = (char)(display->fbuf[i].blue);
		framebuffer[3 * i + 1] = (char)(display->fbuf[i].green);
		framebuffer[3 * i + 2] = (char)(display->fbuf[i].red);
	}

	return GZ_SUCCESS;
}