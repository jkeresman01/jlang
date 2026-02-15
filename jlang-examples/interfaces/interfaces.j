// Interface with full method signatures
interface Greeter {
    fn greet(name: char*) -> void;
    fn get_greeting() -> i32;
}

// Struct implementing Greeter
struct Person : Greeter {
    Name: char*;
    Age: i32;
}

struct Robot : Greeter {
    Id: i32;
    Model: char*;
}

// Methods for Person (self: Person*)
fn greet(self: Person*, name: char*) -> void {
    printf("Hello %s, I am a person named %s!\n", name, self.Name);
}

fn get_greeting(self: Person*) -> i32 {
    return 1;
}

// Methods for Robot (self: Robot*)
fn greet(self: Robot*, name: char*) -> void {
    printf("BEEP BOOP! Greetings %s, I am robot model %s!\n", name, self.Model);
}

fn get_greeting(self: Robot*) -> i32 {
    return 2;
}

// Polymorphic function taking interface type
fn do_greet(g: Greeter, target: char*) -> void {
    g.greet(target);
}

fn main() -> i32 {
    // Create struct instances
    var p: Person* = alloc<Person>();
    p.Name = "Alice";
    p.Age = 30;

    var r: Robot* = alloc<Robot>();
    r.Id = 42;
    r.Model = "T-1000";

    // Direct method calls on concrete types
    p.greet("World");
    r.greet("World");

    // Virtual dispatch through interface
    var g1: Greeter = p;
    var g2: Greeter = r;

    g1.greet("via interface");
    g2.greet("via interface");

    // Get greeting codes
    var code1: i32 = g1.get_greeting();
    var code2: i32 = g2.get_greeting();
    printf("Greeting codes: %d, %d\n", code1, code2);

    // Polymorphic function call
    do_greet(g1, "polymorphic");
    do_greet(g2, "polymorphic");

    return 0;
}
