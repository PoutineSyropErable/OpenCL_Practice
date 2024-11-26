#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CL/cl.h>

#define ARRAY_SIZE 1000

void list_devices() {
    cl_uint num_platforms;
    clGetPlatformIDs(0, NULL, &num_platforms);
    if (num_platforms == 0) {
        printf("No OpenCL platforms found.\n");
        return;
    }

    cl_platform_id *platforms = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));
    clGetPlatformIDs(num_platforms, platforms, NULL);

    for (cl_uint i = 0; i < num_platforms; ++i) {
        char platform_name[128];
        clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
        printf("\nPlatform %u: %s\n", i, platform_name);

        cl_uint num_devices = 0;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (num_devices == 0) continue;

        cl_device_id *devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

        for (cl_uint j = 0; j < num_devices; ++j) {
            char device_name[128];
            char board_name[128] = "Unknown";
            clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
            clGetDeviceInfo(devices[j], 0x4038 /* CL_DEVICE_BOARD_NAME_AMD */, sizeof(board_name), board_name, NULL);
            printf("  Device %u: %s\n", j, device_name);
            printf("    Board Name: %s\n", board_name);
        }

        free(devices);
    }
    free(platforms);
}

cl_device_id select_device(const char *target_board_name) {
    cl_uint num_platforms = 0;
    clGetPlatformIDs(0, NULL, &num_platforms);
    if (num_platforms == 0) {
        printf("No OpenCL platforms found.\n");
        return NULL;
    }

    cl_platform_id *platforms = (cl_platform_id *)malloc(num_platforms * sizeof(cl_platform_id));
    clGetPlatformIDs(num_platforms, platforms, NULL);

    for (cl_uint i = 0; i < num_platforms; ++i) {
        cl_uint num_devices = 0;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        if (num_devices == 0) continue;

        cl_device_id *devices = (cl_device_id *)malloc(num_devices * sizeof(cl_device_id));
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

        for (cl_uint j = 0; j < num_devices; ++j) {
            char board_name[128];
            if (clGetDeviceInfo(devices[j], 0x4038 /* CL_DEVICE_BOARD_NAME_AMD */, sizeof(board_name), board_name, NULL) == CL_SUCCESS) {
                if (strcmp(board_name, target_board_name) == 0) {
                    cl_device_id selected_device = devices[j];
                    free(devices);
                    free(platforms);
                    return selected_device;
                }
            }
        }
        free(devices);
    }
    free(platforms);
    return NULL;
}

void vector_add(float* A, float* B, float* C, size_t size, cl_device_id device) {
    cl_int err;

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

    // Kernel source code
    const char* kernelSource =
        "__kernel void vector_add(__global const float* A, __global const float* B, __global float* C) {"
        "    int id = get_global_id(0);"
        "    if (id < 1000) {"
        "        C[id] = A[id] + B[id];"
        "    }"
        "}";

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

    // Cleanup
    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
}

int main() {
    // List devices
    printf("Listing available devices:\n");
    list_devices();

    // Select the target device
    const char *target_board_name = "AMD Radeon RX 6750 XT";
    cl_device_id device = select_device(target_board_name);
    if (device == NULL) {
        printf("Failed to select device: %s\n", target_board_name);
        return 1;
    }
    printf("\n\nSelected device: %s\n\n", target_board_name);

    // Initialize host data
    float A[ARRAY_SIZE], B[ARRAY_SIZE], C[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        A[i] = i;
        B[i] = 3 * i;
    }

    // Perform vector addition
    vector_add(A, B, C, ARRAY_SIZE, device);

    // Output results
    printf("First 10 results:\n");
    for (int i = 0; i < 10; i++) {
        printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
    }

    printf("Last 10 results:\n");
    for (int i = ARRAY_SIZE - 10; i < ARRAY_SIZE; i++) {
        printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
    }

    return 0;
}
