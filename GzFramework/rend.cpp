/*	YASH ABOOJ
CSCI 580: 3-D Graphics and Rendering
Homework 6: Aniti-Aliasing using Jittered Sub-Sampling
abooj@usc.edu */

/* CS580 Homework 6 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI 3.14159265

int GzRotXMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value

	float cosTheta = cos(degree * PI / 180.0);
	float sinTheta = sin(degree * PI / 180.0);

	mat[0][0] = 1.0;	mat[0][1] = 0.0;		mat[0][2] = 0.0;		mat[0][3] = 0.0;
	mat[1][0] = 0.0;	mat[1][1] = cosTheta;	mat[1][2] = -sinTheta;	mat[1][3] = 0.0;
	mat[2][0] = 0.0;	mat[2][1] = sinTheta;	mat[2][2] = cosTheta;	mat[2][3] = 0.0;
	mat[3][0] = 0.0;	mat[3][1] = 0.0;		mat[3][2] = 0.0;		mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value

	float cosTheta = cos(degree * PI / 180.0);
	float sinTheta = sin(degree * PI / 180.0);

	mat[0][0] = cosTheta;	mat[0][1] = 0.0;	mat[0][2] = sinTheta;	mat[0][3] = 0.0;
	mat[1][0] = 0.0;		mat[1][1] = 1.0;	mat[1][2] = 0.0;		mat[1][3] = 0.0;
	mat[2][0] = -sinTheta;	mat[2][1] = 0.0;	mat[2][2] = cosTheta;	mat[2][3] = 0.0;
	mat[3][0] = 0.0;		mat[3][1] = 0.0;	mat[3][2] = 0.0;		mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value

	float cosTheta = cos(degree * PI / 180.0);
	float sinTheta = sin(degree * PI / 180.0);

	mat[0][0] = cosTheta;	mat[0][1] = -sinTheta;	mat[0][2] = 0.0;	mat[0][3] = 0.0;
	mat[1][0] = sinTheta;	mat[1][1] = cosTheta;	mat[1][2] = 0.0;	mat[1][3] = 0.0;
	mat[2][0] = 0.0;		mat[2][1] = 0.0;		mat[2][2] = 1.0;	mat[2][3] = 0.0;
	mat[3][0] = 0.0;		mat[3][1] = 0.0;		mat[3][2] = 0.0;	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
// Create translation matrix
// Pass back the matrix using mat value

	mat[0][0] = 1.0;	mat[0][1] = 0.0;	mat[0][2] = 0.0;	mat[0][3] = translate[X];
	mat[1][0] = 0.0;	mat[1][1] = 1.0;	mat[1][2] = 0.0;	mat[1][3] = translate[Y];
	mat[2][0] = 0.0;	mat[2][1] = 0.0;	mat[2][2] = 1.0;	mat[2][3] = translate[Z];
	mat[3][0] = 0.0;	mat[3][1] = 0.0;	mat[3][2] = 0.0;	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
// Create scaling matrix
// Pass back the matrix using mat value

	mat[0][0] = scale[X];	mat[0][1] = 0.0;		mat[0][2] = 0.0;		mat[0][3] = 0.0;
	mat[1][0] = 0.0;		mat[1][1] = scale[Y];	mat[1][2] = 0.0;		mat[1][3] = 0.0;
	mat[2][0] = 0.0;		mat[2][1] = 0.0;		mat[2][2] = scale[Z];	mat[2][3] = 0.0;
	mat[3][0] = 0.0;		mat[3][1] = 0.0;		mat[3][2] = 0.0;		mat[3][3] = 1.0;

	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay	*display)
{
/*  
- malloc a renderer struct 
- keep closed until all inits are done 
- setup Xsp and anything only done once 
- span interpolator needs pointer to display 
- check for legal class GZ_Z_BUFFER_RENDER 
- init default camera 
*/ 

	*render = new GzRender;

	// initialize GzRender parameters
	(*render)->renderClass	= renderClass;
	(*render)->display		= display;
	(*render)->open			= 0;
	(*render)->matlevel		= -1;
	(*render)->numlights	= 0;

	// set up default GzCamera
	(*render)->camera.FOV = DEFAULT_FOV;
	(*render)->camera.worldup[X] = 0.0;		(*render)->camera.lookat[X] = 0.0;	(*render)->camera.position[X] = DEFAULT_IM_X;
	(*render)->camera.worldup[Y] = 1.0;		(*render)->camera.lookat[Y] = 0.0;	(*render)->camera.position[Y] = DEFAULT_IM_Y;
	(*render)->camera.worldup[Z] = 0.0;		(*render)->camera.lookat[Z] = 0.0;	(*render)->camera.position[Z] = DEFAULT_IM_Z;

	// set up default light coefficeints
	GzColor Ka = DEFAULT_AMBIENT;
	(*render)->Ka[RED] = Ka[RED];	(*render)->Ka[GREEN] = Ka[GREEN];	(*render)->Ka[BLUE] = Ka[BLUE];
	GzColor Kd = DEFAULT_DIFFUSE;
	(*render)->Kd[RED] = Kd[RED];	(*render)->Kd[GREEN] = Kd[GREEN];	(*render)->Kd[BLUE] = Kd[BLUE];
	GzColor Ks = DEFAULT_SPECULAR;
	(*render)->Ks[RED] = Ks[RED];	(*render)->Ks[GREEN] = Ks[GREEN];	(*render)->Ks[BLUE] = Ks[BLUE];
	(*render)->spec = DEFAULT_SPEC;

	// initialize the perspective-to-screen transformation
	float xs = (*render)->display->xres;
	float ys = (*render)->display->yres;
	float zs = (float)INT_MAX * tan((*render)->camera.FOV * PI / 360.0);

	(*render)->Xsp[0][0] = xs / 2.0;(*render)->Xsp[0][1] = 0.0;			(*render)->Xsp[0][2] = 0.0;	(*render)->Xsp[0][3] = xs / 2.0;
	(*render)->Xsp[1][0] = 0.0;		(*render)->Xsp[1][1] = -ys / 2.0;	(*render)->Xsp[1][2] = 0.0;	(*render)->Xsp[1][3] = ys / 2.0;
	(*render)->Xsp[2][0] = 0.0;		(*render)->Xsp[2][1] = 0.0;			(*render)->Xsp[2][2] = zs;	(*render)->Xsp[2][3] = 0.0;
	(*render)->Xsp[3][0] = 0.0;		(*render)->Xsp[3][1] = 0.0;			(*render)->Xsp[3][2] = 0.0;	(*render)->Xsp[3][3] = 1.0;

	return GZ_SUCCESS;
}


