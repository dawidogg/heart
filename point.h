#ifndef POINT
#define POINT

#define GET_SIZE(a) (1 << (32-__builtin_clz(a)) + 1)
#define arr_size_p(a) ((int)(a)[0].x)

typedef struct {
	double x, y;
} Point;

void arr_init(double** arr);
void arr_init_p(Point** arr);
void arr_push_p(Point** arr_input, Point val);
void arr_shrink_p(Point** arr_input, int size);

Point p_add(Point a, Point b);
Point p_subtract(Point a, Point b);
double p_dot_product(Point a, Point b);
double p_norm(Point a);
Point p_divide(Point a, double x);
void p_print(const char* str, Point a);
#define p_angle_dir(a, b) (2*((a).x*(b).y - (a).y*(b).x > 0) - 1)

#endif
