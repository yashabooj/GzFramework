#include "Gz.h"
#include "rend.h"
#include "disp.h"

void rayOffsetFromCamera(int width, int height, int aspect_ratio, int x, int y, float * x_amnt, float * y_amnt) {
	
	if (width > height) {
		*x_amnt = ((x + 0.5) / width) * aspect_ratio - (((width-height)/(float)height)/2);
		*y_amnt = ((height - y) + 0.5) / height;
	}
	else if (width < height) {
		*x_amnt = (x + 0.5) / width;
		*y_amnt = (((height - y) + 0.5) / height) / aspect_ratio - (((height - width) / (float)width ) / 2);
	}
	else {
		*x_amnt = (x + 0.5) / width;
		*y_amnt = ((height - y) + 0.5) / height;
	}

}


int renderTriangle(GzRender* render) {
	int	width = 256;
	int height = 256;
	float aspect_ratio = (float) (width / height);
	
	float x_amnt, y_amnt;
	GzCoord ray_origin, ray_direction;			/* camera ray */

	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			
			rayOffsetFromCamera(width, height, aspect_ratio, x, y, &x_amnt, &y_amnt);
			
			/* TODO : Find ray origin and ray direction vectors
					: Ray Origin is same as camera position
					: Ray Direction needs x_amnt, y_amnt calculated above
			*/

			for(int i = 0; i < numTrianglesInMesh; i++) {
				/* TODO : Find intersection of ray with all triangles in mesh, if any.
						: Valid intersections return distance of intersecting point from camera.
						: Push (into an array) all valid intersections
				*/
			}

			
			/* TODO : Find the closest intersecting triangle
					: Function returns index of the triangle intersecting
					: Based on the triangle chosen, color for the pixel is chosen.
			*/
			int visibleTriangle = closestIntersectingTriangle();


		}
	}

	return GZ_SUCCESS;
}