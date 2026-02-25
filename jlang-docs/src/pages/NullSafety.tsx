import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function NullSafety() {
  return (
    <>
      <h1>Null Safety</h1>

      {/* ── The Billion-Dollar Mistake ── */}
      <h2>The Billion-Dollar Mistake</h2>

      <p>
        In 2009, Tony Hoare &mdash; the inventor of the null reference &mdash;
        stood in front of a room full of software engineers at QCon London and
        said: <em>"I call it my billion-dollar mistake. It was the invention of
        the null reference in 1965."</em> He had added it to ALGOL W simply
        because it was easy to implement. That single shortcut went on to
        infect nearly every language that followed &mdash; C, C++, Java, C# &mdash;
        and has been responsible for countless crashes, security vulnerabilities,
        and late-night debugging sessions ever since.
      </p>

      <p>
        Robert C. Martin retells this story in <em>Clean Code: A Handbook of
        Agile Software Craftsmanship</em>, Chapter 7 &mdash; "Error Handling",
        where he lays down two rules: <strong>Don't Return Null</strong> and{' '}
        <strong>Don't Pass Null</strong>. His argument is simple &mdash; every{' '}
        <code>null</code> you return is a null-check someone will forget to
        write, and every <code>null</code> you pass is a crash waiting to happen.
      </p>

      <p>
        Jlang takes this advice to its logical conclusion: pointers are
        non-nullable by default. If a value might be absent, you must say so
        explicitly with <code>*?</code>. The compiler won't let you forget.
        Hoare needed fifty years and a public apology. You just need a question
        mark.
      </p>

      <Callout type="important">
        <strong>Why pointers earn their place here</strong>
        <br /><br />
        Java was created in large part because of pointer bugs. James Gosling
        looked at the damage C and C++ caused &mdash; dangling pointers, wild
        arithmetic, entire systems undone by a single bad address &mdash; and
        ripped pointers out entirely. But they kept <code>null</code>. They
        removed the gun and left the bullet.{' '}
        <code>NullPointerException</code> became the most thrown exception in
        Java&rsquo;s history. If pointers were dangerous enough to kill,{' '}
        <em>why did null survive?</em>
        <br /><br />
        Pointers were the first concept that truly broke my brain. That feeling where
        someone is drawing boxes and arrows on a whiteboard, explaining how this
        address points to that address, and you&rsquo;re nodding but nothing is
        clicking &mdash; just a wall of fog between you and the idea. The second time
        that happened was Kotlin&rsquo;s <code>reified</code> keyword and the whole
        rabbit hole of covariance, contravariance, and type erasure vs. reification.
        Two concepts that made me feel genuinely stupid before they finally made me
        better. That&rsquo;s why they earn their place in this language and in this
        book.
        <br /><br />
        And then there&rsquo;s Go. Go proved that pointers don&rsquo;t have to be the
        nightmare that C made them. Go has pointers &mdash; real, honest pointers &mdash;
        but no pointer arithmetic. You can take the address of a variable, you can
        dereference it, but you cannot walk through memory like a drunk cowboy with a
        loaded revolver. Go showed the world that if you forbid the dangerous parts
        and keep the useful parts, pointers become a tool instead of a time bomb.
        That&rsquo;s exactly the bet Jlang makes.
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/RrMptmNYkSw"
          title="James Gosling on why Java removed pointers"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      {/* ── Section 1: Non-nullable by Default ── */}
      <h2>Non-nullable by Default</h2>
      <p>
        Pointer types in jlang are <strong>non-nullable by default</strong>. A
        plain pointer type like <code>Person*</code> can never hold{' '}
        <code>null</code>.
      </p>

      <CodeBlock code={`var p: Person* = alloc<Person>();`} />

      <p>
        To declare a pointer that <em>can</em> be null, append a question mark
        to create a nullable pointer: <code>*?</code>.
      </p>

      <CodeBlock code={`var q: Person*? = null;`} />

      {/* ── Section 2: The Three Rules ── */}
      <h2>The Three Rules</h2>

      <h3>1. You cannot assign <code>null</code> to a non-nullable pointer</h3>
      <CodeBlock code={`var p: Person* = null;  // ERROR: cannot assign null to non-nullable type 'Person*'`} />

      <h3>2. Non-nullable pointers must be initialized</h3>
      <CodeBlock code={`var p: Person*;  // ERROR: non-nullable pointer 'p' must be initialized`} />

      <h3>3. You cannot access members on a nullable pointer directly</h3>
      <CodeBlock code={`var q: Person*? = null;
printf("%s", q.Name);  // ERROR: cannot access member 'Name' on nullable type 'Person*?'`} />

      <p>
        To access members on a nullable pointer, use the elvis operator to
        convert it to a non-nullable pointer first:
      </p>

      <CodeBlock code={`var q: Person*? = findPerson("Alice");
var p: Person* = q ?: getDefaultPerson();
printf("Found: %s", p.Name);  // OK: p is non-nullable`} />

      {/* ── Section 3: Comparison Table ── */}
      <h2>Nullable vs Non-nullable</h2>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Syntax</th>
            <th>Nullable</th>
            <th>Can be null</th>
            <th>Member access</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>Type*</code></td>
            <td>No</td>
            <td>No</td>
            <td>Direct</td>
          </tr>
          <tr>
            <td><code>Type*?</code></td>
            <td>Yes</td>
            <td>Yes</td>
            <td>Requires null check</td>
          </tr>
        </tbody>
      </table>

      <Callout type="tip">
        Prefer <code>Type*</code> (non-nullable) whenever possible. Only use{' '}
        <code>Type*?</code> when a value genuinely might be absent. This
        eliminates an entire class of null-pointer bugs at compile time.
      </Callout>

      <Callout type="note">
        Smart casts after null checks are a planned future enhancement. In the
        future, the compiler will automatically narrow a <code>Type*?</code> to{' '}
        <code>Type*</code> inside a null-check block.
      </Callout>

      {/* ── Section 4: Elvis Operator ── */}
      <h2>Elvis Operator <code>?:</code></h2>
      <p>
        The elvis operator <code>?:</code> provides a concise way to supply a
        fallback value when a nullable expression is null. If the left side is
        non-null, it is returned; otherwise, the right side is evaluated and
        returned.
      </p>

      <CodeBlock code={`var primary: Config*? = findConfig("main");
var cfg: Config* = primary ?: getDefaultConfig();`} />

      <h3>Short-circuit Evaluation</h3>
      <p>
        The right-hand side is only evaluated when the left-hand side is null.
        This means expensive fallback computations are skipped when they are not
        needed.
      </p>

      <CodeBlock code={`var primary: Config*? = findConfig("main");
// getDefaultConfig() is only called if primary is null
var cfg: Config* = primary ?: getDefaultConfig();`} />

      <h3>Equivalent Logic</h3>
      <p>
        The elvis operator is syntactic sugar for a common null-check pattern:
      </p>

      <CodeBlock code={`var primary: Config*? = findConfig("main");

// Using elvis operator
var cfg: Config* = primary ?: getDefaultConfig();

// Equivalent logic:
// if primary is non-null → cfg = primary
// if primary is null     → cfg = getDefaultConfig()`} />

      <h3>Behavior Summary</h3>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Expression</th>
            <th>Left is non-null</th>
            <th>Left is null</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>a ?: b</code></td>
            <td>Returns <code>a</code></td>
            <td>Evaluates and returns <code>b</code></td>
          </tr>
        </tbody>
      </table>

      <Callout type="note">
        The elvis operator is inspired by Kotlin's <code>?:</code> operator. The
        name comes from the resemblance of <code>?:</code> to Elvis Presley's
        hairstyle when viewed sideways.
      </Callout>

    </>
  )
}
