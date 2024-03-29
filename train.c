#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

double learning_rate = 0.3;
double data[5000][5];
double weight[4];
int data_size = 0, data_wrong_size = 0;
int data_delimiter;

void read_data() {
	DIR *dir;
	struct dirent *entry;
	dir = opendir("./data/");
	if (dir == NULL) {
		perror("opendir");
		exit(-1);
	}
	while ((entry = readdir(dir))) {
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..") &&
			strstr(entry->d_name, "result") != NULL) {
			char fpath[64];
			sprintf(fpath, "./data/%s", entry->d_name);
			FILE *f = fopen(fpath, "r");
			for (int i = 0; i < 2; i++) {
				fscanf(f, "%lf%lf", &data[data_size][0], &data[data_size][1]); //, &data[data_size][2]);
				data[data_size][0] *= 100;
				data[data_size][1] *= 100;
				data[data_size][2] = 1;
				data[data_size][3] = 1;
				data_size++;
			}
			fclose(f);
		}
	}
	closedir(dir);
	data_delimiter = data_size;

	DIR *dir2;
	struct dirent *entry2;
	dir2 = opendir("./data_wrong/");

	if (dir2 == NULL) {
		perror("opendir");
		exit(-1);
	}
	while ((entry2 = readdir(dir2))) {
//		printf("%d %d \n", data_size, data_delimiter);
//		if (data_size-data_delimiter > 200) break;
		if (strcmp(entry2->d_name, ".") && strcmp(entry2->d_name, "..") &&
			strstr(entry2->d_name, "result") != NULL) {
			char fpath[64];
			sprintf(fpath, "./data_wrong/%s", entry2->d_name);
			FILE *f = fopen(fpath, "r");
			for (int i = 0; i < 2; i++) {
				fscanf(f, "%lf%lf", &data[data_size][0], &data[data_size][1]); // , &data[data_size][2]);
				data[data_size][0] *= 100;
				data[data_size][1] *= 100;
				data[data_size][2] = 1;
				data[data_size][3] = -1;
				data_size++;
			}
			fclose(f);
		}
	}
	closedir(dir);
	
	printf("Data size: %d\n", data_size);
}

void kakdela() {
	int heart_count = 0;
	for (int i = 0; i < data_delimiter; i++) {
		if ((weight[0]*data[i][0] + weight[1]*data[i][1] /* + weight[2]*data[i][2] */ + weight[2]*data[i][2]) > 0) {
			heart_count++;
		}
	}
	double correct_heart = (double)heart_count / (double)data_delimiter * 100;
	printf("Correct heart detection: %lf%%\n", correct_heart);

	heart_count = 0;
	for (int i = data_delimiter; i < data_size; i++) {
		if ((weight[0]*data[i][0] + weight[1]*data[i][1] /* + weight[2]*data[i][2]  */+ weight[2]*data[i][2]) <= 0) {
			heart_count++;
		}
	}
	double correct_not_heart = (double)heart_count / (double)(data_size - data_delimiter) * 100;
	printf("Correct not heart detection: %lf%%\n", correct_not_heart);
}

int main() {
	read_data();
	srand(time(NULL)); 
	weight[0] = (double)rand()/(double)RAND_MAX*2 - 1;
	weight[1] = (double)rand()/(double)RAND_MAX*2 - 1;
	weight[2] = (double)rand()/(double)RAND_MAX*2 - 1;
	weight[3] = (double)rand()/(double)RAND_MAX*2 - 1;
	for (int i = 0; i < 1000; i++) {
		int rand_index = (double)rand()/(double)RAND_MAX * (double)data_size;
		double *x = data[rand_index];
		double d = data[rand_index][3];
		if (isinf(x[0]) || isinf(x[1]) || isinf(x[2]) || isinf(x[3]) || isinf(x[4]) || isinf(x[5]))
			continue;
		weight[0] = weight[0] + learning_rate*x[0]*d;
		weight[1] = weight[1] + learning_rate*x[1]*d;
		weight[2] = weight[2] + learning_rate*x[2]*d;
//		weight[3] = weight[3] + learning_rate*x[3]*d;
	}
	kakdela();
	printf("[%lf %lf %lf]\n", weight[0], weight[1], weight[2]);

	return 0;
}
