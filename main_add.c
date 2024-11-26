#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>

#define ARRAY_SIZE 1000

void print_results(const float* A, const float* B, const float* C, size_t size) {
    printf("First 10 entries:\n");
    for (int i = 0; i < 10; i++) {
        printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
    }

    printf("Last 10 entries:\n");
    for (int i = size - 10; i < size; i++) {
        printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
    }
}

void vector_add(float* A, float* B, float* C, size_t size, const char* kernelSource) {
    cl_int err;

    // Get the platform and device
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create OpenCL context and command queue
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);

    // Create OpenCL buffers
    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, size * sizeof(float), NULL, &err);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, size * sizeof(float), NULL, &err);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, size * sizeof(float), NULL, &err);

    // Copy data to device
    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, size * sizeof(float), A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, size * sizeof(float), B, 0, NULL, NULL);

    // Create and build OpenCL program
    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
    clBuildProgram(program, 1, &device, NULL, NULL, NULL);

    // Create kernel and set arguments
    cl_kernel kernel = clCreateKernel(program, "vector_add", &err);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    // Execute the kernel
    size_t globalSize = size;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

    // Read back the results
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, size * sizeof(float), C, 0, NULL, NULL);

    // Clean up
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

int main() {
    // Initialize host data
    float A[ARRAY_SIZE], B[ARRAY_SIZE], C[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        A[i] = i;
        B[i] = 3 * i;
    }

    // Kernel source code
    const char* kernelSource = 
        "__kernel void vector_add(__global const float* A, __global const float* B, __global float* C) {"
        "    int id = get_global_id(0);"
        "    if (id < 1000) {"
        "        C[id] = A[id] + B[id];"
        "    }"
        "}";

    // Perform vector addition
    vector_add(A, B, C, ARRAY_SIZE, kernelSource);

    // Print results
    print_results(A, B, C, ARRAY_SIZE);

    return 0;
}
