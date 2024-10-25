#include <stdio.h>
#include <openacc.h>
 
int main() {
	//printf("Supported OpenACC revision: %d.\n", _OPENACC);
	int device_type = acc_get_device_type();
	printf("Device Type is %d \n", device_type);
	int count = acc_get_num_devices(device_type);
	printf("Found %d GPUs.\n", count);
	int n = acc_get_device_num(device_type);
	printf("Default accelerator number is %d.\n", n);
	
	return 0;
}
