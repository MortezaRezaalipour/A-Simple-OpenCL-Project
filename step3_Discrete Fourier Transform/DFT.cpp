// OpenCL3.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <CL\cl.h>
const int ARRAY_SIZE = 300;
const int FILE_SIZE = 1000;
typedef struct{
	float Re;
	float Im;
} complex;

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

	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	//--------------------------------------------------------------------------------
	//--5th: Create Memory Object------------------------------------------------------------------------------

	cl_mem input_memobj = NULL;
	cl_mem output_memobj = NULL;
	cl_mem n_memobj = NULL;

	// The following function allocates space on the device(GPU) memory
	// The allocated memory can be accessed from host side using the returned memobj pointer
	input_memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_SIZE * sizeof(complex), NULL, 0);
	output_memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, ARRAY_SIZE * sizeof(complex), NULL, 0);
	n_memobj = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_mem), NULL, 0);


	printf("\n %d", sizeof(complex));
	printf("\n %d", sizeof(float));
	printf("\n %d", sizeof(cl_mem));
	printf("\n %d", sizeof(int));

	// Pointers to host memory
	complex *input;
	complex *output;
	int n = ARRAY_SIZE;

	input = (complex*)malloc(ARRAY_SIZE * sizeof(complex));
	output = (complex*)malloc(ARRAY_SIZE * sizeof(complex));

	// initializing input and output
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		input[i].Re = i + 1;
		input[i].Im = 0;
		//output[i].Im = 1;
		//output[i].Re = 1;
	}

	// Displayin Array input----------------------------------------
	printf("\n \n This is input array before dft : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" (%.2f , %.2f) ", input[i].Re, input[i].Im);
	}
	printf("\n");

	printf("This is output : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" (%.2f, %.4f)  ", output[i].Re, output[i].Im);
	}
	printf("\n");
	printf("%d", n);

	// Copying Input  into Device memory 
	clEnqueueWriteBuffer(command_queue, input_memobj, CL_TRUE, 0, ARRAY_SIZE * sizeof(complex), input, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, n_memobj, CL_TRUE, 0, sizeof(cl_mem), (void *)&n, 0, NULL, NULL);
	//--------------------------------------------------------------------------------
	//--6th: Read Kernel File------------------------------------------------------------------------------
	//Online Compiling 
	//Reading the Kernel's Source Code

	FILE *fp;
	char fileName[] = "./calconeoutput.cl";
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

	kernel = clCreateKernel(program, "calconeoutput", 0);
	//--------------------------------------------------------------------------------
	//--10th: Set Kernel Arguments------------------------------------------------------------------------------

	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_memobj);
	clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&output_memobj);
	clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&n_memobj);

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
	//printf("\n\n Start time = %.20f ", start);
	//printf("\n\n end time = %.20f \n", end);
	printf(" execution time: %10.5f [ms] \n\n", (end - start) / 1000000.0);
	//--------------------------------------------------------------------------------
	//--12th : Read from the Memory Object----------------------------------------------------------------------

	clEnqueueReadBuffer(command_queue, output_memobj, CL_TRUE, 0, ARRAY_SIZE * sizeof(complex), output, 0, NULL, NULL);

	//Displaying Results : Array C ----------------------------------------
	printf("This is output : \n");
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf(" (%.2f, %.4f)  ", output[i].Re, output[i].Im);
	}
	printf("\n");
	printf("%d", n);
	//----------------------------------------------------------
	//--13th: Free Objects------------------------------------------------------------------------------

	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(input_memobj);
	clReleaseMemObject(output_memobj);
	clReleaseMemObject(n_memobj);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

	//--Freeing host-side objects------------------------------------------------------------------------------
	free(input);
	free(output);
	free(source_str);
	//--------------------------------------------------------------------------------

	getchar();
	return 0;
}
