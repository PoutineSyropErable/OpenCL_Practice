#include <stdio.h>
#include <CL/cl.h>

#define ARRAY_SIZE 1000

int main() {
    // Initialize host data
    float A[ARRAY_SIZE], B[ARRAY_SIZE], C[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        A[i] = i;
        B[i] = i * 2;
    }

    // OpenCL setup
    cl_platform_id platform;
    cl_device_id device;
    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    cl_command_queue queue = clCreateCommandQueue(context, device, 0, NULL);

    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_SIZE * sizeof(float), NULL, NULL);
    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY, ARRAY_SIZE * sizeof(float), NULL, NULL);
    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, ARRAY_SIZE * sizeof(float), NULL, NULL);

    clEnqueueWriteBuffer(queue, bufferA, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), A, 0, NULL, NULL);
    clEnqueueWriteBuffer(queue, bufferB, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), B, 0, NULL, NULL);

    const char* kernelSource = 
        "__kernel void vector_add(__global const float* A, __global const float* B, __global float* C) {"
        "    int id = get_global_id(0);"
        "    C[id] = A[id] + B[id];"
        "}";

    cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, NULL);
    clBuildProgram(program, 1, &device, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "vector_add", NULL);
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &bufferA);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &bufferB);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &bufferC);

    size_t globalSize = ARRAY_SIZE;
    clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);
    clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, ARRAY_SIZE * sizeof(float), C, 0, NULL, NULL);

    // Output results
    for (int i = 0; i < 10; i++) {
        printf("%f + %f = %f\n", A[i], B[i], C[i]);
    }

    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);

    return 0;
}
