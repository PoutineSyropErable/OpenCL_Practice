#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to select a specific GPU by board name
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
                printf("Found Device: %s\n", board_name);
                if (strcmp(board_name, target_board_name) == 0) {
                    cl_device_id selected_device = devices[j];
                    free(devices);
                    free(platforms);
                    return selected_device; // Return the matching device
                }
            }
        }
        free(devices);
    }
    free(platforms);

    printf("Device '%s' not found.\n", target_board_name);
    return NULL;
}

int main() {
    // Target board name for AMD Radeon RX 6750 XT
    const char *target_board_name = "AMD Radeon RX 6750 XT";

    // Select the desired device
    cl_device_id device = select_device(target_board_name);
    if (device == NULL) {
        printf("Failed to select the device.\n");
        return 1;
    }

    printf("Selected Device: %s\n", target_board_name);

    // You can now create an OpenCL context with the selected device
    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, NULL);
    if (context == NULL) {
        printf("Failed to create OpenCL context for the selected device.\n");
        return 1;
    }

    printf("OpenCL context created successfully.\n");

    // Clean up
    clReleaseContext(context);
    return 0;
}
