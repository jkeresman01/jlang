import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Introduction() {
  return (
    <>
      <section className="hero">
        <h1>jlang</h1>
        <p className="tagline">
          A procedural programming language mostly inspired by Go/Rust
        </p>
        <p className="sub-tagline">
          Clean syntax, explicit memory management, and modern language features
          compiled to native code via LLVM.
        </p>
        <div className="hero-badges">
          <span className="hero-badge">C++</span>
          <span className="hero-badge">LLVM</span>
        </div>
      </section>

      <Callout type="note">
        This project is still very much a work in progress. Syntax and features
        may change.
      </Callout>

      <h2>Syntax Design Decisions</h2>
      <p>
        jlang aims to be a language that feels familiar to developers coming
        from C, Go, or Rust while introducing its own conventions for clarity
        and safety. The syntax is intentionally minimal &mdash; there are no hidden
        control flows, no implicit conversions, and no garbage collector.
      </p>
      <p>
        Variables are declared with <code>var</code> for mutable bindings and{' '}
        <code>val</code> for immutable ones. Types are written after the name,
        separated by a colon, keeping declarations easy to read at a glance.
        Functions use the <code>fn</code> keyword and specify their return type
        with an arrow (<code>{'->'}</code>).
      </p>
      <p>
        Memory management is explicit. You allocate with <code>alloc</code> and
        free with <code>free</code> &mdash; no runtime overhead, no surprises. This
        gives you full control over when and how resources are acquired and
        released.
      </p>

      <h2>A Quick Taste</h2>
      <p>
        Here is a small program that finds all prime numbers up to a given
        limit using the Sieve of Eratosthenes:
      </p>
      <CodeBlock code={`fn main() -> i32 {
    val limit: i32 = 100;
    var sieve: bool[101];

    for (var i: i32 = 0; i <= limit; i++) {
        sieve[i] = true;
    }

    for (var p: i32 = 2; p * p <= limit; p++) {
        if (sieve[p]) {
            for (var j: i32 = p * p; j <= limit; j += p) {
                sieve[j] = false;
            }
        }
    }

    printf("Primes up to %d:\\n", limit);
    for (var i: i32 = 2; i <= limit; i++) {
        if (sieve[i]) {
            printf("%d ", i);
        }
    }

    return 0;
}`} />
    </>
  )
}
