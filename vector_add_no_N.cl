__kernel void vector_add(__global const int* a, __global const int* b, __global int* c) {
    int id = get_global_id(0);
    c[id] = a[id] + b[id];
}
