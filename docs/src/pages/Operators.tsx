import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Operators() {
  return (
    <>
      <h1>Operators</h1>

      {/* ── Section 1: Logical Operators ── */}
      <h2>Logical Operators</h2>
      <p>
        jlang provides the standard logical operators <code>&&</code> (AND),{' '}
        <code>||</code> (OR), and <code>!</code> (NOT). These operators use{' '}
        <strong>short-circuit evaluation</strong>: the right-hand side is only
        evaluated if necessary.
      </p>

      <CodeBlock code={`if (x > 0 && y > 0) {
    printf("Both positive");
}

if (a == 0 || b == 0) {
    printf("At least one is zero");
}

if (!isReady) {
    printf("Not ready yet");
}`} />

      <h3>Short-circuit Examples</h3>
      <p>
        Short-circuit evaluation is useful for guard conditions and null checks:
      </p>

      <CodeBlock code={`// Guard: division is never performed if b is zero
if (b != 0 && a / b > 10) {
    printf("Ratio exceeds 10");
}

// Fallback: loadFromDisk() only runs if hasData is false
if (hasData || loadFromDisk()) {
    printf("Data available");
}`} />

      {/* ── Section 2: Non-short-circuit Operators ── */}
      <h2>Non-short-circuit: <code>and</code> / <code>or</code></h2>
      <p>
        The keyword operators <code>and</code> and <code>or</code> always
        evaluate <strong>both</strong> sides, regardless of the left-hand
        result. Use these when both operands have side effects that must execute.
      </p>

      <CodeBlock code={`// Both validate() and authorize() always run
if (validate() and authorize()) {
    printf("Proceed");
}

// Both logAttempt() and notifyAdmin() always run
if (logAttempt() or notifyAdmin()) {
    printf("Action recorded");
}`} />

      <h3>Comparison</h3>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Operator</th>
            <th>Short-circuit</th>
            <th>Right side evaluated</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>&&</code></td>
            <td>Yes</td>
            <td>Only if left is <code>true</code></td>
          </tr>
          <tr>
            <td><code>||</code></td>
            <td>Yes</td>
            <td>Only if left is <code>false</code></td>
          </tr>
          <tr>
            <td><code>and</code></td>
            <td>No</td>
            <td>Always</td>
          </tr>
          <tr>
            <td><code>or</code></td>
            <td>No</td>
            <td>Always</td>
          </tr>
        </tbody>
      </table>

      {/* ── Section 3: Increment and Decrement ── */}
      <h2>Increment and Decrement</h2>
      <p>
        jlang supports both prefix and postfix increment (<code>++</code>) and
        decrement (<code>--</code>) operators.
      </p>

      <CodeBlock code={`var x: i32 = 5;

x++;    // postfix: x becomes 6
++x;    // prefix:  x becomes 7
x--;    // postfix: x becomes 6
--x;    // prefix:  x becomes 5`} />

      <table className="doc-table">
        <thead>
          <tr>
            <th>Expression</th>
            <th>Value of expression</th>
            <th>Side effect</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>++x</code></td>
            <td>New value (after increment)</td>
            <td>Increments <code>x</code> by 1</td>
          </tr>
          <tr>
            <td><code>x++</code></td>
            <td>Old value (before increment)</td>
            <td>Increments <code>x</code> by 1</td>
          </tr>
          <tr>
            <td><code>--x</code></td>
            <td>New value (after decrement)</td>
            <td>Decrements <code>x</code> by 1</td>
          </tr>
          <tr>
            <td><code>x--</code></td>
            <td>Old value (before decrement)</td>
            <td>Decrements <code>x</code> by 1</td>
          </tr>
        </tbody>
      </table>

      <Callout type="note">
        There is no performance difference between prefix and postfix increment
        or decrement in jlang. Unlike C++ iterators, where postfix may create a
        temporary copy, jlang treats both forms identically in terms of
        performance. Choose whichever reads better in context.
      </Callout>

      {/* ── Section 4: Bitwise Operators ── */}
      <h2>Bitwise Operators</h2>
      <p>
        jlang provides the full set of bitwise operators for working with
        individual bits in integer values.
      </p>

      <CodeBlock code={`var a: i32 = 0b1100;  // 12
var b: i32 = 0b1010;  // 10

var bitwiseAnd := a & b;   // 0b1000 = 8
var bitwiseOr  := a | b;   // 0b1110 = 14
var bitwiseXor := a ^ b;   // 0b0110 = 6
var complement := ~a;      // bitwise NOT
var leftShift  := a << 2;  // 0b110000 = 48
var rightShift := a >> 1;  // 0b0110 = 6`} />

      <h3>Compound Assignment</h3>
      <p>
        All bitwise operators have compound assignment forms:
      </p>

      <CodeBlock code={`var flags: i32 = 0;

flags |= 0b0001;   // set bit 0
flags |= 0b0100;   // set bit 2
flags &= ~0b0001;  // clear bit 0
flags ^= 0b0100;   // toggle bit 2
flags <<= 1;       // shift all bits left
flags >>= 1;       // shift all bits right`} />

      <h3>XOR Swap</h3>
      <p>
        A classic trick using XOR to swap two values without a temporary
        variable:
      </p>

      <CodeBlock code={`var x: i32 = 10;
var y: i32 = 20;

x ^= y;
y ^= x;
x ^= y;

// x is now 20, y is now 10`} />

      <Callout type="note">
        The XOR swap trick is a well-known technique, but on modern hardware it
        is typically <strong>slower</strong> than using a temporary variable. The
        CPU pipeline stalls on the data dependencies between the three XOR
        operations. Prefer a straightforward temp-variable swap for clarity and
        performance.
      </Callout>

      {/* ── Section 5: Semicolons ── */}
      <h2>Semicolons</h2>
      <p>
        Semicolons are <strong>required</strong> in jlang. Every statement must
        end with a semicolon. There is no automatic semicolon insertion.
      </p>

      <CodeBlock code={`var x: i32 = 10;
x = x + 1;
printf("x = %d", x);`} />
    </>
  )
}
