fn main() -> i32 {
    var x: i32 = 2;

    // Switch statement (C-style with fallthrough)
    switch (x) {
        case 1:
            printf("one");
            break;
        case 2:
            printf("two");
            break;
        case 3:
            printf("three");
            break;
        default:
            printf("other");
    }

    // Switch statement with fallthrough (no break between cases)
    var y: i32 = 1;
    switch (y) {
        case 1:
        case 2:
            printf("one or two");
            break;
        case 3:
            printf("three");
            break;
        default:
            printf("default");
    }

    // Switch expression (no fallthrough, => arms)
    var z: i32 = 3;
    var result: i32 = switch (z) {
        case 1 => 10,
        case 2 => 20,
        case 3 => 30,
        default => 0
    };
    printf("switch expr result = %d", result);

    // Switch expression with multiple values per arm
    var w: i32 = 2;
    var category: i32 = switch (w) {
        case 1, 2, 3 => 1,
        case 4, 5, 6 => 2,
        default => 0
    };
    printf("category = %d", category);

    return 0;
}
