typedef struct{
	float Re;
	float Im;
} complex;

__kernel void calconeoutput(__global complex* input, __global complex* output,__global int* n)
{
	float PI = 3.14159265358979323846264338327950288;
	
	int k = get_global_id(0);
	int m = *n;
	int i;
	output[k].Re = 0;
	output[k].Im = 0;
	for (i = 0; i < m; i++)
	{
		float zr = cos(2 * PI * k * i / m);
		float zi = -sin(2 * PI * k * i / m);
		output[k].Re += input[i].Re * zr - input[i].Im * zi;
		output[k].Im += input[i].Re * zi + input[i].Im * zr;
	}
}