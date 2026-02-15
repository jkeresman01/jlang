import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Functions() {
  return (
    <>
      <h1>Functions</h1>

      {/* ── Section 1: Declaring Functions ── */}
      <h2>Declaring Functions</h2>
      <p>
        Functions are declared with the <code>fn</code> keyword. Parameters use
        colon-separated type annotations, and the return type is specified with a
        trailing arrow <code>-&gt;</code>.
      </p>

      <CodeBlock code={`fn add(a: i32, b: i32) -> i32 {
    return a + b;
}`} />

      <p>
        The <code>fn</code> keyword is concise and easy to scan. Trailing return
        types improve readability by keeping the function name and parameters
        front-and-center, and they align naturally when scanning a list of function
        signatures.
      </p>

      <CodeBlock code={`fn greet(name: char*) -> void {
    printf("Hello, %s!", name);
}

fn square(x: i32) -> i32 {
    return x * x;
}

fn isEven(n: i32) -> bool {
    return n % 2 == 0;
}`} />

      <Callout type="note">
        The <code>fn</code> keyword and trailing return type syntax are inspired
        by Rust and Kotlin. This style is increasingly common in modern languages
        because it makes function signatures easier to read and parse.
      </Callout>

      {/* ── Section 2: Methods with Explicit Self ── */}
      <h2>Methods with Explicit Self</h2>
      <p>
        Methods on structs take an explicit <code>self</code> parameter as their
        first argument. The type of <code>self</code> is a pointer to the struct,
        making the receiver visible and unambiguous.
      </p>

      <CodeBlock code={`fn print(self: Person*) {
    printf("Name: %s", self.Name);
}`} />

      <p>
        This means there is no hidden <code>this</code> pointer. The receiver is
        just a regular parameter that you declare and name yourself.
      </p>

      <CodeBlock code={`fn getName(self: Person*) -> char* {
    return self.Name;
}

fn setAge(self: Person*, newAge: i32) -> void {
    self.Age = newAge;
}`} />

      <Callout type="note">
        Explicit self makes the receiver clear at every call site and in every
        function signature. This design is similar to Python's <code>self</code> and
        Rust's <code>self</code> / <code>&amp;self</code> parameter. There are no
        implicit receivers or hidden <code>this</code> bindings to reason about.
      </Callout>
    </>
  )
}
