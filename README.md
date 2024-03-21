# A Tutorial for a Simple OpenCL Project

This project demonstrates the basics of OpenCL programming by implementing three programs: (1) a simple "Hello, World!" project, (2) parallel vector addition, (3) parallel Discrete Fourier Transform (DFT). It includes setting up an OpenCL environment, selecting a device, creating a context and command queue, compiling a kernel, and executing it on a GPU.

## Getting Started

### Prerequisites

- OpenCL SDK installed on your system (e.g., NVIDIA, AMD, Intel)
- C/C++ compiler (e.g., GCC, Clang, MSVC)

### Installation

1. Clone or download the project source code to your local machine.
2. Ensure that the OpenCL headers and library files are accessible to your compiler and linker.

## Usage

To run the project, follow these steps:

1. Navigate to the project directory where the source code is located.
2. Compile the `.cpp` file using a C/C++ compiler, for example:

    ```sh
    g++ -o hello_world hello_world.cpp -lOpenCL
    ```

3. Run the compiled executable:

    ```sh
    ./hello_world
    ```

## Hello World Example

The program goes through the following steps:

- Identifies the OpenCL platform and device.
- Creates an OpenCL context and command queue.
- Reads and compiles the kernel code from `hello.cl`.
- Executes the kernel on the GPU, which modifies a string to output "Hello, World!".
- Retrieves and displays the result from the GPU memory.

## Parallel Vector Addition

This part of the project demonstrates how to perform parallel vector addition using OpenCL. The program adds two arrays (vectors) of floats, A and B, and stores the result in a third array, C. Each element in the result array, C, is the sum of the corresponding elements in arrays A and B.

### How It Works

1. **Initialization**: The program starts by identifying available OpenCL platforms and devices, then selects a GPU device for executing the kernel.
2. **Memory Allocation**: It allocates memory for the input arrays (A and B) and the result array (C) both on the host (CPU) and the device (GPU).
3. **Data Preparation**: The host memory arrays A and B are initialized with values, and these arrays are copied to their corresponding memory objects on the GPU.
4. **Kernel Compilation**: The program reads the kernel code from `vectoraddition.cl`, compiles it, and creates a kernel object. This kernel performs the actual addition of A and B.
5. **Execution**: The kernel is executed on the GPU, with each GPU thread adding one pair of elements from A and B and storing the result in C.
6. **Result Retrieval**: The result array C is copied back from the GPU to the host memory, and then displayed.

### Kernel Function

The `VectorAddition` kernel function takes three arguments: the input arrays A and B, and the output array C. Each thread of execution on the GPU handles one element from each array, performing the addition `C[gid] = A[gid] + B[gid]`, where `gid` is the global ID of the thread.

### Running the Vector Addition

Follow the same steps as the Hello World example to compile and execute this part of the project. Ensure the `vectoraddition.cl` file is in the same directory as the executable.

This example illustrates the basic concepts of data parallel computation in OpenCL, where operations on large data sets are performed in parallel, significantly speeding up the computations compared to sequential execution.


## Parallel Discrete Fourier Transform (DFT)

The third part of the project involves computing the Discrete Fourier Transform (DFT) of a complex signal in parallel using OpenCL. The DFT is used in many areas of engineering, science, and mathematics to transform a signal from its time domain into its frequency domain.

### How It Works

1. **Initialization**: Like the previous examples, the program starts by setting up the OpenCL environment, selecting a device, and creating a context and command queue.
2. **Memory Allocation**: It allocates memory on the GPU for the input and output complex number arrays, and an integer representing the array size.
3. **Data Preparation**: The host memory is initialized with a sample complex signal (real part increasing linearly, imaginary part set to zero), and then copied to the GPU.
4. **Kernel Compilation**: The program reads and compiles the `calconeoutput.cl` kernel code, which calculates one output element of the DFT for each invocation.
5. **Execution**: The kernel is executed on the GPU, where each thread computes one element of the DFT output array in parallel.
6. **Result Retrieval**: The computed DFT is copied back from the GPU to host memory and displayed.

### Kernel Function

The `calconeoutput` kernel function performs the DFT calculation. Each thread computes one element of the DFT output, summing the products of the input signal and the complex exponentials calculated using the formula for DFT.

### Running the DFT

Compile and execute this code similarly to the previous examples. Make sure the `calconeoutput.cl` file is in the same directory as the executable. The output will show the transformed signal in the frequency domain.

This part of the project illustrates the power of OpenCL in handling computationally intensive tasks like DFT, by distributing the workload across multiple processing units to achieve significant performance gains.




## Contributing

Contributions to this project are welcome. Please fork the repository and submit a pull request with your changes.
