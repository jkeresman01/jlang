// Test Result<T, E> type with match expressions

fn divide(a: i32, b: i32) -> Result<i32, char*> {
    if (b == 0) {
        return Err("Division by zero");
    }
    return Ok(a / b);
}

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

    // Test function returning Result - Ok path
    var r1: Result<i32, char*> = divide(10, 3);
    match r1 {
        Ok(v) => {
            printf("10 / 3 = %d\n", v);
        },
        Err(msg) => {
            printf("Error: %s\n", msg);
        }
    };

    // Test function returning Result - Err path
    var r2: Result<i32, char*> = divide(5, 0);
    match r2 {
        Ok(v) => {
            printf("5 / 0 = %d\n", v);
        },
        Err(msg) => {
            printf("Error: %s\n", msg);
        }
    };

    return 0;
}