int GzFreeRender(GzRender *render)
{
/* 
-free all renderer resources
*/

	delete render;

	return GZ_SUCCESS;
}


int GzBeginRender(GzRender *render)
{
/*  
- set up for start of each frame - clear frame buffer 
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms if it want to. 
*/ 

	render->open = 1;

	// set up the perspective-to-screen transformation
	float xs = render->display->xres;
	float ys = render->display->yres;
	float zs = (float)INT_MAX * tan(render->camera.FOV * PI / 360.0);

	render->Xsp[0][0] = xs / 2.0;	render->Xsp[0][1] = 0.0;		render->Xsp[0][2] = 0.0;	render->Xsp[0][3] = xs / 2.0;
	render->Xsp[1][0] = 0.0;		render->Xsp[1][1] = -ys / 2.0;	render->Xsp[1][2] = 0.0;	render->Xsp[1][3] = ys / 2.0;
	render->Xsp[2][0] = 0.0;		render->Xsp[2][1] = 0.0;		render->Xsp[2][2] = zs;		render->Xsp[2][3] = 0.0;
	render->Xsp[3][0] = 0.0;		render->Xsp[3][1] = 0.0;		render->Xsp[3][2] = 0.0;	render->Xsp[3][3] = 1.0;

	// set up the image-to-perspective transformation
	render->camera.Xpi[0][0] = 1.0;	render->camera.Xpi[0][1] = 0.0;
	render->camera.Xpi[0][2] = 0.0;	render->camera.Xpi[0][3] = 0.0;
	render->camera.Xpi[1][0] = 0.0;	render->camera.Xpi[1][1] = 1.0;
	render->camera.Xpi[1][2] = 0.0;	render->camera.Xpi[1][3] = 0.0;
	render->camera.Xpi[2][0] = 0.0;	render->camera.Xpi[2][1] = 0.0;
	render->camera.Xpi[2][2] = 1.0;	render->camera.Xpi[2][3] = 0.0;
	render->camera.Xpi[3][0] = 0.0;	render->camera.Xpi[3][1] = 0.0;
	render->camera.Xpi[3][2] = tan(render->camera.FOV * PI / 360.0);
	render->camera.Xpi[3][3] = 1.0;

	GzCoord cameraZ;
	cameraZ[X] = render->camera.lookat[X] - render->camera.position[X];
	cameraZ[Y] = render->camera.lookat[Y] - render->camera.position[Y];
	cameraZ[Z] = render->camera.lookat[Z] - render->camera.position[Z];
	float normalizeZ = sqrt(pow(cameraZ[X], 2) + pow(cameraZ[Y], 2) + pow(cameraZ[Z], 2));
	cameraZ[X] = cameraZ[X] / normalizeZ;	cameraZ[Y] = cameraZ[Y] / normalizeZ;	cameraZ[Z] = cameraZ[Z] / normalizeZ;

	GzCoord cameraY;
	float UpDotZ = GzDotProduct(render->camera.worldup, cameraZ);
	cameraY[X] = render->camera.worldup[X] - UpDotZ * cameraZ[X];
	cameraY[Y] = render->camera.worldup[Y] - UpDotZ * cameraZ[Y];
	cameraY[Z] = render->camera.worldup[Z] - UpDotZ * cameraZ[Z];
	float normalizeY = sqrt(pow(cameraY[X], 2) + pow(cameraY[Y], 2) + pow(cameraY[Z], 2));
	cameraY[X] = cameraY[X] / normalizeY;	cameraY[Y] = cameraY[Y] / normalizeY;	cameraY[Z] = cameraY[Z] / normalizeY;

	GzCoord cameraX;
	cameraX[X] = cameraY[Y] * cameraZ[Z] - cameraY[Z] * cameraZ[Y];
	cameraX[Y] = cameraY[Z] * cameraZ[X] - cameraY[X] * cameraZ[Z];
	cameraX[Z] = cameraY[X] * cameraZ[Y] - cameraY[Y] * cameraZ[X];

	float XDotPos = GzDotProduct(render->camera.position, cameraX);
	float YDotPos = GzDotProduct(render->camera.position, cameraY);
	float ZDotPos = GzDotProduct(render->camera.position, cameraZ);

	render->camera.Xiw[0][0] = cameraX[X];	render->camera.Xiw[0][1] = cameraX[Y];
	render->camera.Xiw[0][2] = cameraX[Z];	render->camera.Xiw[0][3] = -XDotPos;
	render->camera.Xiw[1][0] = cameraY[X];	render->camera.Xiw[1][1] = cameraY[Y];
	render->camera.Xiw[1][2] = cameraY[Z];	render->camera.Xiw[1][3] = -YDotPos;
	render->camera.Xiw[2][0] = cameraZ[X];	render->camera.Xiw[2][1] = cameraZ[Y];
	render->camera.Xiw[2][2] = cameraZ[Z];	render->camera.Xiw[2][3] = -ZDotPos;
	render->camera.Xiw[3][0] = 0.0;			render->camera.Xiw[3][1] = 0.0;
	render->camera.Xiw[3][2] = 0.0;			render->camera.Xiw[3][3] = 1.0;

	GzMatrix identity;
	GzIdentityMatrix(identity);

	render->matlevel = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			render->Ximage[render->matlevel][i][j] = render->Xsp[i][j];
	GzIdentityMatrix(render->Xnorm[render->matlevel]);

	(render->matlevel)++;
	GzMatrixMultiply(render->Ximage[render->matlevel], render->Ximage[render->matlevel - 1], render->camera.Xpi);
	GzMatrixMultiply(render->Xnorm[render->matlevel], render->Xnorm[render->matlevel - 1], identity);

	GzMatrix temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp[i][j] = render->camera.Xiw[i][j];
	temp[0][3] = 0.0;	temp[1][3] = 0.0;	temp[2][3] = 0.0;

	(render->matlevel)++;
	GzMatrixMultiply(render->Ximage[render->matlevel], render->Ximage[render->matlevel - 1], render->camera.Xiw);
	GzMatrixMultiply(render->Xnorm[render->matlevel], render->Xnorm[render->matlevel - 1], temp);

	return GZ_SUCCESS;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
/*
- overwrite renderer camera structure with new camera definition
*/

	render->camera.FOV = camera->FOV;
	render->camera.worldup[X] = camera->worldup[X];		render->camera.lookat[X] = camera->lookat[X];
	render->camera.worldup[Y] = camera->worldup[Y];		render->camera.lookat[Y] = camera->lookat[Y];
	render->camera.worldup[Z] = camera->worldup[Z];		render->camera.lookat[Z] = camera->lookat[Z];
	render->camera.position[X] = camera->position[X];
	render->camera.position[Y] = camera->position[Y];
	render->camera.position[Z] = camera->position[Z];

	return GZ_SUCCESS;	
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
/*
- push a matrix onto the Ximage stack
- check for stack overflow
*/

	if (render->matlevel >= (MATLEVELS - 1)) {
		perror("Transformation Stack Overflow");
		exit(1);
	}

	(render->matlevel)++;
	GzMatrixMultiply(render->Ximage[render->matlevel], render->Ximage[render->matlevel - 1], matrix);

	GzMatrix temp;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp[i][j] = matrix[i][j];

	temp[0][3] = 0.0; temp[1][3] = 0.0; temp[2][3] = 0.0;
	float normRow1, normRow2, normRow3;
	normRow1 = sqrt(pow(temp[0][0], 2) + pow(temp[0][1], 2) + pow(temp[0][2], 2));
	normRow2 = sqrt(pow(temp[1][0], 2) + pow(temp[1][1], 2) + pow(temp[1][2], 2));
	normRow3 = sqrt(pow(temp[2][0], 2) + pow(temp[2][1], 2) + pow(temp[2][2], 2));
	temp[0][0] = temp[0][0] / normRow1;	temp[0][1] = temp[0][1] / normRow1;	temp[0][2] = temp[0][2] / normRow1;
	temp[1][0] = temp[1][0] / normRow2;	temp[1][1] = temp[1][1] / normRow2;	temp[1][2] = temp[1][2] / normRow2;
	temp[2][0] = temp[2][0] / normRow3;	temp[2][1] = temp[2][1] / normRow3;	temp[2][2] = temp[2][2] / normRow3;
	GzMatrixMultiply(render->Xnorm[render->matlevel], render->Xnorm[render->matlevel - 1], temp);

	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/

	if (render->matlevel == (-1)) {
		perror("Transformation Stack Underflow");
		exit(1);
	}

	(render->matlevel)--;

	return GZ_SUCCESS;
}

