#include <kmeans/kmeans.h>

KMEANS_T* kmeans_alloc(unsigned int num_points, unsigned int num_clusters, unsigned int dim){

    KMEANS_T *kp = NULL;
    
    kp = malloc(sizeof(KMEANS_T));

    kp->num_clusters = num_clusters;
    kp->num_points   = num_points;
    kp->dim          = dim;
    
    // allocate arrays for points and clusters
    kp->points = malloc(num_points * sizeof(Kpoint));
    kp->clusters = malloc(num_clusters * sizeof(Kpoint));
    
    // allocate the dimensions for each point and cluster
    for(int i = 0 ; i < num_points ; i++){
        kp->points[i].val = malloc(dim*sizeof(double));
        kp->points[i].cluster = 0;
    }

    for(int i = 0 ; i < num_clusters ; i++){
        kp->clusters[i].val = malloc(dim*sizeof(double));
        kp->points[i].cluster = i;
    }

    return kp;
}

void kmeans_destroy(KMEANS_T* kp){

   // free points values
   for(int i = 0; i < kp->num_points ; i++){
       free(kp->points[i].val);
   }

   // free points array
   free(kp->points);

   //free cluster values
   for(int i = 0; i < kp->num_clusters ; i++){
       free(kp->clusters[i].val);
   } 

   // free clusters array
   free(kp->clusters);

   free(kp);
}

static double kmeans_dist2(Kpoint *a, Kpoint *b, unsigned int dim) {
    
    double sqrsum = 0;
    for(int i = 0 ; i < dim ; i++){
        sqrsum += pow(a->val[i] - b->val[i], 2);
    }
    return sqrsum;
}

// return the closest distance from a point to a cluster's centroid
static double kmeans_nearest_distance(Kpoint *point,
                                      Kpoint *centroids,
                                      unsigned int num_clusters,
                                      unsigned int dim)
{
	double distance = 0;
    double min_distance = 0;
    
	min_distance = HUGE_VAL;
	for (int i = 0; i < num_clusters; i++) {
		distance = kmeans_dist2(&centroids[i], point, dim);
		if ( distance < min_distance ) {
			min_distance = distance;
		}
	}
	return min_distance;
}

static unsigned int kmeans_nearest_centroid(Kpoint *point,
                                            Kpoint *centroids,
                                            unsigned int num_clusters,
                                            unsigned int dim)
{
	double distance;
    double min_distance;
	int cluster_index;
 
	min_distance = HUGE_VAL;
	cluster_index = point->cluster;	
	for (int i = 0; i < num_clusters; i++) {
		distance = kmeans_dist2(&centroids[i], point, dim);
		if ( distance < min_distance ) {
			min_distance = distance;
			cluster_index = i;
		}
	}	
	return cluster_index;
}

static void kmeans_init_random( Kpoint *points,
                                unsigned int num_points,
                                Kpoint *centroids,
                                unsigned int num_clusters)
{

    for(int i = 0 ; i < num_clusters ; i++){
        centroids[i] = points[ rand() % num_points ];
    }
}

static void kmeans_init_kpp( Kpoint *points,
                             unsigned int num_points,
                             Kpoint *centroids,
                             unsigned int num_clusters,
                             unsigned int dim)
{
    
    // array that stores the shortest distance from a pointer to a cluster
	double *distances = NULL;
    // sum of all distances
	double sum = 0;
 
	distances = (double*) malloc(sizeof(double) * num_points);
    if(distances == NULL){
        fprintf(stderr, "Error in allocating memory\n");
        exit(EXIT_FAILURE);
    }

	// the fist cluster is picked at random
	centroids[0] = points[ rand() % num_points ];
 
	// select the other clusters
	for (int cluster_index = 1; cluster_index < num_clusters; cluster_index++) {
        
        // find the distance from each point to the nearest centroid
		sum = 0;
		for ( int i = 0; i < num_points; i++ ) {
			distances[i] = kmeans_nearest_distance(&points[i], centroids, cluster_index, dim);
			sum += distances[i];
		}
        
		// Pick a random value in the interval of the total distance.
        // Loop through all the distances until reaching this value.
        // Then, assign the centroid as the point associated to that.
        // The probability of some point being selected is directly
        // proportional to the squared distance of the nearest centroid.
		sum = sum * rand() / (RAND_MAX - 1);
		for (int i = 0; i < num_points; i++ ) {
			sum -= distances[i];
			if ( sum <= 0) {
				centroids[cluster_index] = points[i];
				break;
			}
		}
	}
 
	// for each point, assign the associated centroid
    /* for (int i = 0; i < num_points; i++){ */
        /* points[i].cluster = kmeans_nearest_centroid(&points[i], centroids, num_clusters); */
    /* } */
 
	free(distances);
 
	return;
}

static int kmeans_init( Kpoint *points,
                 unsigned int num_points,
                 Kpoint *centroids,
                 unsigned int num_clusters,
                 KMEANS_INIT_T method,
                 unsigned int dim)
{
    
    srand(time(NULL));
    switch (method){
        case KMEANS_RANDOM:
            kmeans_init_random(points, num_points, centroids, num_clusters);
            break;
        case KMEANS_KPP: 
            kmeans_init_kpp(points, num_points, centroids, num_clusters, dim);
            break;
        case KMEANS_CUSTOM:
            // the user has to initialize the centroids with a custom method
            break;
        default:
            fprintf(stderr, "Invalid kmeans initialization method\n");
            return KMEANS_ERROR;
    }

    return KMEANS_OK;
}

void kmeans_apply(
                  Kpoint *points,
                  unsigned int num_points,
                  Kpoint *centroids,
                  unsigned int num_clusters,
                  KMEANS_INIT_T init_method,
                  unsigned int dim
                  )
{
    
    unsigned int changes = 0;
    unsigned int max_iterations = 100;
    //number of points for each cluster
    unsigned int *cluster_size = NULL;
    unsigned int cluster_index = 0;

    cluster_size = malloc(num_clusters*sizeof(unsigned int));
    if(cluster_size == NULL){
        fprintf(stderr, "Error in allocating memory\n");
        exit(EXIT_FAILURE);
    }

    kmeans_init(points, num_points, centroids, num_clusters, init_method, dim);
    
    do{
        memset(cluster_size, 0, dim*sizeof(unsigned int));

        // assign all the points to the closest cluster centroid
        changes = 0;
        for (int i = 0; i < num_points; i++){
            cluster_index = kmeans_nearest_centroid(&points[i], centroids, num_clusters, dim);
            if(points[i].cluster != cluster_index){
                changes++;
            }
            points[i].cluster = cluster_index;
            cluster_size[points[i].cluster]++;
        }

        // zero the centroids
        for (int i = 0 ; i < num_clusters ; i++){
            memset(centroids[i].val, 0, dim*sizeof(double));
        }

        // recompute the centroids of newly formed clusters
        for (int i = 0 ; i < num_points ; i++){
            for(int j = 0 ; j < dim ; j++){
                centroids[points[i].cluster].val[j] += points[i].val[j];
            }
        }
        for (int i = 0 ; i < num_clusters ; i++){
            for(int j = 0 ; j < dim ; j++){
                centroids[i].val[j] /= cluster_size[i];
            }
        }

    } while(changes > 0 && max_iterations--);

    free(cluster_size);
    return;
}


