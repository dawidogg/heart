#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <math.h>
#include <sys/time.h>
#include "point.h"
#include "heart.h"

const int IGNORE_FIRST = 8;

void arr_test_p() {
	Point *bilge;
	arr_init_p(&bilge);
	for (Point i = {1, 12}; i.x <= 70; i.x++) {
		arr_push_p(&bilge, i);
	}
	arr_shrink_p(&bilge, 68);
	for (int i = 1; i <= bilge[0].x; i++)
		p_print("", *bilge);
	free(bilge);
}

void get_timestamp(char *buff) {
	struct timeval t;
	gettimeofday(&t, 0);
	sprintf(buff, "%ld", (t.tv_sec*1000000 + t.tv_usec));
}

int main() {
	const char *device = "/dev/input/event6"; // Replace with your event device
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open input device");
        return 1;
    }

    struct input_event ev;
	int two_fingers = 0;
	int *finger_arr[2];
	
	int current_slot = -1;
	Point *heart_shape[2];
	Point current_point[2];
	arr_init_p(&heart_shape[0]);
	arr_init_p(&heart_shape[1]);
	int ignored_count[2] = {0};
	
	int printed = 1;
	/* arr_push_p(&heart_shape[0], (Point){0, 0}); */
	/* arr_push_p(&heart_shape[1], (Point){0, 0}); */
    while (1) {
        ssize_t bytes = read(fd, &ev, sizeof(struct input_event));
        if (bytes == -1) {
			perror("Failed to read input event");
            break;
        }

        if (bytes != sizeof(struct input_event)) continue;

		// ev.type, ev.code, ev.value
		if (ev.type == EV_KEY && ev.code == BTN_TOOL_DOUBLETAP)	{
			two_fingers = ev.value;
			// printf("Two fingers: %d\n", two_fingers);
		}

		if (!two_fingers) {
			if (!printed) {
				smooth_shape(&heart_shape[0]);
				smooth_shape(&heart_shape[1]);
				double result[4];
				analyze_shape(&heart_shape[0], &heart_shape[1], result);

				char timestamp[32];
				char filename[64];
				get_timestamp(timestamp);
				sprintf(filename, "./data/%s_1.txt", timestamp);
				FILE *finger1 = fopen(filename, "w");
				sprintf(filename, "./data/%s_2.txt", timestamp);
				FILE *finger2 = fopen(filename, "w");
				sprintf(filename, "./data/%s_result.txt", timestamp);
				FILE *result_stream = fopen(filename, "w");

				for (int i = 1; i <= arr_size_p(heart_shape[0]); i++) {
					fprintf(finger1, "%lf %lf\n", heart_shape[0][i].x, heart_shape[0][i].y);
				}
				for (int i = 1; i <= arr_size_p(heart_shape[1]); i++) {
					fprintf(finger2, "%lf %lf\n", heart_shape[1][i].x, heart_shape[1][i].y);
				}
				for (int i = 0; i < 4; i++) {
					fprintf(result_stream, "%lf ", result[i]);
					printf("%lf ", result[i]);
				}
				arr_shrink_p(&heart_shape[0], 0);
				arr_shrink_p(&heart_shape[1], 0);
				printed = 1;
				fclose(finger1);
				fclose(finger2);
				fclose(result_stream);
				printf("\nSaved as %s\n\n", timestamp);
			}
			continue;
		} else printed = 0;
		
		// Both fingers are being tracked
		if (ev.type == EV_ABS && ev.code == ABS_MT_SLOT) {
			current_slot = ev.value;
		}

		if (current_slot != 0 && current_slot != 1) continue;

		if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_X) {
			current_point[current_slot].x = ev.value;
		}
		if (ev.type == EV_ABS && ev.code == ABS_MT_POSITION_Y) {
			current_point[current_slot].y = ev.value;
			ignored_count[current_slot]++;
			if (ignored_count[current_slot] > IGNORE_FIRST)
				arr_push_p(&(heart_shape[current_slot]), current_point[current_slot]);
		}		

    }

    close(fd);
    return 0;
}