int GzIdentityMatrix(GzMatrix mat)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			mat[i][j] = 0;

	mat[0][0] = 1.0;	mat[1][1] = 1.0;	mat[2][2] = 1.0;	mat[3][3] = 1.0;

	return GZ_SUCCESS;
}

int GzMatrixMultiply(GzMatrix product, GzMatrix A, GzMatrix B)
{
/*
- multiply matrix A with matrix B
*/

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			product[i][j] = 0.0;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				product[i][j] = product[i][j] + A[i][k] * B[k][j];

	return GZ_SUCCESS;
}

float GzDotProduct(GzCoord vectorX, GzCoord vectorY)
{
	float dotProduct = vectorX[X] * vectorY[X] + vectorX[Y] * vectorY[Y] + vectorX[Z] * vectorY[Z];

	return dotProduct;
}

int GzTransformCoordinates(GzCoord xformCoord, GzCoord origCoord, GzMatrix XformMat)
{
	float vertex[4];
	for (int i = 0; i < 4; i++)
		vertex[i] = XformMat[i][0] * origCoord[X] + XformMat[i][1] * origCoord[Y] + 
			XformMat[i][2] * origCoord[Z] + XformMat[i][3];

	for (int i = 0; i < 3; i++) {
		xformCoord[i] = vertex[i] / vertex[3];
	}

	xformCoord[0] = xformCoord[0];
	xformCoord[1] = xformCoord[1];

	return GZ_SUCCESS;
}

