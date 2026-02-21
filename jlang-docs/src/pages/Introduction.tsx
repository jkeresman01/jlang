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

      <h2>About This Document</h2>
      <p>
        This is less of a traditional language reference and more of a small book.
        Beyond the usual syntax and semantics, you will find a collection of useful
        materials, cool tricks, and insights gathered from a wide range of educational
        resources over the years. Sprinkled between the chapters are fun facts, video
        deep-dives, and programming challenges that are meant to keep things interesting
        and maybe teach you something unexpected along the way.
      </p>
      <p>
        Many of these materials I found extremely valuable &mdash; and increasingly rare.
        In an era of vibe-coding, where shipping fast matters more than understanding
        deeply, we have quietly forgotten the core concepts, the foundational books, and
        the hard-won lessons that this craft was built on. Some of the videos featured here
        contain words and thoughts from people like Ken Thompson, Dennis Ritchie, and
        Bjarne Stroustrup &mdash; programmers whose work we all stand on, whether we
        realize it or not.
      </p>
      <p>
        Of course, calling this a &ldquo;book&rdquo; or a &ldquo;reference&rdquo; is
        generous &mdash; for that to be true, it would need at least one reader. There are
        far better authors and far better books that deserve your time first. Consider this
        more of a curated collection of things I wish someone had shown me earlier,
        bundled alongside a language that gave me an excuse to put them all in one place.
      </p>
      <p>
        We call them <em>funny</em> challenges for a reason &mdash; not because they are
        jokes, but because each one can be solved in a completely different way depending
        on your background. A mathematician will reach for a formula, a systems programmer
        will reach for bitwise tricks, and an engineer will wonder why you are not just
        checking the logs. Everyone is convinced their approach is the right one, and
        they will defend it stubbornly. That is exactly what makes them fun.
      </p>
      <Callout type="tip">
        <strong>Fun fact:</strong> Some of these challenges are also known to show up in
        technical interviews &mdash; the kind of LeetCode-style questions that catch you off
        guard at a whiteboard at various IT companies. A few of them I remember being asked
        myself. Array rotation was one. The Sieve of Eratosthenes was another &mdash; and I
        still remember how unreasonably long it took me to work through it when I was just
        starting out. The interviewer was patient enough to walk me through the algorithm
        afterward, and that conversation taught me more than any textbook explanation could
        have. Those moments tend to stick with you.
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

      <Callout type="note">
        <strong>Every language has a philosophy &mdash; and they rarely agree.</strong>
        <br /><br />
        jlang says: <em>you own your memory, you own your mistakes</em>.
        C++ says: <em>you can do anything, but we won&rsquo;t stop you from shooting
        yourself in the foot</em>. Rust says: <em>we will stop you from shooting yourself
        in the foot, even if you really want to</em>. Go says: <em>here are 3 ways to do
        it, pick one and move on</em>. And then there is Erlang, which takes a completely
        different stance: <strong>let it crash</strong>. Instead of trying to prevent every
        possible failure, Erlang assumes things <em>will</em> go wrong and builds
        supervision trees that detect crashes and restart processes automatically. The
        philosophy is not about writing code that never fails &mdash; it is about writing
        systems that recover gracefully when it does.
        <br /><br />
        For a deeper dive into Erlang&rsquo;s philosophy:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/SOqQVoVai6s"
          title="Erlang - Let it crash philosophy"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <h2>A Quick Taste</h2>
      <p>
        Every language starts the same way:
      </p>
      <CodeBlock code={`fn main() -> i32 {
    printf("Hello, World!\\n");
    return 0;
}`} />

      <Callout type="note">
        <strong>Fun fact:</strong> The tradition of using &ldquo;Hello, World!&rdquo; as the
        first example program was started by Brian Kernighan in{' '}
        <em>The C Programming Language</em> (1978), co-authored with Dennis Ritchie. Every
        language since has followed suit, and at this point it would feel wrong not to.
      </Callout>

      <p>
        Here is something a bit more involved &mdash; finding all prime numbers up to a given
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
        <strong>The <code>inline</code> keyword in C++ doesn't guarantee inlining</strong>
        <br /><br />
        A common misconception is that marking a function <code>inline</code> in C++ guarantees
        the compiler will inline it. In reality, <code>inline</code> is just a hint &mdash; the
        compiler is free to ignore it. Modern compilers decide on their own whether to inline
        based on heuristics like function size, call frequency, and optimization level.
        <br /><br />
        For a deeper dive, check out the explanation at 55:30:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/ulJm7_aTiQM?start=3330"
          title="Inline keyword in C++ explained"
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
