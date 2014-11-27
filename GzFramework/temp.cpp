int GzGetBarycentricCoords(GzCoord V1, GzCoord V2, GzCoord V3) {

}

int GzBarycentricInterpolation() {

}

int closestTriangleIndex(std::vector<float> reflection_intersections) {

}

int GzGetColor() {

}

int GzAddColors(GzColor resultant, GzColor colorA, GzColor colorB) {
	
	resultant[R] = colorA[R] + colorB[R]; 
	resultant[G] = colorA[G] + colorB[G];
	resultant[B] = colorA[B] + colorB[B];
	
	return GZ_SUCCESS;
}

int GzAdjustColorBrightness(GzColor scaledColor, GzColor color, float scale) {
	
	scaledColor[R] = scale * color[R];
	scaledColor[G] = scale * color[G];
	scaledColor[B] = scale * color[B];
	
	return GZ_SUCCESS;
}

int addReflections(GzTriangle tri, GzCoord incomingRayDirection, GzCoord pointOfIntersection, GzColor colorFromPrimaryIntersection) {
	GzCoord barycentricCoords;
	GzGetBarycentricCoords();
	
	
	/* Step 1: Generate the reflection ray */
	GzCoord N1 = tri.vertNormal[0];
	GzCoord N2 = tri.vertNormal[1];
	GzCoord N3 = tri.vertNormal[2];	
	
	GzCoord normalAtPointOfIntersection;
	GzBarycentricInterpolation(normalAtPointOfIntersection, N1, N2, N3, barycentricCoords);
	
	GzCoord invertedRayDirection;
	GzScalarMultiply(invertedRayDirection, incomingRayDirection, -1.0);
	float dot_normal_ray = GzDotProduct(normalAtPointOfIntersection, invertedRayDirection);
	
	GzCoord reflectedRayDirection;
	GzCoord tempVector1, tempVector2, tempVector3;
	GzScalarMultiply(tempVector1, normalAtPointOfIntersection, dot_normal_ray);
	GzVectorAdd(tempVector2, incomingRayDirection, tempVector1);
	GzScalarMultiply(tempVector3, tempVector2, 2.0);
	GzVectorAdd(reflectedRayDirection, invertedRayDirection, tempVector3);
	GzNormalize(reflectedRayDirection);
	
	
	/* Step 2 : Find out all possible intersections of the reflected ray */
	std::vector<float>	reflection_intersections;
	std::vector<int>	triangleIndex;
	
	for (int index = 0; index < triangleList.size(); index++) {
		GzPlane plane;
		GzSetPlane(&plane, *(triangleList.at(index)));
		GzCoord point;
		int check = GzRayTriangleIntersection(point, plane, ray, *(triangleList.at(index)));

		if (check == GZ_FAILURE)
			continue;
		else {
			//intersections.push_back(point);
			reflection_intersections.push_back(GzEuclideanDistance(point, render->camera.position));
			triangleIndex.push_back(index);
		}
	}
	
	
	/* Step 3 : Find the closest intersecting triangle */
	int closest_triangle_index = closestTriangleIndex(reflection_intersections);
	
	/* Step 4 : Fetch color of the intersecting point */
	if(closest_triangle_index != -1) {
		if(reflection_intersections.at(closest_triangle_index) > accuracy) {
			
			GzCoord reflectionIntersectionPoint;	
			float distance_to_intersecting_object = reflection_intersections.at(closest_triangle_index);
			GzCoord scaledReflectedRayDirection;
			GzScalarMultiply(scaledReflectedRayDirection, reflectedRayDirection, distance_to_intersecting_object);
			GzVectorAdd(reflectionIntersectionPoint, pointOfIntersection, scaledReflectedRayDirection);
			
			GzColor colorFromReflection;
			GzGetColor(colorFromReflection, render, reflectionIntersectionPoint, reflectedRayDirection, triangleList, closest_triangle_index); 
			
			float scaling_factor;
			GzAdjustColorBrightness(colorFromReflection, colorFromReflection, scaling_factor);
			GzAddColors(colorFromPrimaryIntersection, colorFromPrimaryIntersection, colorFromReflection);
		}
	}
	
	
	
	return GZ_SUCCESS;
}