int GzTransformNormals(GzCoord xformNorm, GzCoord origNorm, GzMatrix XformMat)
{
	float vertex[4];
	for (int i = 0; i < 4; i++)
		vertex[i] = XformMat[i][0] * origNorm[X] + XformMat[i][1] * origNorm[Y] +
		XformMat[i][2] * origNorm[Z] + XformMat[i][3];

	for (int i = 0; i < 3; i++) {
		xformNorm[i] = vertex[i] / vertex[3];
	}

	float norm = sqrt(pow(xformNorm[0], 2) + pow(xformNorm[1], 2) + pow(xformNorm[2], 2));
	for (int i = 0; i < 3; i++) {
		xformNorm[i] = xformNorm[i] / norm;
	}

	return GZ_SUCCESS;
}

int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer	*valueList) /* void** valuelist */
{
/*
- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
- later set shaders, interpolaters, texture maps, and lights
*/
	
	for (int i = 0; i < numAttributes; i++) {

		switch (nameList[i]) {
		case GZ_RGB_COLOR:
			render->flatcolor[RED]		= ((float*)valueList[i])[RED];
			render->flatcolor[GREEN]	= ((float*)valueList[i])[GREEN];
			render->flatcolor[BLUE]		= ((float*)valueList[i])[BLUE];
			break;
		case GZ_DIRECTIONAL_LIGHT:
			if (render->numlights == MAX_LIGHTS) {
				perror("No more lights can be accommodated...\n");
				continue;
			}
			render->lights[render->numlights++] = ((GzLight*)valueList[i])[0];
			break;
		case GZ_AMBIENT_LIGHT:
			render->ambientlight = ((GzLight*)valueList[i])[0];
			break;
		case GZ_DIFFUSE_COEFFICIENT:
			render->Kd[RED]		= ((float*)valueList[i])[RED];
			render->Kd[GREEN]	= ((float*)valueList[i])[GREEN];
			render->Kd[BLUE]	= ((float*)valueList[i])[BLUE];
			break;
		case GZ_INTERPOLATE:
			render->interp_mode = ((int*)valueList[i])[0];
			break;
		case GZ_AMBIENT_COEFFICIENT:
			render->Ka[RED]		= ((float*)valueList[i])[RED];
			render->Ka[GREEN]	= ((float*)valueList[i])[GREEN];
			render->Ka[BLUE]	= ((float*)valueList[i])[BLUE];
			break;
		case GZ_SPECULAR_COEFFICIENT:
			render->Ks[RED]		= ((float*)valueList[i])[RED];
			render->Ks[GREEN]	= ((float*)valueList[i])[GREEN];
			render->Ks[BLUE]	= ((float*)valueList[i])[BLUE];
			break;
		case GZ_DISTRIBUTION_COEFFICIENT:
			render->spec = ((float*)valueList[i])[0];
			break;
		case GZ_TEXTURE_MAP:
			render->tex_fun = (GzTexture)valueList[i];
			break;
		case GZ_AASHIFTX:
			render->xOffset = ((float*)valueList[i])[0];
			break;
		case GZ_AASHIFTY:
			render->yOffset = ((float*)valueList[i])[0];
			break;
		default:
			break;
		}
	}

	return GZ_SUCCESS;
}

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, 
				  GzPointer	*valueList)
