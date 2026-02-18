import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Arrays() {
  return (
    <>
      <h1>Arrays</h1>

      <blockquote>
        <p>
          "There are only two hard things in Computer Science: cache invalidation,
          naming things, and off-by-one errors."
        </p>
        <footer>— Every programmer, at some point</footer>
      </blockquote>

      <Callout type="note">
        <strong>Bonus &mdash; Dijkstra: "Programmer? No such profession."</strong>
        <br /><br />
        In 1957, Edsger Dijkstra got married. Dutch marriage rites required him
        to state his profession. He said he was a programmer. The municipal
        authorities of Amsterdam refused &mdash; there was no such profession.
        His marriage certificate instead lists him as a{' '}
        <strong>theoretical physicist</strong>.
        <br /><br />
        In his own words: <em>"Believe it or not, but under the heading
        'profession' my marriage act shows the ridiculous entry 'theoretical
        physicist'!"</em>
        <br /><br />
        This story is told by Dijkstra himself in his 1972 Turing Award
        lecture, <em>"The Humble Programmer"</em> (EWD 340), and is retold
        by Robert C. Martin in <em>Clean Architecture: A Craftsman's Guide
        to Software Structure and Design</em>, Chapter 4
        &mdash; "Structured Programming".
      </Callout>

      <Callout type="note">
        <strong>Fun Fact &mdash; Why do arrays start at 0?</strong>
        <br /><br />
        In 1982, Edsger Dijkstra wrote a memo titled{' '}
        <em>"Why numbering should start at zero"</em>, arguing that zero-based
        indexing is the most natural convention for half-open ranges. But the
        real reason? In C, <code>arr[i]</code> is literally{' '}
        <code>*(arr + i)</code> &mdash; pointer arithmetic. The first element
        is at offset 0 from the base address. It's not philosophy, it's just
        math. Every language that inherited C's memory model inherited its
        indexing. Including this one.
        <br /><br />
        Dijkstra's memo is worth reading &mdash; it's only one page and
        he handwrote it, because of course he did.
      </Callout>

      {/* ── Section 1: Stack-allocated Arrays ── */}
      <h2>Stack-allocated Arrays</h2>
      <p>
        Arrays are declared by appending the size in square brackets to the
        element type. Stack-allocated arrays live on the stack and are
        automatically cleaned up when they go out of scope.
      </p>

      <CodeBlock code={`var arr: i32[5] = [10, 20, 30, 40, 50];`} />

      <h3>Immutable Arrays</h3>
      <p>
        Use <code>val</code> to declare an immutable array. Neither the array
        variable nor its elements can be modified after initialization.
      </p>

      <CodeBlock code={`val constants: i32[3] = [1, 2, 3];
// constants[0] = 99;  // ERROR: cannot modify element of immutable array`} />

      <h3>Type Inference</h3>
      <p>
        The walrus operator <code>:=</code> infers both the element type and the
        array size from the initializer:
      </p>

      <CodeBlock code={`var nums := [1, 2, 3];  // inferred as i32[3]`} />

      {/* ── Section 2: Indexing ── */}
      <h2>Indexing</h2>
      <p>
        Array elements are accessed and modified using zero-based indexing with
        square brackets.
      </p>

      <CodeBlock code={`var arr: i32[5] = [10, 20, 30, 40, 50];

// Reading elements
var first := arr[0];   // 10
var third := arr[2];   // 30

// Writing elements
arr[1] = 99;
printf("arr[1] = %d", arr[1]);  // arr[1] = 99`} />

      {/* ── Section 3: Heap-allocated Arrays ── */}
      <h2>Heap-allocated Arrays</h2>
      <p>
        For arrays that need to outlive the current scope or whose size is too
        large for the stack, allocate on the heap with <code>alloc</code>:
      </p>

      <CodeBlock code={`var heap: i32[10]* = alloc<i32[10]>();

heap[0] = 100;
heap[9] = 999;

printf("First: %d, Last: %d", heap[0], heap[9]);

free(heap);`} />

      {/* ── Section 4: Immutability Applies to Elements ── */}
      <h2>Immutability Applies to Elements</h2>
      <p>
        When an array is declared with <code>val</code>, immutability extends
        to the individual elements. You cannot modify any element of an
        immutable array.
      </p>

      <CodeBlock code={`val scores: i32[3] = [90, 85, 92];

// scores[0] = 100;  // ERROR: cannot modify element of immutable array
// scores = [1,2,3]; // ERROR: cannot assign to immutable variable 'scores'`} />

      {/* ── Section 5: Comparison Table ── */}
      <h2>Array Types</h2>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Syntax</th>
            <th>Storage</th>
            <th>Lifetime</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>Type[size]</code></td>
            <td>Stack</td>
            <td>Current scope</td>
          </tr>
          <tr>
            <td><code>Type[size]*</code></td>
            <td>Heap</td>
            <td>Until <code>free()</code> is called</td>
          </tr>
        </tbody>
      </table>

      <Callout type="note">
        The <code>Type[size]</code> syntax makes the size part of the type
        itself. An <code>i32[3]</code> and an <code>i32[5]</code> are distinct
        types. This is similar to how arrays work in C and Rust, where the size
        is encoded in the type for compile-time bounds checking.
      </Callout>

      {/* ── Funny Example ── */}
      <h2>Funny Treasure Map Example</h2>
      <p>
        You're a pirate. You have a treasure map encoded as an array of 10
        distances (in paces) &mdash; one for each island you must visit in
        order. Unfortunately, your parrot sat on the map and smudged one entry.
        You know the <strong>total journey is exactly 1000 paces</strong> and
        you can read the other 9 distances. Find the missing distance.
      </p>

      <details className="details-block">
        <summary>Funny Answer #1 &mdash; The Mathematician</summary>
        <div className="details-content">
          <p>
            Sum the 9 known distances, subtract from 1000. Done. O(n), one pass,
            no wasted steps. Optimal and elegant.
          </p>
          <CodeBlock code={`fn find_missing(distances: i32[10], smudged_idx: i32) -> i32 {
    val total: i32 = 1000;
    var sum: i32 = 0;

    for (var i: i32 = 0; i < 10; i++) {
        if (i != smudged_idx) {
            sum += distances[i];
        }
    }

    return total - sum;
}`} />
        </div>
      </details>

      <details className="details-block">
        <summary>Funny Answer #2 &mdash; The Programmer</summary>
        <div className="details-content">
          <p>
            Same subtraction trick, but with <em>production-grade</em>{' '}
            paranoia: validate every entry, handle negative distances (you never
            know with pirates), and add extensive logging for the captain.
          </p>
          <CodeBlock code={`fn find_missing(distances: i32[10], smudged_idx: i32) -> i32 {
    val total: i32 = 1000;
    var sum: i32 = 0;
    var suspicious_islands: i32 = 0;

    for (var i: i32 = 0; i < 10; i++) {
        if (i == smudged_idx) {
            continue;  // skip the parrot damage
        }

        if (distances[i] < 0) {
            // Negative distance? Is this island in another dimension?
            printf("WARNING: island %d has negative distance %d\\n", i, distances[i]);
            suspicious_islands++;
        }

        if (distances[i] > 500) {
            // That's suspiciously far for one leg
            printf("AUDIT: island %d seems far (%d paces)\\n", i, distances[i]);
        }

        sum += distances[i];
    }

    val missing: i32 = total - sum;

    if (suspicious_islands > 0) {
        printf("Found %d suspicious islands. Trust issues remain.\\n",
               suspicious_islands);
    }

    printf("Missing distance: %d paces\\n", missing);
    printf("Confidence level: mass of the parrot squared\\n");

    return missing;
}`} />
        </div>
      </details>

      <details className="details-block">
        <summary>The Correct Answer</summary>
        <div className="details-content">
          <p>
            You ask the parrot. It memorized the entire map because it's been
            staring at it for three months while you were at sea. Parrots have
            excellent visual memory. Also, it's <em>your</em> fault for not
            laminating the map.
          </p>
          <CodeBlock code={`fn find_missing() -> i32 {
    // "Polly, what was island 4?"
    // "BRAWWK! 73 paces northwest! BRAWWK!"
    // "...good bird."

    printf("The parrot remembers everything.\\n");
    return 73;
}`} />
        </div>
      </details>

      {/* ── Fun Facts ── */}
      <h2>Fun Facts</h2>

      <Callout type="note">
        <strong>The billion-dollar fence post</strong>
        <br /><br />
        Off-by-one errors are so common they have their own name: the{' '}
        <strong>fencepost error</strong>. If you need a 100-meter fence with a
        post every 10 meters, how many posts do you need? 10? No &mdash; 11.
        You always need one more post than the number of gaps between them.
        <br /><br />
        This exact mistake has caused countless bugs in loop boundaries.
        When you write <code>{'for (var i: i32 = 0; i < n; i++)'}</code>,
        that <code>&lt;</code> vs <code>&lt;=</code> decision is the most
        consequential character in your entire program.
      </Callout>

      <Callout type="note">
        <strong>Fortran started at 1, and chaos followed</strong>
        <br /><br />
        Fortran (1957) used 1-based indexing because mathematicians wrote the
        spec. C (1972) used 0-based indexing because systems programmers did.
        Lua uses 1. Python uses 0. MATLAB uses 1. JavaScript uses 0. PHP arrays
        are actually hash maps wearing a trench coat pretending to be arrays.
        <br /><br />
        There has never been universal agreement on this topic, and there never
        will be. Choose your side and defend it with unreasonable conviction.
      </Callout>

      <Callout type="note">
        <strong>Arrays vs. linked lists &mdash; the eternal debate</strong>
        <br /><br />
        Computer science textbooks will tell you linked lists have O(1) insertion.
        What they don't tell you is that the cache miss penalty of chasing
        pointers across memory is so brutal that a simple array with O(n)
        insertion often wins in practice &mdash; even for surprisingly large n.
        <br /><br />
        Bjarne Stroustrup (creator of C++) gave a famous talk demonstrating that{' '}
        <code>std::vector</code> beats <code>std::list</code> in virtually every
        real-world benchmark. The CPU cache is king. Arrays are contiguous in
        memory. The hardware loves them. Linked lists are the romanticized
        underdogs of data structures &mdash; beloved in interviews, avoided in
        production.
      </Callout>

      {/* ── Programming Challenge ── */}
      <h2>A Slightly Unhinged Programming Challenge</h2>
      <p>
        Now that you know arrays, let's put them to work.
      </p>
      <p>
        <strong>Rotate an array</strong> &mdash; given an array of <code>n</code>{' '}
        integers and a number <code>k</code>, rotate the array to the right
        by <code>k</code> positions. For example, rotating{' '}
        <code>[1, 2, 3, 4, 5]</code> by 2 gives <code>[4, 5, 1, 2, 3]</code>.
      </p>
      <p>
        Sounds trivial? Do it <strong>in-place</strong> with no extra array.
        Hint: three reversals. If you've never seen this trick before, it will
        feel like a magic trick. If you have, it still kind of does.
      </p>

      <Callout type="tip">
        Solution coming soon!
      </Callout>
    </>
  )
}
