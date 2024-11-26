#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define VECTOR_SIZE 1000

// Wrapper function for vector addition
void vector_add(const int* a, const int* b, int* c, size_t size) {
    cl_int err;

    // Get platform and device information
    cl_platform_id platform;
    cl_device_id device;
    err = clGetPlatformIDs(1, &platform, NULL);
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

    // Create OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);

    // Create command queue
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);

    // Allocate memory buffers on the device
    cl_mem buffer_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * size, NULL, &err);
    cl_mem buffer_b = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * size, NULL, &err);
    cl_mem buffer_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * size, NULL, &err);

    // Copy data to device buffers
    err = clEnqueueWriteBuffer(queue, buffer_a, CL_TRUE, 0, sizeof(int) * size, a, 0, NULL, NULL);
    err = clEnqueueWriteBuffer(queue, buffer_b, CL_TRUE, 0, sizeof(int) * size, b, 0, NULL, NULL);

    // Read and compile kernel source
    FILE* fp = fopen("vector_add.cl", "r");
    if (!fp) {
        perror("Kernel file not found");
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    size_t kernel_size = ftell(fp);
    rewind(fp);
    char* kernel_source = (char*)malloc(kernel_size + 1);
    fread(kernel_source, 1, kernel_size, fp);
    kernel_source[kernel_size] = '\0';
    fclose(fp);

    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernel_source, &kernel_size, &err);
    free(kernel_source);

    // Build kernel program
    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char build_log[4096];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(build_log), build_log, NULL);
        printf("Error in kernel build:\n%s\n", build_log);
        exit(1);
    }

    // Create kernel
    cl_kernel kernel = clCreateKernel(program, "vector_add", &err);

    // Set kernel arguments
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer_a);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &buffer_b);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &buffer_c);

    // Execute kernel
    size_t global_work_size = size;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

    // Copy result back to host
    err = clEnqueueReadBuffer(queue, buffer_c, CL_TRUE, 0, sizeof(int) * size, c, 0, NULL, NULL);

    // Clean up
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(buffer_a);
    clReleaseMemObject(buffer_b);
    clReleaseMemObject(buffer_c);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

int main() {
    int a[VECTOR_SIZE], b[VECTOR_SIZE], c[VECTOR_SIZE];

    // Initialize vectors
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        a[i] = i;
        b[i] = 3 * i;
    }

    // Perform vector addition
    vector_add(a, b, c, VECTOR_SIZE);

    // Print first and last 10 elements
    printf("First 10 elements:\n");
    for (int i = 0; i < 10; ++i) {
        printf("i: %d, a[i]: %d, b[i]: %d, c[i]: %d\n", i, a[i], b[i], c[i]);
    }

    printf("\nLast 10 elements:\n");
    for (int i = VECTOR_SIZE - 10; i < VECTOR_SIZE; ++i) {
        printf("i: %d, a[i]: %d, b[i]: %d, c[i]: %d\n", i, a[i], b[i], c[i]);
    }

    return 0;
}
