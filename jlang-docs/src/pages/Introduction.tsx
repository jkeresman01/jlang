import CodeBlock from '../components/CodeBlock';
import Callout from '../components/Callout';

export default function Introduction() {
  return (
    <>
      <section className="hero">
        <h1>jlang</h1>
        <p className="tagline">
          Journey Through a 25-Year-Old&rsquo;s Thoughts, Bundled with a
          Language
        </p>
        <p className="sub-tagline">
          A procedural programming language mostly inspired by Go/Rust &mdash;
          clean syntax, explicit memory management, and modern language features
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

      <h2>A Quick Taste</h2>
      <p>Every language starts the same way:</p>
      <CodeBlock
        code={`fn main() -> i32 {
    printf("Hello, World!\\n");
    return 0;
}`}
      />

      <Callout type="note">
        <strong>Fun fact:</strong> The tradition of using &ldquo;Hello,
        World!&rdquo; as the first example program was started by Brian
        Kernighan in <em>The C Programming Language</em> (1978), co-authored
        with Dennis Ritchie. Every language since has followed suit, and at this
        point it would feel wrong not to.
      </Callout>

      <p>
        Here is something a bit more involved &mdash; finding all prime numbers
        up to a given limit using the Sieve of Eratosthenes:
      </p>
      <CodeBlock
        code={`fn main() -> i32 {
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
}`}
      />

      <h2>Why Procedural?</h2>
      <p>
        jlang is a <strong>procedural</strong> language &mdash; no classes, no
        inheritance, no method overloading. Just functions, structs, and data
        you control. Languages like Go and Rust have shown that you can build
        serious, large-scale software without classical OOP, and jlang follows
        that same spirit: keep it simple, keep it explicit.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> Most people call C++ an object-oriented
        language, but Bjarne Stroustrup himself disagrees. He has repeatedly
        said that C++ is a <em>multi-paradigm</em> language and that forcing
        everything into classes is a misuse of it. Here he is, making exactly
        that point:{' '}
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
        the scenes, no decisions made on your behalf, no runtime quietly
        managing your memory. jlang is built on a simple belief &mdash;{' '}
        <strong>
          if you wrote it, you should understand exactly what it does
        </strong>
        . Every allocation is yours to make, every free is yours to call, and
        every line of code means precisely what it says.
      </p>
      <p>
        Writing in jlang is an act of craftsmanship. The language asks something
        of you &mdash; <strong>discipline</strong>. Every byte you allocate is a
        promise you make to eventually return it. Every variable you declare is
        a statement of intent: that this name matters, that it will be used,
        that it earns its place on the stack. There is no room for clutter, no
        tolerance for loose ends. A declared variable that serves no purpose is
        not just waste &mdash; it is a lie in your code, a name given to
        nothing. jlang believes that good software is built by programmers who
        care about every line they write. At its core, jlang is about{' '}
        <strong>understanding</strong>. But do you really know the difference
        between understanding what your code does and merely following a stack
        of recipes until something compiles?
      </p>

      <Callout type="note">
        <strong>Understanding vs. performing</strong>
        <br />
        <br />
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
        <strong>
          Every language has a philosophy &mdash; and they rarely agree.
        </strong>
        <br />
        <br />
        jlang says: <em>you own your memory, you own your mistakes</em>. C++
        says:{' '}
        <em>
          you can do anything, but we won&rsquo;t stop you from shooting
          yourself in the foot
        </em>
        . Rust says:{' '}
        <em>
          we will stop you from shooting yourself in the foot, even if you
          really want to
        </em>
        . Go says: <em>here are 3 ways to do it, pick one and move on</em>. And
        then there is Erlang, which takes a completely different stance:{' '}
        <strong>let it crash</strong>. Instead of trying to prevent every
        possible failure, Erlang assumes things <em>will</em> go wrong and
        builds supervision trees that detect crashes and restart processes
        automatically. The philosophy is not about writing code that never fails
        &mdash; it is about writing systems that recover gracefully when it
        does.
        <br />
        <br />
        For a deeper dive into Erlang&rsquo;s philosophy:
        <br />
        <br />
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

      <h2>Why the Name &ldquo;jlang&rdquo;?</h2>
      <p>
        Most of my personal projects get a &ldquo;j&rdquo; prefixed to them. Not
        because it stands for something profound &mdash; it does not. It is just
        a reflex at this point, the way some people always park in the same spot
        or always order the same coffee. I needed a name, I had a keyboard in
        front of me, and the letter was right there. The language got its name
        in about two seconds, and the compiler took considerably longer.
      </p>

      <h2>Compiled, Not Interpreted</h2>
      <p>
        If you have ever used Python, Ruby, or JavaScript, you are used to
        languages where your code is read and executed line by line at runtime
        &mdash; that is interpretation. Java and C# take a middle road: they
        are both compiled <em>and</em> interpreted. Java compiles your source
        into bytecode that runs on the JVM, and C# compiles to bytecode that
        runs on the CLR. In both cases, the bytecode is then interpreted or
        JIT-compiled at runtime by the virtual machine. jlang does neither. It is a <strong>compiled language</strong> &mdash; your
        source code goes through scanning, parsing, and semantic analysis, then
        gets translated into LLVM Intermediate Representation, and finally LLVM
        turns that IR into native machine code for your platform. The result is
        a real, standalone executable &mdash; no interpreter, no VM, no runtime
        sitting between your code and the hardware.
      </p>
      <p>
        This puts jlang in the same category as C, C++, Go, and Rust. Your
        program runs at native speed, benefits from LLVM&rsquo;s battle-tested
        optimization passes, and can be distributed as a single binary with no
        dependencies. When you run <code>./output</code>, there is nothing
        between you and the metal.
      </p>

      <Callout type="note">
        <strong>
          Chris Lattner &mdash; &ldquo;What is a compiler?&rdquo;
        </strong>
        <br />
        <br />
        Chris Lattner, the creator of LLVM and Clang, gives a high-level
        explanation of what a compiler actually is.
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/yCd3CzGSte8?start=2120"
          title="Chris Lattner — What is a compiler?"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <h2>Why Implement a Language from Scratch?</h2>
      <p>
        Long distance runners sometimes train with weights strapped to their
        ankles or at high altitudes where the atmosphere is thin. When they
        later unburden themselves, the new relative ease of light limbs and
        oxygen-rich air enables them to run farther and faster.
      </p>
      <p>
        Implementing a language is a real test of programming skill. The code is
        complex and performance critical. You must master recursion, dynamic
        arrays, trees, graphs, and hash tables. You probably use hash tables at
        least in your day-to-day programming, but do you really understand them?
        Well, after we&rsquo;ve crafted our own from scratch, I guarantee you
        will.
      </p>
      <p>
        While I intend to show you that an interpreter isn&rsquo;t as daunting
        as you might believe, implementing one well is still a challenge. Rise
        to it, and you&rsquo;ll come away a stronger programmer, and smarter
        about how you use data structures and algorithms in your day job.
      </p>
      <p style={{ fontSize: '0.9rem', fontStyle: 'italic', opacity: 0.7 }}>
        &mdash; Robert Nystrom, <em>Crafting Interpreters</em>
      </p>

      <p>Muhammad Ali said it better than anyone:</p>
      <blockquote
        style={{
          borderLeft: '3px solid var(--border)',
          paddingLeft: '1rem',
          margin: '1.5rem 0',
          fontStyle: 'italic',
          opacity: 0.85,
        }}
      >
        I hated every minute of training, but I said, &ldquo;Don&rsquo;t quit.
        Suffer now and live the rest of your life as a champion.&rdquo;
        <br />
        <span style={{ fontStyle: 'normal', opacity: 0.7 }}>
          &mdash; Muhammad Ali
        </span>
      </blockquote>
      <p>
        That is exactly what implementing a language from scratch feels like.
        Every moment you spend wrestling with symbol tables, type resolution,
        and code generation is a weight strapped to your ankle. But when you
        finally take it off &mdash; when you go back to writing everyday code
        &mdash; you see everything differently. The struggle is the point.
      </p>
    </>
  );
}
