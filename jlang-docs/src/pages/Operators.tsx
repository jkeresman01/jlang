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

      <Callout type="note">
        <strong>Bentley Rules for Optimizing Work</strong>
        <br /><br />
        Jon Bentley catalogued a set of rules for reducing the work a program does.
        Short-circuiting &mdash; the idea behind <code>&&</code> and <code>||</code> &mdash; is
        one of them. Here is the full list, organized by category:
        <br /><br />
        <em>Data Structures</em>
        <br />
        1. Packing and Encoding &bull; 2. Augmentation &bull; 3. Precomputation &bull;
        4. Compile-Time Initialization &bull; 5. Caching &bull; 6. Sparsity
        <br /><br />
        <em>Logic</em>
        <br />
        7. Constant Folding and Propagation &bull; 8. Common Subexpression Elimination &bull;
        9. Algebraic Identities &bull; <strong>10. Short-Circuiting</strong> &bull;
        11. Ordering Tests &bull; 12. Creating a Fast Path &bull; 13. Combining Tests
        <br /><br />
        <em>Loops</em>
        <br />
        14. Hoisting &bull; 15. Sentinels &bull; 16. Loop Unrolling &bull;
        17. Loop Fusion &bull; 18. Eliminating Wasted Iterations
        <br /><br />
        <em>Functions</em>
        <br />
        19. Inlining &bull; 20. Tail-Recursion Elimination &bull; 21. Coarsening Recursion
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/H-1-X9bkop8?start=2890"
          title="Bentley Rules for Optimizing Work"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

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

      {/* ── Section 4: Bitwise Operators ── */}
      <h2>Bitwise Operators</h2>
      <blockquote>
        <p>
          &ldquo;The cost of software maintenance increases with the square of the
          programmer&rsquo;s creativity.&rdquo;
        </p>
        <footer>&mdash; Robert D. Bliss, 1992, as quoted in <cite>Hacker&rsquo;s Delight</cite> by Henry S. Warren, Jr.</footer>
      </blockquote>
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

      {/* ── Section 4b: Endianness & Byte Swap ── */}
      <h3>Endian Swap</h3>
      <p>
        When working at the bit level you will inevitably run into <strong>byte
        order</strong>. An endian swap reverses the bytes of a multi-byte integer,
        converting between <em>big-endian</em> (most-significant byte first) and{' '}
        <em>little-endian</em> (least-significant byte first) representations.
      </p>

      <CodeBlock code={`// Swap the 4 bytes of a 32-bit integer
fn swapEndian32(val: i32) -> i32 {
    var b0: i32 = (val >> 24) & 255;        // 0xFF
    var b1: i32 = (val >> 8)  & 65280;      // 0xFF00
    var b2: i32 = (val << 8)  & 16711680;   // 0xFF0000
    var b3: i32 = (val << 24);
    return b0 | b1 | b2 | b3;
}

fn main() -> i32 {
    var x: i32 = 16909060;  // 0x01020304
    var swapped: i32 = swapEndian32(x);
    printf("%d -> %d\\n", x, swapped);
    // prints: 16909060 -> 67305985
    return 0;
}`} />

      <Callout type="important">
        Bitwise code like this is much more readable with hexadecimal
        literals &mdash; e.g. <code>& 0xFF</code> instead
        of <code>& 255</code>. Hex literal support (<code>0x</code> prefix)
        is not yet available in jlang but is planned for a future release.
      </Callout>

      <Callout type="tip">
        <strong>Fun fact &mdash; Gulliver&rsquo;s Eggs</strong>
        <br /><br />
        The terms <em>big-endian</em> and <em>little-endian</em> were coined by
        Danny Cohen in his 1980 paper{' '}
        <em>&ldquo;On Holy Wars and a Plea for Peace&rdquo;</em>, borrowing
        from Jonathan Swift&rsquo;s <em>Gulliver&rsquo;s Travels</em> (1726).
        In the novel, the empires of Lilliput and Blefuscu wage a bloody war
        over which end of a soft-boiled egg you should crack open &mdash; the
        big end or the little end. Cohen drew the parallel to computer
        architects arguing over byte order, suggesting the debate is just as
        absurd. Nearly 300 years after Swift and 45 years after Cohen, we
        still haven&rsquo;t settled it.
        <br /><br />
        <strong>How the swap works, step by step:</strong>
        <br /><br />
        <code style={{ whiteSpace: 'pre', display: 'block', padding: '0.75rem 1rem', background: 'var(--bg-code)', borderRadius: '6px', fontSize: '0.835rem', lineHeight: '1.65' }}>
{`Original (big-endian):    0x01 02 03 04
                          [B3] [B2] [B1] [B0]

(val >> 24) & 0xFF          ->  0x00 00 00 01  (B3 moves to B0)
(val >>  8) & 0xFF00        ->  0x00 00 02 00  (B2 moves to B1)
(val <<  8) & 0xFF0000      ->  0x00 03 00 00  (B1 moves to B2)
(val << 24) & 0xFF000000    ->  0x04 00 00 00  (B0 moves to B3)
                               ──────────────
OR them together:              0x04 03 02 01`}
        </code>
        <br />
        Each shift isolates one byte and moves it to its mirrored position;
        the OR combines all four into the swapped result.
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

      {/* ── Puzzle ── */}
      <h2>Puzzle: The Mysterious Output</h2>
      <p>
        Your colleague wrote the following function and swears the output
        is <code>42</code>. Without running the code, what does it actually print?
      </p>

      <CodeBlock code={`fn main() -> i32 {
    var a: i32 = 10;
    var b: i32 = 20;
    var c: i32 = a++ + ++b + a-- - --b;

    printf("%d\\n", c);
    return 0;
}`} />

      <details className="details-block">
        <summary>Answer #1 &mdash; The Overthinker</summary>
        <div className="details-content">
          <p>
            Let me trace through every side effect in evaluation order&hellip;
          </p>
          <CodeBlock code={`// a starts at 10, b starts at 20
// a++ -> yields 10, a becomes 11
// ++b -> b becomes 21, yields 21
// a-- -> yields 11, a becomes 10
// --b -> b becomes 20, yields 20
// c = 10 + 21 + 11 - 20 = 22

printf("%d\\n", c);  // prints 22`} />
        </div>
      </details>

      <details className="details-block">
        <summary>Answer #2 &mdash; The Language Lawyer</summary>
        <div className="details-content">
          <p>
            Actually, modifying and reading the same variable multiple times in
            one expression without a sequence point is <strong>undefined behavior</strong> in
            C/C++. The result could be anything &mdash; 22, 42, or your program
            ordering pizza. The compiler is under no obligation to evaluate
            sub-expressions in any particular order.
          </p>
          <CodeBlock code={`// The C++ standard says:
// "If a side effect on a scalar object is
//  unsequenced relative to another side effect
//  on the same scalar object, the behavior
//  is undefined."
//
// So the answer is: nobody knows.`} />
        </div>
      </details>

      <details className="details-block">
        <summary>The Correct Answer</summary>
        <div className="details-content">
          <p>
            You ask your colleague to rewrite it. Code that requires a
            whiteboard session to figure out what it prints has no business
            being in a codebase. The correct output is a cleaner version:
          </p>
          <CodeBlock code={`fn main() -> i32 {
    var a: i32 = 10;
    var b: i32 = 20;

    // Just say what you mean
    val sum: i32 = a + b + 1 + (a + 1) - b;
    printf("%d\\n", sum);  // 22, obviously

    return 0;
}`} />
        </div>
      </details>
    </>
  )
}
