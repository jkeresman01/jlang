fn main() -> i32 {
    // Stack-allocated array with literal
    var arr: i32[5] = [10, 20, 30, 40, 50];

    // Read from array
    printf("%d\n", arr[0]);
    printf("%d\n", arr[2]);
    printf("%d\n", arr[4]);

    // Write to array
    arr[0] = 99;
    printf("%d\n", arr[0]);

    // Loop over array
    var i: i32 = 0;
    for (var j: i32 = 0; j < 5; j++) {
        printf("arr[%d] = %d\n", j, arr[j]);
    }

    // Type inference with array literal
    var nums := [1, 2, 3];
    printf("%d\n", nums[0]);
    printf("%d\n", nums[1]);
    printf("%d\n", nums[2]);

    // Heap-allocated array
    var heap: i32[10]* = alloc<i32[10]>();
    heap[0] = 100;
    heap[1] = 200;
    printf("heap[0] = %d\n", heap[0]);
    printf("heap[1] = %d\n", heap[1]);

    return 0;
}
