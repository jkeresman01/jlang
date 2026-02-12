import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function GettingStarted() {
  return (
    <>
      <h1>Getting Started</h1>

      {/* ── Section 1: Prerequisites ── */}
      <h2>Prerequisites</h2>
      <p>
        jlang requires <strong>LLVM</strong> to be installed on your system. The
        compiler uses LLVM as its backend for code generation and optimization.
      </p>

      <h3>Ubuntu / Debian</h3>
      <CodeBlock language="bash" code={`sudo apt install llvm-dev`} />

      <h3>Fedora</h3>
      <CodeBlock language="bash" code={`sudo dnf install llvm-devel`} />

      <h3>macOS</h3>
      <CodeBlock language="bash" code={`brew install llvm`} />

      {/* ── Section 2: Building from Source ── */}
      <h2>Building from Source</h2>
      <p>
        Clone the repository and build the compiler using CMake:
      </p>

      <CodeBlock language="bash" code={`mkdir -p build && cd build && cmake .. && make`} />

      <Callout type="tip">
        You can also use the CMake one-liner that avoids changing directories:
      </Callout>

      <CodeBlock language="bash" code={`cmake -B build && cmake --build build`} />

      {/* ── Section 3: Running a Program ── */}
      <h2>Running a Program</h2>
      <p>
        Once built, pass a <code>.j</code> source file to the compiler:
      </p>

      <CodeBlock language="bash" code={`./build/Jlang samples/sample.j`} />

      <p>
        This compiles and runs the specified jlang source file. You can find
        example programs in the <code>samples/</code> directory to get a feel
        for the language.
      </p>
    </>
  )
}
