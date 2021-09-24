#ifndef KMEANS_H
#define KMEANS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define KMEANS_OK     0
#define KMEANS_ERROR -1

/* a data structure that indicates a point that will be used for the
 * kmeans procedure.
 * val      - the coordinates of the point
 * dim      - the dimension of the point 
 * cluster  - the cluster associated to this point 
 */ 
typedef struct {
    // double val[3];
    double *val;
    unsigned int dim;
    unsigned int cluster;
} Kpoint;

/* A data structure that aggregates points and clusters.
 * It is used to facilitate data allocation and function calls
 */
typedef struct {
    unsigned int dim;
    unsigned int num_clusters;
    unsigned int num_points; 
    Kpoint* points;
    Kpoint* centroids;
} KMEANS_T;


/* Initialization methods for Kmeans
 * KMEANS_RANDOM will choose the centroids randomly from the provided points
 * KMEANS_KPP will try to choose centroids that are far from each other
 * KMEANS_CUSTOM will do nothing and the user will have to populate the centroids
 *
 * the recommended one is KMEANS_KPP. KMEANS_RANDOM will not always converge to
 * good results.
 */
typedef enum {
    KMEANS_RANDOM,
    KMEANS_KPP,
    KMEANS_CUSTOM
} KMEANS_INIT_T;

// creates point from an array of double
Kpoint* kmeans_create_point(double* val, size_t size);

// destroy an allocated point
// void kmeans_destroy_point(Kpoint* pt);

/* Creates an object of type KMEANS_T which stores the necessary data to apply kmeans.
 * The parameters are the number of points num_points, the number of clusters num_clusters
 * and the dimension of the points dim.*/
// KMEANS_T* kmeans_alloc(unsigned int num_points,
                       // unsigned int num_clusters,
                       // unsigned int dim);
KMEANS_T* kmeans_alloc(double ** points,
                       unsigned int num_points,
                       unsigned int dim,
                       unsigned int num_clusters);

/* destroys the data allocated by kmeans_alloc */
void kmeans_destroy(KMEANS_T* kp);

unsigned int kmeans_insert_point(KMEANS_T* kp,
                                 unsigned int index,
                                 Kpoint* point
                                 );

/* given two points with arbitrary dimension, that must be specified
 * in the dim argument inside the point, return the squared distance.
 * The points a and b must be of the same dimension.
 */
static double kmeans_dist2(Kpoint *a, Kpoint *b, unsigned int dim);

/* given a list of points and a list of centroids, return the closest
 * distance from an arbitrary point to the cluster's centroid.
 */
static double kmeans_nearest_distance(Kpoint *point,
                                      Kpoint *centroids,
                                      unsigned int num_clusters,
                                      unsigned int dim);

/* given a list of points and a list of centroids, return the index to the
nearest centroid */
static unsigned int kmeans_nearest_centroid(Kpoint *point,
                                            Kpoint *centroids,
                                            unsigned int num_clusters,
                                            unsigned int dim);

/* the kmeans++ initialization algorithm. The kmeans algorithm will have better performance
if we provide a good estimative for the initial clusters. This initialization method gives
a high probability that the centroids will be far away from each other. */
static void kmeans_init_kpp(Kpoint *points,
                            unsigned int num_points,
                            Kpoint *centroids,
                            unsigned int num_clusters,
                            unsigned int dim);

/* initialize the centroids with random points
simple but not recommended because it doesn't always converge to good results */
static void kmeans_init_random(Kpoint *points,
                               unsigned int num_points,
                               Kpoint *centroids,
                               unsigned int num_clusters,
                               unsigned int dim
                               );

/* initialize centroids with a custom method. */
static int kmeans_init(KMEANS_T *kp, KMEANS_INIT_T method);

/* Apply the kmeans clustering */
void kmeans_apply(KMEANS_T *kp, KMEANS_INIT_T init_method, unsigned int max_iterations );

#endif /* KMEANS_H */
