import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function ControlFlow() {
  return (
    <>
      <h1>Control Flow</h1>

      <blockquote>
        <p>
          "Logic, like whiskey, loses its beneficial effect when taken in too
          large quantities."
        </p>
        <footer>
          — Edward John Moreton Drax Plunkett, Lord Dunsany, quoted in{' '}
          <em>Crafting Interpreters</em> by Robert Nystrom
        </footer>
      </blockquote>

      <Callout type="note">
        <strong>Fun Fact</strong>
        <br /><br />
        Turing called his inventions "a-machines" for "automatic". He wasn't so
        self-aggrandizing as to put his own name on them. Later mathematicians
        did that for him. <strong>That's how you get famous while still retaining some
        modesty.</strong>
        <br /><br />
        — From <em>Crafting Interpreters</em> by Robert Nystrom
      </Callout>

      {/* ── Section 1: If/Else ── */}
      <h2>If / Else</h2>
      <p>
        Conditional branching uses familiar C-style syntax. Conditions are
        wrapped in parentheses and the body is enclosed in braces. There is no
        special <code>elseif</code> or <code>elif</code> keyword; chain
        standard <code>else if</code> instead.
      </p>

      <CodeBlock
        code={`if (x == 0) {
    printf("x is zero");
} else if (x == 10) {
    printf("x is ten");
} else {
    printf("x is something else");
}`}
      />

      <p>
        Braces are optional for single-statement bodies, but using them is
        recommended for clarity.
      </p>

      {/* ── Section 2: While Loops ── */}
      <h2>While Loops</h2>
      <p>
        A <code>while</code> loop repeats its body as long as the condition
        evaluates to <code>true</code>.
      </p>

      <CodeBlock
        code={`var i: i32 = 0;
while (i < 10) {
    printf("i = %d", i);
    i = i + 1;
}`}
      />

      {/* ── Section 3: For Loops ── */}
      <h2>For Loops</h2>
      <p>
        The <code>for</code> loop has three semicolon-separated clauses:
        an initializer (variable declaration or expression), a condition, and
        an update expression. All three clauses are optional.
      </p>

      <CodeBlock
        code={`for (var i: i32 = 0; i < 10; i++) {
    printf("i = %d", i);
}`}
      />

      <h3>More Examples</h3>

      <p>With type inference:</p>
      <CodeBlock
        code={`for (var i := 0; i < 5; i++) {
    printf("%d", i);
}`}
      />

      <p>Counting down:</p>
      <CodeBlock
        code={`for (var i: i32 = 10; i > 0; i--) {
    printf("Countdown: %d", i);
}`}
      />

      {/* ── Section 4: Break and Continue ── */}
      <h2>Break and Continue</h2>

      <h3><code>break</code></h3>
      <p>
        The <code>break</code> statement immediately exits the innermost
        enclosing loop.
      </p>

      <CodeBlock
        code={`for (var i: i32 = 0; i < 100; i++) {
    if (i == 3) {
        break;
    }
    printf("i = %d", i);
}
// prints: i = 0, i = 1, i = 2`}
      />

      <h3><code>continue</code></h3>
      <p>
        The <code>continue</code> statement skips the rest of the current
        iteration and jumps to the next one.
      </p>

      <CodeBlock
        code={`for (var j: i32 = 0; j < 5; j++) {
    if (j == 2) {
        continue;
    }
    printf("j = %d", j);
}
// prints: j = 0, j = 1, j = 3, j = 4`}
      />

      <p>
        Both <code>break</code> and <code>continue</code> work in{' '}
        <code>while</code> and <code>for</code> loops.
      </p>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Statement</th>
            <th>Effect</th>
            <th>Works in</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>break</code></td>
            <td>Exits the innermost loop</td>
            <td><code>for</code>, <code>while</code></td>
          </tr>
          <tr>
            <td><code>continue</code></td>
            <td>Skips to next iteration</td>
            <td><code>for</code>, <code>while</code></td>
          </tr>
        </tbody>
      </table>

      <p>
        In <code>for</code> loops, <code>continue</code> jumps to the update
        clause before re-evaluating the condition. In <code>while</code> loops
        it jumps directly to the condition check.
      </p>

      {/* ── Programming Challenge ── */}
      <h2>A Slightly Unhinged Programming Challenge</h2>
      <p>
        Now that you know loops, conditionals, and break/continue &mdash; let's
        put it all together with something fun.
      </p>
      <p>
        <strong>Conway's Game of Life</strong> is a zero-player "game" invented
        by mathematician John Conway in 1970. You start with a grid of cells,
        each either alive or dead. Every tick, four simple rules decide what
        happens next:
      </p>
      <ol>
        <li>A live cell with fewer than 2 neighbors dies (underpopulation).</li>
        <li>A live cell with 2 or 3 neighbors survives.</li>
        <li>A live cell with more than 3 neighbors dies (overpopulation).</li>
        <li>A dead cell with exactly 3 neighbors becomes alive (reproduction).</li>
      </ol>
      <p>
        That's it. Four rules, and somehow you get self-replicating patterns,
        gliders, and even Turing-complete computation. Your task: implement it
        on an 8&times;8 grid in Jlang. Seed it with a
        {' '}<a href="https://en.wikipedia.org/wiki/Glider_(Conway%27s_Game_of_Life)" target="_blank" rel="noopener noreferrer">glider</a>{' '}
        and run it for 20 generations. Print the grid each step.
      </p>

      <Callout type="note">
        <strong>Conway's Game of Life visualized</strong>
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/CgOcEZinQ2I"
          title="Conway's Game of Life"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="tip">
        Solution coming soon!
      </Callout>
    </>
  )
}