/* numParts : how many names and values */
{
/*  
- pass in a triangle description with tokens and values corresponding to 
      GZ_POSITION:3 vert positions in model space 
- Xform positions of verts  
- Clip - just discard any triangle with verts behind view plane 
       - test for triangles with all three verts off-screen 
- invoke triangle rasterizer  
*/ 

	GzCoord Vx, Vy, Vz;
	GzCoord Nx, Ny, Nz;
	GzTextureIndex UV[3];

	for (int i = 0; i < numParts; i++) {

		switch (nameList[i]) {
		case GZ_POSITION:
			// read triangle coordinates from the vertex list
			Vx[X] = ((float*)valueList[i])[0]; Vy[X] = ((float*)valueList[i])[3]; Vz[X] = ((float*)valueList[i])[6];
			Vx[Y] = ((float*)valueList[i])[1]; Vy[Y] = ((float*)valueList[i])[4]; Vz[Y] = ((float*)valueList[i])[7];
			Vx[Z] = ((float*)valueList[i])[2]; Vy[Z] = ((float*)valueList[i])[5]; Vz[Z] = ((float*)valueList[i])[8];
			break;
		case GZ_NORMAL:
			// read vertex normals from the normal list
			Nx[X] = ((float*)valueList[i])[0]; Ny[X] = ((float*)valueList[i])[3]; Nz[X] = ((float*)valueList[i])[6];
			Nx[Y] = ((float*)valueList[i])[1]; Ny[Y] = ((float*)valueList[i])[4]; Nz[Y] = ((float*)valueList[i])[7];
			Nx[Z] = ((float*)valueList[i])[2]; Ny[Z] = ((float*)valueList[i])[5]; Nz[Z] = ((float*)valueList[i])[8];
			break;
		case GZ_TEXTURE_INDEX:
			// read texture indices from the texture list
			UV[0][U] = ((float*)valueList[i])[0];	UV[0][V] = ((float*)valueList[i])[1];
			UV[1][U] = ((float*)valueList[i])[2];	UV[1][V] = ((float*)valueList[i])[3];
			UV[2][U] = ((float*)valueList[i])[4];	UV[2][V] = ((float*)valueList[i])[5];
		default:
			break;
		}
	}

	/* BARYCENTRIC TRIANGLE RASTERIZATION */

	// apply transformations
	GzCoord V1, V2, V3;
	GzTransformCoordinates(V1, Vx, render->Ximage[render->matlevel]);
	GzTransformCoordinates(V2, Vy, render->Ximage[render->matlevel]);
	GzTransformCoordinates(V3, Vz, render->Ximage[render->matlevel]);
	V1[X] -= render->xOffset;	V1[Y] -= render->yOffset;
	V2[X] -= render->xOffset;	V2[Y] -= render->yOffset;
	V3[X] -= render->xOffset;	V3[Y] -= render->yOffset;

	GzCoord N1, N2, N3;
	GzTransformNormals(N1, Nx, render->Xnorm[render->matlevel]);
	GzTransformNormals(N2, Ny, render->Xnorm[render->matlevel]);
	GzTransformNormals(N3, Nz, render->Xnorm[render->matlevel]);

	GzCoord x, y, z;
	x[0] = V1[X];	x[1] = V2[X];	x[2] = V3[X];
	y[0] = V1[Y];	y[1] = V2[Y];	y[2] = V3[Y];
	z[0] = V1[Z];	z[1] = V2[Z];	z[2] = V3[Z];

	// compute the coefficients of edge line equations
	float A[2], B[2], C[2];

	A[0] = y[0] - y[1];
	B[0] = x[1] - x[0];
	C[0] = x[0] * y[1] - x[1] * y[0];

	A[1] = y[1] - y[2];
	B[1] = x[2] - x[1];
	C[1] = x[1] * y[2] - x[2] * y[1];

	// calculate the bounding box limits
	int xmin, xmax, ymin, ymax;
	xmin = ((int)x[0] < (int)x[1]) ? (((int)x[0] < (int)x[2]) ? (int)x[0] : (int)x[2]) : (((int)x[1] < (int)x[2]) ? (int)x[1] : (int)x[2]);
	ymin = ((int)y[0] < (int)y[1]) ? (((int)y[0] < (int)y[2]) ? (int)y[0] : (int)y[2]) : (((int)y[1] < (int)y[2]) ? (int)y[1] : (int)y[2]);
	xmax = ((int)x[0] > (int)x[1]) ? (((int)x[0] > (int)x[2]) ? (int)x[0] : (int)x[2]) : (((int)x[1] > (int)x[2]) ? (int)x[1] : (int)x[2]);
	ymax = ((int)y[0] > (int)y[1]) ? (((int)y[0] > (int)y[2]) ? (int)y[0] : (int)y[2]) : (((int)y[1] > (int)y[2]) ? (int)y[1] : (int)y[2]);

	// rastarize the triangle
	float alpha, beta, gamma;
	GzColor colors[3];

	for (int i = ymin; i <= ymax; i++) {
		for (int j = xmin; j <= xmax; j++) {

			// calculate barycentric coordinates for the pixel (i,j)
			alpha = (A[0] * (float)j + B[0] * (float)i + C[0]) / (A[0] * x[2] + B[0] * y[2] + C[0]);
			beta = (A[1] * (float)j + B[1] * (float)i + C[1]) / (A[1] * x[0] + B[1] * y[0] + C[1]);
			gamma = 1 - alpha - beta;

			// decide whether to draw the pixel
			if ((alpha > 0) && (alpha < 1) && (beta > 0) && (beta < 1) && (gamma > 0) && (gamma < 1)) {

				// Z-interpolation using barycentric coordinates
				float zDepth = alpha * z[2] + beta * z[0] + gamma * z[1];

				if ((int)zDepth < 0)
					continue;

				// obtain the depth in the Z-direction
				GzIntensity rVal, gVal, bVal, aVal;
				GzDepth zVal;
				GzGetDisplay(render->display, j, i, &rVal, &gVal, &bVal, &aVal, &zVal);

				// Z-buffer: render the closest value, discard others
				if (zDepth < zVal) {
					float red, green, blue;
					if (render->tex_fun != 0) {
						//Determine Color
						GzColor K;
						GzGetTexture(K, render, UV, z, alpha, beta, gamma);
						render->Ka[0] = K[0];	render->Ka[1] = K[1];	render->Ka[2] = K[2];
						render->Kd[0] = K[0];	render->Kd[1] = K[1];	render->Kd[2] = K[2];

						if (render->interp_mode != GZ_NORMALS)
							render->Ks[0] = K[0];	render->Ks[1] = K[1];	render->Ks[2] = K[2];
					}
					switch (render->interp_mode)
					{
					case GZ_COLOR:
						GzShade(colors[0], render, N1);
						GzShade(colors[1], render, N2);
						GzShade(colors[2], render, N3);

						red = alpha * colors[2][0] + beta * colors[0][0] + gamma * colors[1][0];
						green = alpha * colors[2][1] + beta * colors[0][1] + gamma * colors[1][1];
						blue = alpha * colors[2][2] + beta * colors[0][2] + gamma * colors[1][2];

						red = (GzIntensity)(red * 4095.0);
						green = (GzIntensity)(green * 4095.0);
						blue = (GzIntensity)(blue * 4095.0);

						GzPutDisplay(render->display, j, i, red, green, blue, 1, zDepth);
						break;
					case GZ_NORMALS:
						GzCoord N;
						float norm;

						N[0] = alpha * N3[0] + beta * N1[0] + gamma * N2[0];
						N[1] = alpha * N3[1] + beta * N1[1] + gamma * N2[1];
						N[2] = alpha * N3[2] + beta * N1[2] + gamma * N2[2];
						norm = sqrt(pow(N[0], 2) + pow(N[1], 2) + pow(N[2], 2));
						N[0] = N[0] / norm;	N[1] = N[1] / norm;	N[2] = N[2] / norm;

						GzShade(colors[0], render, N);

						red		= (GzIntensity)(colors[0][0] * 4095.0);
						green	= (GzIntensity)(colors[0][1] * 4095.0);
						blue	= (GzIntensity)(colors[0][2] * 4095.0);

						GzPutDisplay(render->display, j, i, red, green, blue, 1, zDepth);
						break;
					default:
						GzShade(colors[0], render, N1);

						red = (GzIntensity)(colors[0][0] * 4095.0);
						green = (GzIntensity)(colors[0][1] * 4095.0);
						blue = (GzIntensity)(colors[0][2] * 4095.0);

						GzPutDisplay(render->display, j, i, red, green, blue, 1, zDepth);
						break;
					}
				}
			}
		}
	}

	return GZ_SUCCESS;
}


