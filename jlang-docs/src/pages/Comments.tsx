import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Comments() {
  return (
    <>
      <h1>Comments</h1>

      <blockquote>
        <p>
          "Comments are not like Schindler's List. They are not 'pure good.'
          Indeed, comments are, at best, a necessary evil. If our programming
          languages were expressive enough, or if we had the talent to subtly
          wield those languages to express our intent, we would not need
          comments very much — perhaps not at all."
        </p>
        <footer>
          — Robert C. Martin, <em>Clean Code</em>
        </footer>
      </blockquote>

      {/* ── Section 1: Single-line comments ── */}
      <h2>Single-line comments</h2>
      <p>
        Use <code>//</code> to write a comment that spans the rest of the line.
        Everything after <code>//</code> until the end of the line is ignored by the compiler.
      </p>

      <CodeBlock
        code={`// This is a single-line comment

fn main() -> i32 {
    // Comment before a variable
    var x: i32 = 10; // inline comment

    return 0;
}`}
      />

      {/* ── Section 2: Block comments ── */}
      <h2>Block comments</h2>
      <p>
        Use <code>/* ... */</code> to write comments that span multiple lines.
        Block comments are useful for temporarily disabling code or writing
        longer explanations.
      </p>

      <CodeBlock
        code={`/* This is a
   multi-line
   block comment */

fn main() -> i32 {
    /* Block comment in code */
    var y: i32 = 20;

    return 0;
}`}
      />

      {/* ── Section 3: Comments don't affect code ── */}
      <h2>Comments don't affect code</h2>
      <p>
        Comments are stripped during lexing and have no effect on program behavior.
        Operators like <code>/</code> still work normally — the scanner only treats
        <code>//</code> and <code>/*</code> as comment starters.
      </p>

      <CodeBlock
        code={`fn main() -> i32 {
    var x: i32 = 10;
    var y: i32 = 20;

    // Division still works fine
    var z: i32 = x / y;

    return 0;
}`}
      />

      <Callout type="tip">
        Prefer single-line comments for brief notes and block comments for
        disabling larger sections of code during debugging.
      </Callout>

      <Callout type="life-lesson">
        <strong>Banner comments are a relic of the past</strong>
        <p>
          You may encounter code that uses long decorative comment lines — often
          called <em>banner comments</em> — to visually group functions into
          sections:
        </p>

        <CodeBlock
          code={`// Actions //////////////////////////////////

fn doSomething() -> void {
}

fn doSomethingElse() -> void {
}

// Utilities /////////////////////////////////

fn helperA() -> i32 {
    return 0;
}

fn helperB() -> i32 {
    return 0;
}`}
        />

        <p>
          This practice originates from the early days of C and C++, where it
          actually served a purpose. In C++ implementation files (<code>.cpp</code>),
          all methods are written at the top level — there is no visual
          distinction between public, private, or protected members. Banners
          helped developers mentally map which section of the class they were
          working in:
        </p>

        <CodeBlock
          code={`///////////////////////////////////////////////////////
// Public
///////////////////////////////////////////////////////

void Player::move(float dx, float dy) {
    x += dx;
    y += dy;
}

void Player::jump() {
    velocity.y = jumpForce;
}

///////////////////////////////////////////////////////
// Private
///////////////////////////////////////////////////////

void Player::applyGravity() {
    velocity.y -= gravity;
}

void Player::clampPosition() {
    if (y < 0) y = 0;
}`}
        />

        <p>
          In modern languages — including Jlang — this reason no longer applies.
          Access modifiers are declared directly on the members, the structure is
          visible at a glance, and any decent editor lets you jump to a symbol
          instantly. Banners just add noise.
        </p>

        <blockquote>
          <p>
            "There are rare times when it makes sense to gather certain
            functions together beneath a banner like this. But in general, they
            are clutter that should be eliminated — especially the startling
            train of slashes at the end."
          </p>
          <footer>
            — Robert C. Martin, <em>Clean Code</em>
          </footer>
        </blockquote>

        <p>
          If you feel the need to separate your code into labeled sections,
          it is usually a sign that your file is doing too much.
          Split it into smaller, well-named files instead.
        </p>
      </Callout>
    </>
  )
}
