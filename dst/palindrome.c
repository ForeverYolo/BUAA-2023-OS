#include <stdio.h>
int main() {
	int n;
	scanf("%d", &n);
	int x = n;
	int new = 0;
	do {
		new = new*10 + x%10;
		x = x/10;
	} while(x!=0);
	if (new == n) {
		printf("Y\n");
	} else {
		printf("N\n");
	}
	return 0;
}
