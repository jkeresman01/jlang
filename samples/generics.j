fn max<T>(a: T, b: T) -> T {
    if (a > b) { return a; }
    return b;
}

struct Pair<T, U> {
    First: T;
    Second: U;
}

fn main() -> i32 {
    var a: i32 = max<i32>(10, 20);
    printf("max<i32>(10, 20) = %d\n", a);

    var b: f64 = max<f64>(3.14, 2.71);
    printf("max<f64>(3.14, 2.71) = %f\n", b);

    var p: Pair<i32, f64>* = alloc<Pair<i32, f64>>();
    p.First = 42;
    p.Second = 3.14;
    printf("Pair: (%d, %f)\n", p.First, p.Second);
    free(p);

    return 0;
}