int GzShade(GzColor color, GzRender* render, GzCoord N) {
	GzCoord E, R, tempN;
	float NDotL, NDotE, RDotE;

	color[0] = 0.0;	color[1] = 0.0;	color[2] = 0.0;
	E[0]	 = 0.0;	E[1]	 = 0.0;	E[2]	 = -1.0;
	NDotE = GzDotProduct(N, E);

	for (int ind = 0; ind < render->numlights; ind++) {
		for (int i = 0; i < 3; i++)
			tempN[i] = N[i];

		NDotL = GzDotProduct(tempN, render->lights[ind].direction);
		if ((NDotL < 0) && (NDotE < 0)) {
			tempN[0] = (-1.0) * tempN[0];	tempN[1] = (-1.0) * tempN[1];	tempN[2] = (-1.0) * tempN[2];
			NDotL = GzDotProduct(tempN, render->lights[ind].direction);
		}
		else if ((NDotL < 0) || (NDotE < 0))
			continue;
		
		for (int comp = 0; comp < 3; comp++)
			R[comp] = 2.0 * NDotL * tempN[comp] - render->lights[ind].direction[comp];

		RDotE = GzDotProduct(R, E);
		if (RDotE < 0.0)	RDotE = 0.0;
		if (RDotE > 1.0)	RDotE = 1.0;
		
		for (int comp = 0; comp < 3; comp++) {
			color[comp] = color[comp] + (render->Kd[comp] * NDotL +
				render->Ks[comp] * pow(RDotE, render->spec)) * render->lights[ind].color[comp];
		}
	}

	for (int comp = 0; comp < 3; comp++) {
		color[comp] = color[comp] + render->Ka[comp] * render->ambientlight.color[comp];
		if (color[comp] > 1.0)
			color[comp] = 1.0;
		if (color[comp] < 0.0)
			color[comp] = 0.0;
	}

	return GZ_SUCCESS;
}


