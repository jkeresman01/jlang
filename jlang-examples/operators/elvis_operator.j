// Elvis operator (?:) in jlang
// Returns the left operand if non-null, otherwise evaluates and returns the right operand.
// Short-circuit: the right side is only evaluated when the left side is null.

struct Config {
    Timeout: i32;
}

fn getDefaultConfig() -> Config* {
    var cfg: Config* = alloc<Config>();
    cfg.Timeout = 30;
    return cfg;
}

fn main() -> i32 {
    // Nullable pointer that starts as null
    var primary: Config*? = null;

    // Elvis operator: primary is null, so getDefaultConfig() is called
    var cfg: Config* = primary ?: getDefaultConfig();
    printf("Timeout: %d\n", cfg.Timeout);

    // Now assign a real value to primary
    var custom: Config* = alloc<Config>();
    custom.Timeout = 60;
    primary = custom;

    // Elvis operator: primary is non-null, so getDefaultConfig() is NOT called
    var cfg2: Config* = primary ?: getDefaultConfig();
    printf("Timeout: %d\n", cfg2.Timeout);

    free(cfg);
    free(custom);
    return 0;
}
