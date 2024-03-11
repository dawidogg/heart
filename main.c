#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <math.h>
#include <sys/time.h>
#include "point.h"
#include "heart.h"
#include "config.h"

void get_timestamp(char *buff) {
	struct timeval t;
	gettimeofday(&t, 0);
	sprintf(buff, "%ld", (t.tv_sec*1000000 + t.tv_usec));
}

int main(int argc, char **vargs) {
	int fd = open(touchpad_path, O_RDONLY);
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
				double result[6];
				analyze_shape(&heart_shape[0], &heart_shape[1], result);
				if (skip_writing) goto SkipWriting;

				char timestamp[32];
				char filename[64];
				get_timestamp(timestamp);
				sprintf(filename, "./%s/%s_1.txt", output_dir, timestamp);
				FILE *finger1 = fopen(filename, "w");
				sprintf(filename, "./%s/%s_2.txt", output_dir, timestamp);
				FILE *finger2 = fopen(filename, "w");
				sprintf(filename, "./%s/%s_result.txt", output_dir, timestamp);
				FILE *result_stream = fopen(filename, "w");

				for (int i = 1; i <= arr_size_p(heart_shape[0]); i++) {
					fprintf(finger1, "%lf %lf\n", heart_shape[0][i].x, heart_shape[0][i].y);
				}
				for (int i = 1; i <= arr_size_p(heart_shape[1]); i++) {
					fprintf(finger2, "%lf %lf\n", heart_shape[1][i].x, heart_shape[1][i].y);
				}
				for (int i = 0; i < 6; i++) {
					fprintf(result_stream, "%lf ", result[i]);
				}
				fclose(finger1);
				fclose(finger2);
				fclose(result_stream);
				printf("\nSaved as %s\n\n", timestamp);

				char plot_command[64];
				sprintf(plot_command, "./p ./%s/%s >nul 2>nul &", output_dir, timestamp);
				system(plot_command);
			SkipWriting:
				printed = 1;
				ignored_count[0] = 0;
				ignored_count[1] = 0;
				arr_shrink_p(&heart_shape[0], 0);
				arr_shrink_p(&heart_shape[1], 0);

				for (int i = 0; i < 6; i++) {
					printf("%lf ", result[i]);
				}
				printf("\n");
				if ((result[0] > 2*result[1] && result[3] > 2*result[4]) &&
					result[2] <= 2.5 && result[5] <= 2.5) {
					printf("Heart!\n");
					system(on_heart_detect_command);
				} else {
					printf("Not heart.\n");
				}

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
			if (ignored_count[current_slot] > ignore_first)
				arr_push_p(&(heart_shape[current_slot]), current_point[current_slot]);
		}		

    }

    close(fd);
    return 0;
}
