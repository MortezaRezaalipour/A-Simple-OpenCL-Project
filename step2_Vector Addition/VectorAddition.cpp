// OpenCL2.cpp : Defines the entry point for the console application.
// Vector Addition A + B = C
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <CL\cl.h>

const int ARRAY_SIZE = 100;
const int FILE_SIZE = 1000;

int main()
{
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
	printf("\n\n");
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

	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	//--------------------------------------------------------------------------------
	//--5th: Create Memory Object------------------------------------------------------------------------------

	//Pointers to device memory
	cl_mem Amemobj = NULL;
	cl_mem Bmemobj = NULL;
	cl_mem Cmemobj = NULL;

	// The following function allocates space on the device(GPU) memory
	// The allocated memory can be accessed from host side using the returned memobj pointer
	Amemobj = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_SIZE * sizeof(float), NULL, 0);
	Bmemobj = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_SIZE * sizeof(float), NULL, 0);
	Cmemobj = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_SIZE * sizeof(float), NULL, 0);

	//Pointers to Host memory
	float *A;
	float *B;
	float *C;

	A = (float*)malloc(ARRAY_SIZE * sizeof(float));
	B = (float*)malloc(ARRAY_SIZE * sizeof(float));
	C = (float*)malloc(ARRAY_SIZE * sizeof(float));

	// initializing A and B 
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		A[i] = i;
		B[i] = i;
	}
	// Displayin Array A----------------------------------------
	printf("This is A : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" %.2f", A[i]);
	}
	printf("\n");
	//----------------------------------------------------------

	//Displaying Array B----------------------------------------
	printf("This is B : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" %.2f", B[i]);
	}
	printf("\n");
	//----------------------------------------------------------

	// Copying A and B into Device memory 
	clEnqueueWriteBuffer(command_queue, Amemobj, CL_TRUE, 0,ARRAY_SIZE * sizeof(float), A, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, Bmemobj, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), B, 0, NULL, NULL);
	//--------------------------------------------------------------------------------
	//--6th: Read Kernel File------------------------------------------------------------------------------
	//Online Compiling 
	//Reading the Kernel's Source Code

	FILE *fp;
	char fileName[] = "./vectoraddition.cl";
	char *source_str;
	size_t source_size;

	//Loading the kernel
	fp = fopen(fileName, "r");

	source_str = (char*)malloc(FILE_SIZE);

	//The source code of "hello.cl" file is in the source_str variable
	//The number of the characters contained in "hello.cl" file is in source_size variable
	source_size = fread(source_str, 1, FILE_SIZE, fp);

	printf("number of characters read is %d \n\n", source_size);
	printf("Begining of hello.cl file: \n\n");
	puts(source_str);
	fclose(fp);
	//--------------------------------------------------------------------------------
	//--7th: Create Program Object------------------------------------------------------------------------------

	cl_program program = NULL;

	program = clCreateProgramWithSource(context, 1, (const char**)&source_str, (const size_t*)&source_size, 0);
	//--------------------------------------------------------------------------------
	//--8th: Compile the Program Object------------------------------------------------------------------------------

	clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	//--------------------------------------------------------------------------------
	//--9th: Create Kernel Object------------------------------------------------------------------------------

	cl_kernel kernel = NULL;

	kernel = clCreateKernel(program, "VectorAddition", 0);
	//--------------------------------------------------------------------------------
	//--10th: Set Kernel Arguments------------------------------------------------------------------------------

	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&Amemobj);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&Bmemobj);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&Cmemobj);

	printf("\n \n the size of cl_mem is : %d \n\n", sizeof(cl_mem));
	//--------------------------------------------------------------------------------
	//--11th: Executing the Kernel (Enqueue Task)---------------------------------------------------------------

	size_t global_work_size[1] = { ARRAY_SIZE };
	size_t local_work_size[1] = { 1 };

	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &event);

	// to calculate the execution time
	clWaitForEvents(1, &event);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	//--------------------------------------------------------------------------------
	//--12th : Read from the Memory Object----------------------------------------------------------------------

	

	clEnqueueReadBuffer(command_queue, Cmemobj, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), C, 0, NULL, NULL);

	//Displaying Results : Array C ----------------------------------------
	printf("This is C : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" %.2f", C[i]);
	}
	printf("\n");
	//----------------------------------------------------------

	//--------------------------------------------------------------------------------
	//--13th: Free Objects------------------------------------------------------------------------------
	
	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(Amemobj);
	clReleaseMemObject(Bmemobj);
	clReleaseMemObject(Cmemobj);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	//--Freeing host-side objects------------------------------------------------------------------------------
	free(A);
	free(B);
	free(C);
	free(source_str);
	//--------------------------------------------------------------------------------


	getchar();
	return 0;
}

