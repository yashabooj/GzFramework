/*	YASH ABOOJ
CSCI 580: 3-D Graphics and Rendering
Homework 6: Aniti-Aliasing using Jittered Sub-Sampling
abooj@usc.edu */

/* CS580 Homework 6 */

#include "disp.h" /* include your own disp.h file (e.g. hw1)*/
#include <vector>

/* Camera defaults */
#define	DEFAULT_FOV		35.0
#define	DEFAULT_IM_Z	(-10.0)  /* world coords for image plane origin */
#define	DEFAULT_IM_Y	(5.0)    /* default look-at point = 0,0,0 */
#define	DEFAULT_IM_X	(-10.0)

#define	DEFAULT_AMBIENT	{0.1, 0.1, 0.1}
#define	DEFAULT_DIFFUSE	{0.7, 0.6, 0.5}
#define	DEFAULT_SPECULAR	{0.2, 0.3, 0.4}
#define	DEFAULT_SPEC		32

#define	MATLEVELS	100		/* how many matrix pushes allowed */
#define	MAX_LIGHTS	10		/* how many lights allowed */

///* Dummy definition : change it later */
//#ifndef GzLight
//#define GzLight		GzPointer
//#endif

//#ifndef GzTexture
//#define GzTexture	GzPointer
//#endif

#ifndef GZRENDER
#define GZRENDER
typedef struct {			/* define a renderer */
  GzRenderClass	renderClass;
  GzDisplay		*display;
  short		    open;
  GzCamera		camera;
  short		    matlevel;	        /* top of stack - current xform */
  GzMatrix		Ximage[MATLEVELS];	/* stack of xforms (Xsm) */
  GzMatrix		Xnorm[MATLEVELS];	/* xforms for norms (Xim) */
  GzMatrix		Xsp;		        /* NDC to screen (pers-to-screen) */
  GzColor		flatcolor;          /* color state for flat shaded triangles */
  int			interp_mode;
  int			numlights;
  GzLight		lights[MAX_LIGHTS];
  GzLight		ambientlight;
  GzColor		Ka, Kd, Ks;
  float		    spec;		/* specular power */
  GzTexture		tex_fun;    /* tex_fun(float u, float v, GzColor color) */
  float			xOffset;
  float			yOffset;
}  GzRender;
#endif

// Function declaration
// HW2
int GzNewRender(GzRender **render, GzRenderClass renderClass, GzDisplay *display);
int GzFreeRender(GzRender *render);
int GzBeginRender(GzRender	*render);
int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer *valueList);
int GzPutTriangle(GzRender *render, int	numParts, GzToken *nameList,
	GzPointer *valueList);

// HW3
int GzPutCamera(GzRender *render, GzCamera *camera);
int GzPushMatrix(GzRender *render, GzMatrix	matrix);
int GzPopMatrix(GzRender *render);

// Object Translation
int GzRotXMat(float degree, GzMatrix mat);
int GzRotYMat(float degree, GzMatrix mat);
int GzRotZMat(float degree, GzMatrix mat);
int GzTrxMat(GzCoord translate, GzMatrix mat);
int GzScaleMat(GzCoord scale, GzMatrix mat);

int GzIdentityMatrix(GzMatrix mat);
int GzMatrixMultiply(GzMatrix product, GzMatrix A, GzMatrix B);
float GzDotProduct(GzCoord vectorX, GzCoord vectorY);
int GzTransformCoordinates(GzCoord xformCoord, GzCoord origCoord, GzMatrix XformMat);
int GzTransformNormals(GzCoord xformNorm, GzCoord origNorm, GzMatrix XformMat);
int GzShade(GzColor color, GzRender* render, GzCoord N);
int GzGetTexture(GzColor K, GzRender* render, GzTextureIndex UV[], GzCoord z, float alpha, float beta, float gamma);
int GzSetPlane(GzPlane* p, GzCoord V1, GzCoord V2, GzCoord V3, GzCoord N1, GzCoord N2, GzCoord N3);
int GzSetPlane(GzPlane* p, GzTriangle tri);
float GzNorm(GzCoord vector);
int GzNormalize(GzCoord vector);
int GzCrossProduct(GzCoord ACrossB, GzCoord A, GzCoord B);
int GzVector(GzCoord vectAtoB, GzCoord A, GzCoord B);
int GzRayTriangleIntersection(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzCoord V1, GzCoord V2, GzCoord V3);
int GzRayTriangleIntersection(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzTriangle tri);
int GzPointWithinTriangle(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzCoord V1, GzCoord V2, GzCoord V3);
int GzPointWithinTriangle(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzTriangle tri);
int GzPointAbovePlane(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzCoord V1, GzCoord V2, GzCoord V3);
int GzPointAbovePlane(GzCoord PointOfIntersection, GzPlane plane, GzRay ray, GzTriangle tri);
int GzSetTriangle(GzTriangle* tri, GzCoord V1, GzCoord V2, GzCoord V3, GzCoord N1, GzCoord N2, GzCoord N3);
int GzRayTrace(GzRender* render, std::vector<GzTriangle*> triangleList);
int GzRayOffsetFromCamera(int width, int height, float aspect_ratio, int x, int y, float * x_amnt, float * y_amnt);
int GzSetRay(GzRay* ray, GzCoord origin, GzCoord direction);
int GzScalarMultiply(GzCoord scaledVector, GzCoord vector, float scale);
int GzVectorAdd(GzCoord vectAplusB, GzCoord A, GzCoord B);
float GzEuclideanDistance(GzCoord coordA, GzCoord coordB);
int GzGetBarycentricCoordinates(float *alpha, float * beta, float *gamma, float xCoord, float yCoord, GzTriangle tri);
int GzInterpolateNormals(GzCoord normal, float alpha, float beta, float gamma, GzTriangle tri);
int GzShadeWithShadow(GzColor color, GzRender* render, GzCoord N, GzCoord point, std::vector<GzTriangle*> triangleList, int triIndex);