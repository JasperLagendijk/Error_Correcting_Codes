#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define NWITEMS 512
const char *source =
"kernel void memset( global uint *dst )             \n"
"{                                                    \n"
"    dst[get_global_id(0)] = get_global_id(0);        \n"
"}                                                    \n";



int main(int argc, char** argv)
{
	//1. Get a platform (i.e., intel, nvidia amd etc)
	cl_platform_id platform;
	cl_uint num_platforms; //Syntax: clGetPlatformIDs(num_entries to add to platform, pointer to platform object, pointer to number of opencl platforms available)
	cl_int error = clGetPlatformIDs(1, &platform, &num_platforms);;
	if(error != CL_SUCCESS)
	{
		printf("Error with getting platform: %d\n",  error);
		return -1;
	}
	printf("Step 1. Number of platforms: %d\n", num_platforms);

	
	
	//2. Find a gpu device
	cl_device_id device;
	cl_uint num_devices;
	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices); //Syntax: clGetDeviceIDs(platform, device_type, take from constant, )
	
	if(error != CL_SUCCESS)
	{
		printf("Error with getting device: %d\n",  num_devices);
		switch(error)
		{
			case CL_INVALID_PLATFORM:
				printf("Error with getting platform\n");
				break;
			case CL_INVALID_DEVICE_TYPE:
				printf("Error with device type\n");
				break;
			case CL_INVALID_VALUE:
				printf("Error invalid value\n");
				break;
			case CL_DEVICE_NOT_FOUND:
				printf("Error device not found\n");
				break;
			case CL_OUT_OF_RESOURCES:
				printf("Error out of resources\n");
				break;
			case CL_OUT_OF_HOST_MEMORY:
				printf("Error out of host memory\n");
				break;
		}
		return -1;
	}
	
	printf("Step 2.\n");
	
	//3. Create a context and command queue on that device
	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL); //Syntax: clCreateContext(properties, num_devices, devices, pfn_notify, user_data, errcode_ret)
	cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, NULL, NULL); //Syntax: clCreateCommandQueue(context, device, properties, errcode_ret)
	printf("Step 3.\n");
	
	//4. Perform runtime source compilation, and obtain kernel entry point.
	cl_program program = clCreateProgramWithSource(context, 1, &source, NULL, NULL); //Creates the program object and load the program for a given source
	clBuildProgram(program, 1, &device, NULL, NULL, NULL); //Compile the program for a specific device and builds it in program?
	cl_kernel kernel = clCreateKernel(program, "memset", NULL); //Creates the kernel object of a specific program, give it a name
	printf("Step 4.\n");	
	
	//5. Create a data buffer.
	cl_mem buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, NWITEMS * sizeof(cl_uint), NULL, NULL); 
	printf("Step 5.\n");	
	
	
	//6. Launch the kernel. Let OpenCL pick the local work size.
	size_t global_work_size = NWITEMS;
	clSetKernelArg(kernel, 0, sizeof(buffer), (void*) &buffer); //Set argument for the kernel. IE put variable(s) into kernel 
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL); //Enqueues command to execute a kernel for a given device.
	clFinish( queue ); //Blocks until all previous OpenCL commands are issued and have completed
	printf("Step 6.\n");	
	
	//7. Look at the results via synchronous buffer map
	cl_uint *ptr;
	ptr = (cl_uint *) clEnqueueMapBuffer(queue, buffer, CL_TRUE, CL_MAP_READ, 0, NWITEMS * sizeof(cl_uint), 0, NULL, NULL, NULL);
	printf("Step 7.\n");	
	
	
	for(int i =0; i < NWITEMS; i++) printf("%d %d\n", i, ptr[i]);
	
	
	return 0;
}

