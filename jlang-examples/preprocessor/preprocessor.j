// Preprocessor sample: #define, #undef, #ifdef, #ifndef, #else, #endif, #include

#include "preprocessor_defs.j"

#define DEBUG
#define VERSION 2
#define LABEL "jlang-preprocessor"

fn main() -> i32 {
    // Macro substitution in regular code
    var limit: i32 = MAX_SIZE;
    var ver: i32 = VERSION;
    printf("limit = %d, version = %d", limit, ver);

    // Macros inside string literals are NOT substituted
    printf("MAX_SIZE as text");

    // Greeting comes from the included file
    var msg: char* = GREETING;
    printf("%s", msg);

    // #ifdef: DEBUG is defined, so this block is included
#ifdef DEBUG
    printf("Debug mode is ON");
#endif

    // #ifndef: RELEASE is not defined, so this block is included
#ifndef RELEASE
    printf("This is not a release build");
#endif

    // #ifdef / #else: RELEASE is not defined, takes the else branch
#ifdef RELEASE
    printf("Release path");
#else
    printf("Non-release path");
#endif

#ifdef DEBUG
    #ifndef VERBOSE
        printf("Debug without verbose");
    #else
        printf("Debug with verbose");
    #endif
#endif

#undef DEBUG

#ifdef DEBUG
    printf("This should NOT appear");
#else
    printf("DEBUG was undefined");
#endif

    // Substitution still works for remaining macros
    var tag: char* = LABEL;
    printf("tag = %s", tag);

    return 0;
}
