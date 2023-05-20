#include <lib.h>
#define NUM 256
int arr[NUM][NUM];
int brr[NUM][NUM];
int crr[NUM][NUM];



void generateA(int arr[NUM][NUM]) {
	for (int i = 0; i < NUM ; i++) {
		for (int j = 0; j < NUM ; j++) {
			arr[i][j] = arr[i][j] + i;
		}
	}
	return;
}


void generateB(int brr[NUM][NUM]) {
	for (int i = 0; i < NUM ; i++) {
		for (int j = 0; j < NUM ; j++) {
			brr[i][j] = brr[i][j] + j;
		}
	}
	return;
}


void calcute(int arr[NUM][NUM],int brr[NUM][NUM],int crr[NUM][NUM]) {
	for (int i = 0; i < NUM ; i++) {
		for (int j = 0; j < NUM ; j++) {
			for (int k = 0; k < NUM ; k++) {
				crr[i][j] += arr[i][k] * brr[k][j];
			}
			debugf("%05d ",crr[i][j]);
		}
		debugf("\n");
	}
	return;
}


int main() {
	debugf("hello world!\n");
	u_int enter_time, enter_us;
	enter_time = get_time(&enter_us);
	//debugf("%d:%d\n", s, us);
	generateA(arr);
	generateB(brr);
	calcute(arr,brr,crr);
	u_int now_time,now_us;
	now_time = get_time(&now_us);
	//debugf("%d:%d\n", s, us);
	int gap = ((int)now_time -(int)enter_time) * 1000000 + (int)now_us - (int)enter_us;
	gap = gap / 1000;
	debugf("======Test End======\n");
	debugf("Time Passed：%d ms\n",gap);
	debugf("======Test End======\n");
	return 0;
}
