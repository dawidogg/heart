#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "heart.h"
#include "point.h"

double get_concativity(Point* point, int start, int end) {
	double concativity_rate = 0;
	for (int i = start+2; i <= end; i++) {
		Point a = p_subtract(point[i-1], point[i-2]);
		Point b = p_subtract(point[i], point[i-1]);
		double x = p_dot_product(a, b) / p_norm(a) / p_norm(b);
		if (x > 1) x = 1;
		if (x < -1) x = -1;
		x = acos(x);
		if (x != x) x = 0;
		concativity_rate += p_angle_dir(a, b) * x;
//		printf("%f %d\n", x, angle_dir);
	}
	concativity_rate /= end-start-2;
	return concativity_rate;
}

void readData(const char* path, Point* point, int* psize) {
	int psize_ref = *psize;
	FILE *data_stream = fopen(path, "r");
	int temp;
	while (fscanf(data_stream, "%d", &temp) == 1) {
		if (psize_ref % 2)
			point[psize_ref/2].y = temp;
		else
			point[psize_ref/2].x = temp;
		psize_ref++;
	}
	psize_ref /= 2;
	fclose(data_stream);
	*psize = psize_ref;
}

void smooth_shape(Point **point_ref) {
	Point *point = *point_ref;
	// Average the angle for 3 consecutive points
	for (int i = 1; i <=  arr_size_p(point)-2; i++)
		point[i] = p_divide(p_add(p_add(point[i], point[i+1]), point[i+2]), 3);
	arr_shrink_p(&point, arr_size_p(point)-2);
	*point_ref = point;
}

void analyze_shape(Point **point1_ref, Point **point2_ref, double *result) {
	Point *point1 = *point1_ref, *point2 = *point2_ref;
	// Determine the left and right halves
	Point *p_left, *p_right;
	int p_left_size, p_right_size;
	Point vector1 = p_subtract(point1[arr_size_p(point1)], point1[0]);
	Point vector2 = p_subtract(point2[arr_size_p(point2)], point1[0]);
	if (p_angle_dir(vector1, vector2) < 0) {
		p_left = point1+1;
		p_left_size = arr_size_p(point1);
		p_right = point2+1;
		p_right_size = arr_size_p(point2);
		*point1_ref = point1;
		*point2_ref = point2;
	} else {
		p_left = point2+1;
		p_left_size = arr_size_p(point2);
		p_right = point1+1;
		p_right_size = arr_size_p(point1);
		*point1_ref = point2;
		*point2_ref = point1;
	}
	/* p_print("First point of left half:", p_left[0]); */
	/* p_print("First point of right half:", p_right[0]); */

	Point p_left_vector = p_subtract(p_left[p_left_size-1], p_left[0]);
	Point p_right_vector = p_subtract(p_right[p_right_size-1], p_right[0]);

	// Find rightmost and leftmost point index
	double A, B, C;
	Point AB;
	A = 1.0/p_right_vector.x;
	B = -1.0/p_right_vector.y;
	AB = (Point){A,B};
	C = p_right[0].x / p_right_vector.x - p_right[0].y / p_right_vector.y;
	int rightmost = 0;
	for (int i = 0; i < p_right_size; i++) {
		double current_dist = fabs(p_dot_product(AB, p_right[i]) - C) / p_norm(AB);
		double best_dist = fabs(p_dot_product(AB, p_right[rightmost]) - C) / p_norm(AB);
		if (current_dist > best_dist)
			rightmost = i;
	}
	A = 1.0/p_left_vector.x;
	B = -1.0/p_left_vector.y;
	AB = (Point){A,B};
	C = p_left[0].x / p_left_vector.x - p_left[0].y / p_left_vector.y;
	int leftmost = 0;
	for (int i = 0; i < p_left_size; i++) {
		double current_dist = fabs(p_dot_product(AB, p_left[i]) - C) / p_norm(AB);
		double best_dist = fabs(p_dot_product(AB, p_left[leftmost]) - C) / p_norm(AB);
		if (current_dist > best_dist)
			leftmost = i;
	}
	/* p_print("Rightmost point:", p_right[rightmost]); */
	/* p_print("Leftmost point:", p_left[leftmost]); */
	/* FILE *extremes = fopen("./point_data/rightmost.txt", "w"); */
	/* fprintf(extremes, "%d %d\n", (int)(p_right[rightmost].x), (int)(p_right[rightmost].y)); */
	/* fclose(extremes); */
	/* extremes = fopen("./point_data/leftmost.txt", "w"); */
	/* fprintf(extremes, "%d %d\n", (int)(p_left[leftmost].x), (int)(p_left[leftmost].y)); */
	/* fclose(extremes); */

	double curve_rate_right_upper = get_concativity(p_right, 0, rightmost);
	double curve_rate_right_lower = get_concativity(p_right, rightmost, p_right_size);
	double curve_rate_left_upper = -get_concativity(p_left, 0, leftmost);
	double curve_rate_left_lower = -get_concativity(p_left, leftmost, p_left_size);	
	/* printf("Right half's upper concativity rate: %lf\n", curve_rate_right_upper); */
	/* printf("Right half's lower concativity rate: %lf\n", curve_rate_right_lower); */
	/* printf("Left half's upper concativity rate: %lf\n", curve_rate_left_upper); */
	/* printf("Left half's lower concativity rate: %lf\n", curve_rate_left_lower); */
	
	if (curve_rate_right_upper > 0.01 && curve_rate_right_lower < 0.01 &&
		curve_rate_left_upper > 0.01 && curve_rate_left_lower < 0.01) {
		printf("Heart!\n");
	} else {
		printf("Not heart.\n");
	}
	result[0] = curve_rate_left_upper;
	result[1] =	curve_rate_left_lower;
	result[2] =	curve_rate_right_upper;
	result[3] =	curve_rate_right_lower;
}
