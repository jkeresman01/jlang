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

      <Callout type="note">
        <blockquote>
          <p>
            "Truth can only be found in one place: the code."
          </p>
          <footer>
            — Robert C. Martin, <em>Clean Code</em>
          </footer>
        </blockquote>
      </Callout>

      <Callout type="life-lesson">
        <blockquote>
          <p>
            "A rumor ends when it reaches a wise man's ear."
          </p>
        </blockquote>
        <p>
          Not every piece of information deserves to be repeated. A wise person
          hears a rumor and lets it die — they don't pass it along, amplify it,
          or treat it as fact. The same principle applies to code comments: not
          every thought that crosses your mind while coding needs to be written
          down. A comment that restates the obvious, spreads a misunderstanding,
          or echoes outdated reasoning is the code equivalent of passing along a
          rumor. Let noise die where it lands. Only propagate what is true,
          useful, and necessary.
        </p>
      </Callout>

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

      <Callout type="tip">
        <strong>Fun fact:</strong> If you should know any language, it should
        probably be Java. Over <strong>3 billion devices</strong> run Java
        worldwide, with more than <strong>73 billion Java Virtual Machines</strong>{' '}
        running globally. Over <strong>90% of Fortune 500 companies</strong> use
        Java for their software development, and there are an estimated{' '}
        <strong>9 million Java developers</strong> employed today &mdash; with
        projections showing 18.7 million new Java developer positions emerging
        by 2026. If there is one language worth knowing inside and out, Java is
        a pretty safe bet.
        <br /><br />
        But that&rsquo;s just my opinion. Here is what Bjarne Stroustrup thinks:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/NvWTnIoQZj4"
          title="Bjarne Stroustrup on how many languages you should know"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="tip">
        <strong>Java vs C#:</strong> If you ever doubt whether C# is better than
        Java, listen to what Scala Johansson has to say:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/RnqAXuLZlaE"
          title="Java vs C#"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>
    </>
  )
}