int GzGetTexture(GzColor K, GzRender* render, GzTextureIndex vertUV[], GzCoord z, float alpha, float beta, float gamma) {
	GzTextureIndex UVwarp[3], UVpersp, UV;

	GzCoord VzPersp;
	float VzInterp;
	VzPersp[0] = z[0] / ((float)INT_MAX - z[0]);
	VzPersp[1] = z[1] / ((float)INT_MAX - z[1]);
	VzPersp[2] = z[2] / ((float)INT_MAX - z[2]);

	UVwarp[0][U] = vertUV[0][U] / (VzPersp[0] + 1);	UVwarp[1][U] = vertUV[1][U] / (VzPersp[1] + 1);	UVwarp[2][U] = vertUV[2][U] / (VzPersp[2] + 1);
	UVwarp[0][V] = vertUV[0][V] / (VzPersp[0] + 1);	UVwarp[1][V] = vertUV[1][V] / (VzPersp[1] + 1);	UVwarp[2][V] = vertUV[2][V] / (VzPersp[2] + 1);

	UVpersp[U] = alpha * UVwarp[2][U] + beta * UVwarp[0][U] + gamma * UVwarp[1][U];
	UVpersp[V] = alpha * UVwarp[2][V] + beta * UVwarp[0][V] + gamma * UVwarp[1][V];

	float zDepth = alpha * z[2] + beta * z[0] + gamma * z[1];
	VzInterp = zDepth / ((float)INT_MAX - zDepth);

	UV[U] = UVpersp[U] * (VzInterp + 1);	UV[V] = UVpersp[V] * (VzInterp + 1);

	render->tex_fun(UV[U], UV[V], K);

	return GZ_SUCCESS;
}


/* NOT part of API - just for general assistance */

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}

