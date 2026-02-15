fn main() -> i32 {
    // Continue in a while loop - skip printing when i == 2
    var i: i32 = 0;
    while (i < 5) {
        i = i + 1;
        if (i == 3) {
            continue;
        }
        printf("while i = %d", i);
    }
    printf("After while: i = %d", i);

    // Continue in a for loop - skip printing when j == 2
    for (var j: i32 = 0; j < 5; j++) {
        if (j == 2) {
            continue;
        }
        printf("for j = %d", j);
    }
    printf("After for loop");

    return 0;
}
