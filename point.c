#include "point.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void arr_init_p(Point** arr) {
	*arr = (Point*) calloc(GET_SIZE(0), sizeof(Point));
	(*arr)[0].x = (*arr)[0].y = 0;
}

void arr_push_p(Point** arr_input, Point val) {
	Point *arr = *arr_input;
	arr[0].x = arr[0].x + 1;
	arr[0].y = arr[0].y + 1;
	if (GET_SIZE(arr[0].x) > GET_SIZE(arr[0].x-1)) {
		//		printf("Array expanded! Old size: %d, New size: %d\n", GET_SIZE(arr[0]-1), GET_SIZE(arr[0]));
		arr = realloc(arr, GET_SIZE((int)(arr[0].x))*sizeof(Point));
	}
	arr[(int)arr[0].x] = val;
	*arr_input = arr;
}

void arr_shrink_p(Point** arr_input, int size) {
	Point *arr = *arr_input;
	if (GET_SIZE((int)arr[0].x) > GET_SIZE(size))
		arr = realloc(arr, GET_SIZE(size)*sizeof(Point));
	arr[0].x = arr[0].y = size;
	*arr_input = arr;
}

Point p_add(Point a, Point b) {
	return (Point){a.x+b.x, a.y+b.y};
}

Point p_subtract(Point a, Point b) {
	return (Point){a.x-b.x, a.y-b.y};
}

double p_dot_product(Point a, Point b) {
	return (a.x*b.x + a.y*b.y);
}

double p_norm(Point a) {
	return sqrt(pow(a.x, 2)+pow(a.y, 2));
}

Point p_divide(Point a, double x) {
	return (Point){a.x/x, a.y/x};
}

void p_print(const char* str, Point a) {
	printf("%s (%lf, %lf)\n", str, a.x, a.y);
}
