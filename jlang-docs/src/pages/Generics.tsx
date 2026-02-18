import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Generics() {
  return (
    <>
      <h1>Generics</h1>

      <p>
        Generics let you write functions and structs that work with any type,
        without duplicating code. You declare one or more <strong>type
        parameters</strong> inside angle brackets (<code>&lt;T&gt;</code>) and
        the compiler generates specialised versions for each concrete type you
        use.
      </p>

      {/* ── Section 1: Generic Functions ── */}
      <h2>Generic Functions</h2>
      <p>
        Place type parameters after the function name. They can appear in
        parameter types, the return type, or both.
      </p>

      <CodeBlock code={`fn max<T>(a: T, b: T) -> T {
    if (a > b) { return a; }
    return b;
}`} />

      <p>
        Call a generic function by providing the concrete type in angle brackets:
      </p>

      <CodeBlock code={`var a: i32 = max<i32>(10, 20);
printf("max<i32>(10, 20) = %d", a);

var b: f64 = max<f64>(3.14, 2.71);
printf("max<f64>(3.14, 2.71) = %f", b);`} />

      <p>
        Each call site like <code>max&lt;i32&gt;</code> and{' '}
        <code>max&lt;f64&gt;</code> produces its own compiled function, fully
        optimised for that type.
      </p>

      {/* ── Section 2: Generic Structs ── */}
      <h2>Generic Structs</h2>
      <p>
        Structs can also be parameterised. A struct with multiple type
        parameters lets you build flexible, reusable data containers.
      </p>

      <CodeBlock code={`struct Pair<T, U> {
    First: T;
    Second: U;
}`} />

      <p>
        Use the concrete type when allocating and accessing the struct:
      </p>

      <CodeBlock code={`var p: Pair<i32, f64>* = alloc<Pair<i32, f64>>();
p.First = 42;
p.Second = 3.14;
printf("Pair: (%d, %f)", p.First, p.Second);
free(p);`} />

      <Callout type="note">
        Generic structs are always allocated on the heap with{' '}
        <code>alloc&lt;T&gt;()</code> and must be freed manually. See{' '}
        <a href="#/memory">Memory Management</a> for details.
      </Callout>

      {/* ── Section 3: Full Example ── */}
      <h2>Full Example</h2>
      <p>
        Putting it all together — generic functions and generic structs in a
        single program:
      </p>

      <CodeBlock code={`fn max<T>(a: T, b: T) -> T {
    if (a > b) { return a; }
    return b;
}

struct Pair<T, U> {
    First: T;
    Second: U;
}

fn main() -> i32 {
    var a: i32 = max<i32>(10, 20);
    printf("max<i32>(10, 20) = %d", a);

    var b: f64 = max<f64>(3.14, 2.71);
    printf("max<f64>(3.14, 2.71) = %f", b);

    var p: Pair<i32, f64>* = alloc<Pair<i32, f64>>();
    p.First = 42;
    p.Second = 3.14;
    printf("Pair: (%d, %f)", p.First, p.Second);
    free(p);

    return 0;
}`} />

      <Callout type="tip">
        <strong>Fun fact:</strong> Under the hood, jlang implements generics
        through <strong>monomorphization</strong> — instead of producing a
        single "generic" function at runtime, the compiler generates completely
        separate, type-specialised versions for each concrete type you use.
        That means <strong>zero runtime overhead</strong>: every call is direct
        and every struct access uses a known memory layout. This is the same
        strategy used by C++ templates and Rust generics. For a great overview
        of different approaches, see{' '}
        <a
          href="https://thume.ca/2019/07/14/a-tour-of-metaprogramming-models-for-generics/"
          target="_blank"
          rel="noopener noreferrer"
        >
          A Tour of Metaprogramming Models for Generics
        </a>{' '}
        by Tristan Hume.
      </Callout>
    </>
  )
}
