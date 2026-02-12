import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Variables() {
  return (
    <>
      <h1>Variables</h1>

      {/* ── Section 1: Variables with var ── */}
      <h2>Variables with <code>var</code></h2>
      <p>
        Variables are declared with the <code>var</code> keyword using
        colon-separated type annotations. The identifier comes first, followed
        by a colon and the type, then an optional initializer.
      </p>

      <CodeBlock code={`var count: i32 = 42;`} />
      <CodeBlock code={`var name: char* = "jlang";`} />
      <CodeBlock code={`var isReady: bool = true;`} />

      <Callout type="note">
        The colon syntax (<code>name: Type</code>) clearly separates identifiers
        from types, consistent with TypeScript, Kotlin, and Rust.
      </Callout>

      {/* ── Section 2: Type Inference with := ── */}
      <h2>Type Inference with <code>:=</code></h2>
      <p>
        When the type can be determined from the initializer, use the
        walrus operator <code>:=</code> to let the compiler infer it for you.
      </p>

      <CodeBlock
        code={`var x := 42;        // i32
var pi := 3.14159;  // f64
var ready := true;  // bool
var sum := x + 10;  // i32
var isPositive := x > 0;  // bool`}
      />

      <Callout type="note">
        Type inference reduces verbosity while maintaining type safety.
        The type is resolved at compile-time with no runtime overhead.
      </Callout>

      {/* ── Section 3: Immutable variables with val ── */}
      <h2>Immutable variables with <code>val</code></h2>
      <p>
        Use <code>val</code> to declare variables that cannot be reassigned after
        initialization. All mutation operations are rejected at compile
        time: <code>=</code>, <code>+=</code>, <code>-=</code>, <code>*=</code>,{' '}
        <code>/=</code>, <code>%=</code>, <code>++</code>, <code>--</code>.
      </p>

      <CodeBlock
        code={`val pi: f64 = 3.14159;
val maxRetries := 3;
var counter: i32 = 0;
counter = counter + 1;  // OK
// pi = 2.71;           // ERROR: Cannot assign to immutable variable 'pi'
// maxRetries++;        // ERROR: Cannot modify immutable variable 'maxRetries'`}
      />

      <table className="doc-table">
        <thead>
          <tr>
            <th>Keyword</th>
            <th>Mutability</th>
            <th>Use case</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>var</code></td>
            <td>Mutable</td>
            <td>Loop counters, accumulators, state that changes</td>
          </tr>
          <tr>
            <td><code>val</code></td>
            <td>Immutable</td>
            <td>Constants, configuration, values that shouldn't change</td>
          </tr>
        </tbody>
      </table>

      <Callout type="note">
        <code>val</code> was chosen over <code>const</code> to pair naturally
        with <code>var</code>. This follows Kotlin and Scala convention.
      </Callout>

      <Callout type="tip">
        Prefer <code>val</code> over <code>var</code> when possible. Immutable
        bindings make code easier to reason about and prevent accidental mutation
        bugs.
      </Callout>

      {/* ── Section 4: Unused variables are compile-time errors ── */}
      <h2>Unused variables are compile-time errors</h2>
      <p>
        Declaring a variable and never using it is a compile-time error. This
        also applies to function parameters. jlang follows the Go and Rust
        philosophy of keeping code clean by disallowing dead declarations.
      </p>

      <CodeBlock
        code={`fn main() -> i32 {
    var x: i32 = 10;
    var y: i32 = 20;  // ERROR: unused variable 'y'
    return x;
}`}
      />

      <Callout type="tip">
        If you intentionally want to ignore a value, use a naming convention
        like <code>_unused</code> or refactor the code to not declare the
        variable at all.
      </Callout>
    </>
  )
}
