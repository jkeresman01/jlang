interface IPrintable {
    fn print();
}

struct Person : IPrintable {
    Name: char*;    // Public - uppercase
    Age: i32;       // Public - uppercase
}

fn print(self: Person*) {
    printf("Name: %s", self.Name);
    printf("Age: %d", self.Age);
}

struct Frog : IPrintable {
    Name: char*;    // Public
    Age: i32;       // Public
}

fn print(self: Frog*) {
    printf("Frog's name: %s", self.Name);
    printf("Age: %d", self.Age);
}

fn main() -> i32 {
    var person: Person* = alloc<Person>();
    person.Name = "Alice";
    person.Age = 30;

    person.print();

    var frog: Frog* = alloc<Frog>();
    frog.Name = "Kermit";
    frog.Age = 5;

    frog.print();

    free(person);
    free(frog);
    return 0;
}
