#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
a data structure that indicates a point that will be used for the
kmeans procedure.
    val      - the coordinates of the point
    dim      - the dimension of the point 
    cluster  - the cluster associated to this point 
*/ 
typedef struct {
    double val[3];
    unsigned int dim;
    unsigned int cluster;
} Kpoint;

/*
given two points with arbitrary dimension, that must be specified
in the dim argument inside the point, return the squared distance.
The points a and b must be of the same dimension.
*/
double kmeans_dist2(Kpoint *a, Kpoint *b);

/*
given a list of points and a list of centroids, return the closest
distance from an arbitrary point to the cluster's centroid.
*/
double kmeans_nearest_distance(Kpoint *point, Kpoint *centroids, unsigned int num_clusters);

/* given a list of points and a list of centroids, return the index to the
nearest centroid */
unsigned int kmeans_nearest_centroid(Kpoint *point, Kpoint *centroids, unsigned int num_clusters);

/* the kmeans++ initialization algorithm. The kmeans algorithm will have better performance
if provide a good estimative for the initial clusters. This initialization method gives
a high probability that the centroids will be far away from each other. */
void kmeans_initialize_kpp(Kpoint *points, int num_points, Kpoint *centroids, int num_clusters);

#endif /* KMEANS_H */
