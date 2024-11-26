__kernel void vector_add(__global const float* A, __global const float* B, __global float* C) {
    int id = get_global_id(0);
    // if (id < N) { // Ensure we don't access out-of-bounds memory
    C[id] = A[id] + B[id];
    // }
}
