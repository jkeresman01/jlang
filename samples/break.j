fn main() -> i32 {
    // Break out of a while loop
    var i: i32 = 0;
    while (i < 100) {
        if (i == 5) {
            break;
        }
        printf("while i = %d", i);
        i = i + 1;
    }
    printf("After while: i = %d", i);

    // Break out of a for loop
    for (var j: i32 = 0; j < 100; j++) {
        if (j == 3) {
            break;
        }
        printf("for j = %d", j);
    }
    printf("After for loop");

    return 0;
}
