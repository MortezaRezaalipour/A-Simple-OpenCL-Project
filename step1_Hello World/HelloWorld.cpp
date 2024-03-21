#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <CL\cl.h>


int main()
{
	//Page 60 of the book : The OpenCL Programming Book

	//--1st: Get a list of available platforms------------------------------------------------------------
	
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms = 0;

	clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	printf("Number of Platforms = %d \n", ret_num_platforms);
	printf("First Platform ID = %d \n", platform_id);
	printf("\n");
	//--------------------------------------------------------------------------------
	//--2nd: Select Device------------------------------------------------------------

	cl_device_id device_id = NULL;
	cl_uint ret_num_devices = 0;

	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
	printf("Number of Devices = %d \n", ret_num_devices);
	printf("First Device ID = %d \n", device_id);
	printf("\n");
	//--------------------------------------------------------------------------------
	//--3rd: Create Context------------------------------------------------------------------------------
	
	cl_context context = NULL;

	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, 0);
	//--------------------------------------------------------------------------------
	//--4th: Create Command Queue------------------------------------------------------------------------------

	// to calculate the execution time 
	cl_event event;
	cl_ulong start = 0;
	cl_ulong end = 0; 

	cl_int ret;

	cl_command_queue command_queue = NULL; 
	//const cl_queue_properties *CQPE = CL_QUEUE_PROFILING_ENABLE;
	//command_queue = clCreateCommandQueueWithProperties(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	//--------------------------------------------------------------------------------
	//--5th: Create Memory Object------------------------------------------------------------------------------
	
	cl_mem memobj = NULL;

	// The following function allocates space on the device(GPU) memory
	// The allocated memory can be accessed from host side using the returned memobj pointer
	memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, 400 , NULL, 0);
	//--------------------------------------------------------------------------------
	//--6th: Read Kernel File------------------------------------------------------------------------------
	//Online Compiling 
	//Reading the Kernel's Source Code

	FILE *fp;
	char fileName[] = "./hello.cl";
	char *source_str;
	size_t source_size;

	//Loading the kernel
	fp = fopen(fileName, "r");

	source_str = (char*) malloc(1000);
	
	//The source code of "hello.cl" file is in the source_str variable
	//The number of the characters contained in "hello.cl" file is in source_size variable
	source_size = fread(source_str, 1, 1000, fp);

	printf("number of characters read is %d \n\n", source_size);
	printf("Begining of hello.cl file: \n\n");
	puts(source_str);
	fclose(fp);
	//--------------------------------------------------------------------------------
	//--7th: Create Program Object------------------------------------------------------------------------------

	cl_program program = NULL;

	program = clCreateProgramWithSource(context, 1,(const char**) &source_str,(const size_t*) &source_size, 0);
	//--------------------------------------------------------------------------------
	//--8th: Compile the Program Object------------------------------------------------------------------------------

	clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	//--------------------------------------------------------------------------------
	//--9th: Create Kernel Object------------------------------------------------------------------------------
	
	cl_kernel kernel = NULL;

	kernel = clCreateKernel(program, "hello", 0);
	//--------------------------------------------------------------------------------
	//--10th: Set Kernel Arguments------------------------------------------------------------------------------

	clSetKernelArg(kernel, 0, 4, (void *)&memobj);

	printf("\n \n the size of cl_mem is : %d \n\n", sizeof(cl_mem));
	//--------------------------------------------------------------------------------
	//--11th: Executing the Kernel (Enqueue Task)---------------------------------------------------------------

	size_t global_work_size[1] = {1};
	size_t local_work_size[1] = {1};

	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &event);

	// to calculate the execution time
	clWaitForEvents(1, &event);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	//--------------------------------------------------------------------------------
	//--12th : Read from the Memory Object----------------------------------------------------------------------

	char string[400];

	clEnqueueReadBuffer(command_queue, memobj, CL_TRUE, 0, 400, string, 0, NULL, NULL);
	//--------------------------------------------------------------------------------
	//--13th: Free Objects------------------------------------------------------------------------------

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(memobj);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
	//--------------------------------------------------------------------------------
	//--Displaying results------------------------------------------------------------------------------
	puts(string);
	//--------------------------------------------------------------------------------
	//--Freeing host-side objects------------------------------------------------------------------------------
	free(source_str);
	//--------------------------------------------------------------------------------

	getchar();
	return 0;
} 

