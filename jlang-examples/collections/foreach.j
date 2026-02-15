fn main() -> i32 {
    // foreach over fixed-size array
    var arr: i32[5];
    arr[0] = 10;
    arr[1] = 20;
    arr[2] = 30;
    arr[3] = 40;
    arr[4] = 50;

    printf("Array foreach:\n");
    for elem in arr {
        printf("  %d\n", elem);
    }

    // foreach over vector
    val v := std::Vector<i32>();
    v.push(100);
    v.push(200);
    v.push(300);

    printf("Vector foreach:\n");
    for item in v {
        printf("  %d\n", item);
    }

    // foreach with break
    printf("Break at 200:\n");
    for item in v {
        if item == 200 {
            break;
        }
        printf("  %d\n", item);
    }

    // foreach with continue
    printf("Skip 200:\n");
    for item in v {
        if item == 200 {
            continue;
        }
        printf("  %d\n", item);
    }

    v.free();
    return 0;
}
