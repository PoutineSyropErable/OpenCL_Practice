#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE 1000

// Struct to manage OpenCL resources
typedef struct {
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem bufferA;
	cl_mem bufferB;
	cl_mem bufferC;
} OpenCLContext;

// Global device variable
cl_device_id device;

// Function to list available OpenCL devices
void list_devices() {
	cl_uint num_platforms;
	clGetPlatformIDs(0, NULL, &num_platforms);
	if (num_platforms == 0) {
		printf("No OpenCL platforms found.\n");
		return;
	}

	cl_platform_id* platforms = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
	clGetPlatformIDs(num_platforms, platforms, NULL);

	for (cl_uint i = 0; i < num_platforms; ++i) {
		char platform_name[128];
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, NULL);
		printf("\nPlatform %u: %s\n", i, platform_name);

		cl_uint num_devices = 0;
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
		if (num_devices == 0)
			continue;

		cl_device_id* devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
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

// Function to select a specific OpenCL device
cl_device_id select_device(const char* target_board_name) {
	cl_uint num_platforms = 0;
	clGetPlatformIDs(0, NULL, &num_platforms);
	if (num_platforms == 0) {
		printf("No OpenCL platforms found.\n");
		return NULL;
	}

	cl_platform_id* platforms = (cl_platform_id*)malloc(num_platforms * sizeof(cl_platform_id));
	clGetPlatformIDs(num_platforms, platforms, NULL);

	for (cl_uint i = 0; i < num_platforms; ++i) {
		cl_uint num_devices = 0;
		clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
		if (num_devices == 0)
			continue;

		cl_device_id* devices = (cl_device_id*)malloc(num_devices * sizeof(cl_device_id));
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

// Helper function to load kernel source from a file
char* read_kernel_source(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (!file) {
		printf("Failed to load kernel file: %s\n", filename);
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	size_t file_size = ftell(file);
	rewind(file);

	char* source = (char*)malloc(file_size + 1);
	source[file_size] = '\0'; // Null-terminate the string
	fread(source, 1, file_size, file);
	fclose(file);
	return source;
}

// Function to initialize OpenCL context and resources
OpenCLContext* context_create(size_t size) {
	OpenCLContext* ctx = (OpenCLContext*)malloc(sizeof(OpenCLContext));
	cl_int err;

	// Create context and queue
	ctx->context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	ctx->queue = clCreateCommandQueue(ctx->context, device, 0, &err);

	// Allocate buffers
	ctx->bufferA = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, size * sizeof(float), NULL, &err);
	ctx->bufferB = clCreateBuffer(ctx->context, CL_MEM_READ_ONLY, size * sizeof(float), NULL, &err);
	ctx->bufferC = clCreateBuffer(ctx->context, CL_MEM_WRITE_ONLY, size * sizeof(float), NULL, &err);

	// Load kernel source
	char* kernelSource = read_kernel_source("vector_add.cl");
	if (!kernelSource) {
		printf("Failed to read kernel source.\n");
		free(ctx);
		return NULL;
	}

	// Build program and create kernel
	ctx->program = clCreateProgramWithSource(ctx->context, 1, (const char**)&kernelSource, NULL, &err);
	free(kernelSource);
	clBuildProgram(ctx->program, 1, &device, NULL, NULL, NULL);
	ctx->kernel = clCreateKernel(ctx->program, "vector_add", &err);

	return ctx;
}

// Function to execute kernel and read results
void vector_add(OpenCLContext* ctx, float* A, float* B, float* C, size_t size) {
	cl_int err;

	// Copy data to buffers
	clEnqueueWriteBuffer(ctx->queue, ctx->bufferA, CL_TRUE, 0, size * sizeof(float), A, 0, NULL, NULL);
	clEnqueueWriteBuffer(ctx->queue, ctx->bufferB, CL_TRUE, 0, size * sizeof(float), B, 0, NULL, NULL);

	// Set kernel arguments
	clSetKernelArg(ctx->kernel, 0, sizeof(cl_mem), &ctx->bufferA);
	clSetKernelArg(ctx->kernel, 1, sizeof(cl_mem), &ctx->bufferB);
	clSetKernelArg(ctx->kernel, 2, sizeof(cl_mem), &ctx->bufferC);

	// Execute the kernel
	size_t globalSize = size;
	clEnqueueNDRangeKernel(ctx->queue, ctx->kernel, 1, NULL, &globalSize, NULL, 0, NULL, NULL);

	// Read back the results
	clEnqueueReadBuffer(ctx->queue, ctx->bufferC, CL_TRUE, 0, size * sizeof(float), C, 0, NULL, NULL);
}

// Function to clean up OpenCL resources
void context_cleanup(OpenCLContext* ctx) {
	clReleaseMemObject(ctx->bufferA);
	clReleaseMemObject(ctx->bufferB);
	clReleaseMemObject(ctx->bufferC);
	clReleaseProgram(ctx->program);
	clReleaseKernel(ctx->kernel);
	clReleaseCommandQueue(ctx->queue);
	clReleaseContext(ctx->context);
	free(ctx);
}

// Main function
int main() {
	printf("Listing available devices:\n");
	list_devices();

	const char* target_board_name = "AMD Radeon RX 6750 XT";
	device = select_device(target_board_name);
	if (device == NULL) {
		printf("Failed to select device: %s\n", target_board_name);
		return 1;
	}
	printf("\n\nSelected device: %s\n\n", target_board_name);

	float A[ARRAY_SIZE], B[ARRAY_SIZE], C[ARRAY_SIZE];
	for (int i = 0; i < ARRAY_SIZE; i++) {
		A[i] = i;
		B[i] = 3 * i;
	}

	OpenCLContext* ctx = context_create(ARRAY_SIZE);
	if (!ctx) {
		printf("Failed to create OpenCL context.\n");
		return 1;
	}

	vector_add(ctx, A, B, C, ARRAY_SIZE);

	printf("First 10 results:\n");
	for (int i = 0; i < 10; i++) {
		printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
	}

	printf("Last 10 results:\n");
	for (int i = ARRAY_SIZE - 10; i < ARRAY_SIZE; i++) {
		printf("a[%d] = %f, b[%d] = %f, c[%d] = %f\n", i, A[i], i, B[i], i, C[i]);
	}

	context_cleanup(ctx);
	return 0;
}
