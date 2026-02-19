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

      <Callout type="warning">
        jlang is in its early stages &mdash; things may not work as expected or
        may be outright broken. As the language evolves, parts of this
        documentation can fall out of date or become inaccurate. If something
        looks off, it probably is. Corrections and feedback are always welcome.
      </Callout>

      <h2>Why Procedural?</h2>
      <p>
        jlang is a <strong>procedural</strong> language &mdash; no classes, no inheritance,
        no method overloading. Just functions, structs, and data you control. Languages
        like Go and Rust have shown that you can build serious, large-scale software
        without classical OOP, and jlang follows that same spirit: keep it simple,
        keep it explicit.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> Most people call C++ an object-oriented language, but
        Bjarne Stroustrup himself disagrees. He has repeatedly said that C++ is a
        <em>multi-paradigm</em> language and that forcing everything into classes is
        a misuse of it. Here he is, making exactly that point:
        {' '}
        <a
          href="https://www.youtube.com/watch?v=uTxRF5ag27A&t=2550"
          target="_blank"
          rel="noopener noreferrer"
        >
          Watch on YouTube (42:30)
        </a>
      </Callout>

      <h2>Jlang Philosophy</h2>
      <p>
        A programming language should respect the programmer. No magic behind
        the scenes, no decisions made on your behalf, no runtime quietly managing
        your memory. jlang is built on a simple belief &mdash; <strong>if you wrote it,
        you should understand exactly what it does</strong>. Every allocation is yours to
        make, every free is yours to call, and every line of code means precisely
        what it says.
      </p>
      <p>
        Writing in jlang is an act of craftsmanship. The language asks something
        of you &mdash; <strong>discipline</strong>. Every byte you allocate is a
        promise you make to eventually return it. Every variable you declare is a
        statement of intent: that this name matters, that it will be used, that it
        earns its place on the stack. There is no room for clutter, no tolerance
        for loose ends. A declared variable that serves no purpose is not just
        waste &mdash; it is a lie in your code, a name given to nothing. jlang
        believes that good software is built by programmers who care about every
        line they write. At its core, jlang is about <strong>understanding</strong>.
        But do you really know the difference between understanding what your code
        does and merely following a stack of recipes until something compiles?
      </p>

      <Callout type="note">
        <strong>Understanding vs. performing</strong>
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/Nb2tebYAaOA?start=1373"
          title="Understanding vs. performing"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

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

      <h2>About This Document</h2>
      <Callout type="tip">
        Throughout this documentation you will find small programming puzzles,
        fun facts, and video materials sprinkled between the chapters. Let's be
        honest &mdash; language docs are usually painfully dry, so we added these
        to keep things interesting and maybe teach you something unexpected
        along the way.
      </Callout>
      <p>
        Here are a few examples of how this might look:
      </p>

      <h2>Funny Lottery Example</h2>
      <p>
        You are running a lottery draw: <strong>7 out of 27</strong> balls.
        After drawing 6 balls, one ball accidentally jumps out of the machine
        and rolls under a table. You still have the remaining 20 balls in the
        machine, so you know their numbers. How do you figure out which ball
        escaped?
      </p>

      <details className="details-block">
        <summary>Funny Answer #1 &mdash; The Mathematician</summary>
        <div className="details-content">
          <p>
            Sum all numbers from 1 to 27, subtract the 6 drawn balls, subtract
            the 20 remaining balls. Whatever is left is the missing ball.
          </p>
          <CodeBlock code={`fn find_missing_ball(drawn: i32[6], remaining: i32[20]) -> i32 {
    // Sum of 1..27 = 27 * 28 / 2 = 378
    var total: i32 = 378;

    for (var i: i32 = 0; i < 6; i++) {
        total -= drawn[i];
    }

    for (var i: i32 = 0; i < 20; i++) {
        total -= remaining[i];
    }

    return total;
}`} />
        </div>
      </details>

      <details className="details-block">
        <summary>Funny Answer #2 &mdash; The Programmer</summary>
        <div className="details-content">
          <p>
            XOR all numbers from 1 to 27, then XOR with every drawn and
            remaining ball. Since <code>x ^ x = 0</code>, all known balls
            cancel out and only the missing one survives.
          </p>
          <CodeBlock code={`fn find_missing_ball(drawn: i32[6], remaining: i32[20]) -> i32 {
    var result: i32 = 0;

    for (var i: i32 = 1; i <= 27; i++) {
        result = result ^ i;
    }

    for (var i: i32 = 0; i < 6; i++) {
        result = result ^ drawn[i];
    }

    for (var i: i32 = 0; i < 20; i++) {
        result = result ^ remaining[i];
    }

    return result;
}`} />
        </div>
      </details>

      <details className="details-block">
        <summary>The Correct Answer</summary>
        <div className="details-content">
          <p>
            You check the camera footage. Every lottery draw is recorded and
            supervised by officials. The escaped ball's number was documented
            the moment it left the machine. No code required.
          </p>
          <CodeBlock code={`fn find_missing_ball() -> i32 {
    // Ask the supervisor
    // Check the camera footage
    // Read the official protocol

    printf("Just check the recording.\\n");
    return 0;
}`} />
        </div>
      </details>

      <h2>Fun Facts</h2>

      <Callout type="note">
        <strong>XOR swap trick</strong>
        <br /><br />
        A classic use of XOR is swapping two variables without a temporary:
        <br /><br />
        <code>a ^= b; b ^= a; a ^= b;</code>
        <br /><br />
        The XOR swap creates a serial data dependency chain &mdash; each step reads the
        result of the previous one, which prevents the CPU from using instruction-level
        parallelism. A straightforward temp-variable swap (<code>var tmp := a; a = b; b = tmp;</code>)
        allows the two loads to execute in parallel and is actually faster on modern
        out-of-order hardware. The XOR trick is a neat bit of trivia, not a performance
        optimization.
        <br /><br />
        For a deeper dive, check out the explanation at 21:00:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/ZusiKXcz_ac?start=1260"
          title="XOR swap explained"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="note">
        <strong>No performance difference between prefix and postfix increment</strong>
        <br /><br />
        Unlike C++ iterators, there is no performance benefit to using <code>++x</code> over{' '}
        <code>x++</code> in jlang. Both generate the same three operations (load, add, store),
        and the only difference is which already-computed register value gets returned.
        No temporary copy is created.
        <br /><br />
        The &quot;prefer <code>++i</code> over <code>i++</code>&quot; advice comes from C++
        where postfix on complex objects (like iterators) requires constructing a temporary copy:
        <br /><br />
        <code style={{ whiteSpace: 'pre', display: 'block', padding: '0.75rem 1rem', background: 'var(--bg-code)', borderRadius: '6px', fontSize: '0.835rem', lineHeight: '1.65' }}>
{`// C++ iterator postfix - expensive!
Iterator operator++(int) {
    Iterator copy = *this;  // make a copy
    ++(*this);              // increment original
    return copy;            // return the copy
}`}
        </code>
        <br />
        For primitive types, modern compilers optimize both to identical machine code.
        Choose based on semantics, not performance.
      </Callout>
    </>
  )
}
