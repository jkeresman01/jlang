// Test Result<T, E> type with match expressions

fn main() -> i32 {
    // Test Ok case
    var result1: Result<i32, char*> = Ok(42);

    var x: i32 = match result1 {
        Ok(v) => v * 2,
        Err(e) => 0
    };

    printf("Ok result: %d\n", x);

    // Test Err case
    var result2: Result<i32, char*> = Err("Something went wrong");

    var y: i32 = match result2 {
        Ok(v) => v,
        Err(e) => -1
    };

    printf("Err result: %d\n", y);

    // Test match as statement (void)
    var result3: Result<i32, char*> = Ok(100);

    match result3 {
        Ok(q) => {
            printf("Success: %d\n", q);
        },
        Err(msg) => {
            printf("Error: %s\n", msg);
        }
    };

    return 0;
}
