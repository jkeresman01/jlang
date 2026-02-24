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

      {/* ── Section 4: How Other Languages Do It ── */}
      <h2>How Other Languages Do It</h2>

      <h3>Java — Type Erasure</h3>
      <p>
        Java takes the opposite approach to monomorphization. When you write{' '}
        <code>List&lt;String&gt;</code> in Java, the compiler checks the types at
        compile time but then <strong>erases</strong> them — the compiled bytecode
        just sees a raw <code>List</code> of <code>Object</code>. This means there
        is only a single version of the generic class at runtime, regardless of how
        many different type arguments you use.
      </p>
      <p>The practical consequences are:</p>
      <ul>
        <li>
          You cannot inspect type parameters at runtime —{' '}
          <code>if (x instanceof List&lt;String&gt;)</code> is a compile error in
          Java because that information is gone.
        </li>
        <li>
          You cannot write <code>new T()</code> or <code>new T[]</code> inside a
          generic class because the JVM doesn't know what <code>T</code> actually
          is.
        </li>
        <li>
          Primitives like <code>int</code> must be boxed into{' '}
          <code>Integer</code> to be used with generics, adding allocations and
          indirection.
        </li>
      </ul>
      <p>
        The upside is smaller compiled output — one copy of the code serves all
        types — but the trade-off is less type information at runtime and extra
        overhead from boxing and casts.
      </p>

      <h3>Kotlin — <code>reified</code> Type Parameters</h3>
      <p>
        Kotlin runs on the JVM so it inherits Java's type erasure by default. But
        it has a clever escape hatch: the <code>reified</code> keyword. When you
        mark a type parameter as <code>reified</code> on an{' '}
        <code>inline</code> function, the compiler inlines the function body at
        every call site and substitutes the real type — giving you access to the
        type argument at runtime:
      </p>
      <CodeBlock language="kotlin" code={`inline fun <reified T> isType(value: Any): Boolean {
    return value is T   // works! T is known at runtime
}`} />
      <p>
        This is essentially <strong>monomorphization for a single function</strong>{' '}
        — the compiler copies the function body and plugs in the concrete type,
        just like jlang and C++ do for all generics. The limitation is that{' '}
        <code>reified</code> only works on <code>inline</code> functions, not on
        classes or regular methods, so it's a targeted fix rather than a
        language-wide strategy.
      </p>

      <Callout type="note">
        <strong>TL;DR:</strong> Java erases generic types at compile time (one
        shared implementation, no runtime type info). Kotlin's{' '}
        <code>reified</code> brings type info back for inline functions via
        copy-paste monomorphization. jlang monomorphizes everything — every generic
        usage becomes its own fully specialised, zero-overhead version.
      </Callout>
    </>
  )
}
