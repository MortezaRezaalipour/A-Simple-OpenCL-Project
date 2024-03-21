__kernel void VectorAddition(__global float* A, __global float* B, __global float* C)
{
	int gid = get_global_id(0);
	C[gid] = A[gid] + B[gid];
}