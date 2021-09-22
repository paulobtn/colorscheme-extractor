#include <kmeans/kmeans.h>

double kmeans_dist2(Kpoint *a, Kpoint *b) {
    
    if(a->dim != b->dim){
        fprintf(stderr, "points must have the same dimension\n");
        exit(EXIT_FAILURE);
    }
    double sqrsum = 0;
    for(int i = 0 ; i < a->dim ; i++){
        sqrsum += pow(a->val[i] - b->val[i], 2);
    }
    return sqrsum;
}

// return the closest distance from a point to a cluster's centroid
double kmeans_nearest_distance(Kpoint *point, Kpoint *centroids, unsigned int num_clusters)
{
	double distance = 0;
    double min_distance = 0;
    
	min_distance = HUGE_VAL;
	for (int i = 0; i < num_clusters; i++) {
		distance = kmeans_dist2(&centroids[i], point);
		if ( distance < min_distance ) {
			min_distance = distance;
		}
	}
	return min_distance;
}

unsigned int kmeans_nearest_centroid(Kpoint *point, Kpoint *centroids, unsigned int num_clusters)
{
	double distance;
    double min_distance;
	int cluster_index;
 
	min_distance = HUGE_VAL;
	cluster_index = point->cluster;	
	for (int i = 0; i < num_clusters; i++) {
		distance = kmeans_dist2(&centroids[i], point);
		if ( distance < min_distance ) {
			min_distance = distance;
			cluster_index = i;
		}
	}	
	return cluster_index;
}

void kmeans_initialize_kpp(Kpoint *points, int num_points, Kpoint *centroids, int num_clusters) {
    
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
			distances[i] = kmeans_nearest_distance(&points[i], centroids, cluster_index);
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
    for (int i = 0; i < num_points; i++){
        points[i].cluster = kmeans_nearest_centroid(&points[i], centroids, num_clusters);
    }
 
	free(distances);
 
	return;
}

void kmeans_apply(Kpoint *points, int num_points, Kpoint *centroids, int num_clusters) {
    
    unsigned int dim = 3;
    unsigned int changes = 0;
    unsigned int max_iterations = 100;
    //number of points for each cluster
    unsigned int *cluster_size = NULL;
    unsigned int cluster_index = 0;

    cluster_size = malloc(num_clusters*sizeof(unsigned int));

    
    do{
        memset(cluster_size, 0, dim*sizeof(unsigned int));

        // assign all the points to the closest cluster centroid
        changes = 0;
        for (int i = 0; i < num_points; i++){
            cluster_index = kmeans_nearest_centroid(&points[i], centroids, num_clusters);
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


