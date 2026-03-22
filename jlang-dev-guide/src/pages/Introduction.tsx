import Callout from '../components/Callout';
import CodeBlock from '../components/CodeBlock';

export default function Introduction() {
  return (
    <>
      <section className="hero">
        <h1>Jlang Dev Guide</h1>
        <p className="tagline">
          A Developer&rsquo;s Guide to the Jlang Compiler Internals
        </p>
        <p className="sub-tagline">
          How the compiler works under the hood &mdash; from scanning source
          code to emitting native executables via LLVM.
        </p>
        <div className="hero-badges">
          <span className="hero-badge">C++</span>
          <span className="hero-badge">LLVM</span>
          <span className="hero-badge">CMake</span>
        </div>
      </section>

      <Callout type="note">
        This guide is a living document that evolves alongside the compiler.
        Sections may be incomplete or out of date as the codebase changes.
      </Callout>

      <h2>What is this?</h2>
      <p>
        This is the developer guide for <strong>jlang</strong> &mdash; a
        procedural programming language compiled to native code via LLVM. If the{' '}
        <a href="https://jkeresman01.github.io/jlang/">language docs</a> teach
        you how to <em>write</em> jlang, this guide teaches you how the compiler{' '}
        <em>reads, understands, and translates</em> it.
      </p>
      <p>
        Whether you want to fix a bug, add a language feature, or simply
        understand how a real compiler is put together, this is the place to
        start.
      </p>

      <h2>High-level Architecture</h2>
      <p>
        The compiler follows a classic pipeline. Source code flows through these
        stages:
      </p>
      <ol>
        <li>
          <strong>Preprocessor</strong> &mdash; handles <code>#include</code>{' '}
          and <code>#define</code> directives
        </li>
        <li>
          <strong>Scanner</strong> &mdash; breaks source text into tokens
        </li>
        <li>
          <strong>Parser</strong> &mdash; builds an Abstract Syntax Tree (AST)
          from the token stream
        </li>
        <li>
          <strong>Semantic Analyzer</strong> &mdash; validates types, resolves
          symbols, checks correctness
        </li>
        <li>
          <strong>Code Generator</strong> &mdash; walks the AST and emits LLVM
          IR in multiple passes
        </li>
        <li>
          <strong>LLVM Backend</strong> &mdash; optimizes the IR and produces a
          native executable
        </li>
      </ol>

      <h2>Repository Layout</h2>
      <p>The project is organized as a CMake monorepo with these modules:</p>
      <CodeBlock
        language="jlang"
        code={`// Module structure
jlang-shared/          // INTERFACE header-only lib (Assert.h, StringUtils.h)
jlang-preprocessor/    // STATIC lib, depends on jlang-shared
jlang-core/            // STATIC lib + Jlang executable + tests
jlang-stl/             // C runtime for std::Vector, linked into compiled programs
jlang-examples/        // Categorized .j sample files
jlang-docs/            // Language documentation (Vite + React)
jlang-dev-guide/       // This guide (Vite + React)`}
      />

      <h2>Building & Running</h2>
      <CodeBlock
        language="jlang"
        code={`// Build the compiler
cmake -B build && cmake --build build

// Run a jlang program
./build/jlang-core/Jlang jlang-examples/basic/simple.j -o output && ./output

// Run the test suite
./build/jlang-core/jlang-core-tests

// Useful flags
./build/jlang-core/Jlang program.j --dump-ast    // Print the AST
./build/jlang-core/Jlang program.j --emit-ir     // Print LLVM IR`}
      />

      <Callout type="tip">
        Use <code>--dump-ast</code> and <code>--emit-ir</code> liberally when
        working on the compiler. They are your best debugging tools.
      </Callout>
    </>
  );
}
