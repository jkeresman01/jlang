import CodeBlock from '../components/CodeBlock';
import Callout from '../components/Callout';

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
          <p>"Truth can only be found in one place: the code."</p>
          <footer>
            — Robert C. Martin, <em>Clean Code</em>
          </footer>
        </blockquote>
      </Callout>

      <Callout type="life-lesson">
        <blockquote>
          <p>"A rumor ends when it reaches a wise man's ear."</p>
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
          I found an old image on my phone the other day &mdash; a
          programming assignment from about five years ago. I still remember
          writing one enormous method that split a sentence into words. It was
          ugly. I remember thinking at the time: I am a genius, I better
          comment this so future-me understands it. A few books later I
          realised something profound &mdash; the code was so bad that no
          comment could have saved it. The comment was not documentation, it
          was a cry for help. Nameing is also import found this one ORM SQL Boiler 
          just some time ago, like who names the thing boiler, 
          that goes in the bathroom.
        </p>
        <p>
          Linus Torvalds once gave a talk where he described what he considers
          &ldquo;good taste&rdquo; in code &mdash; and the kind of people he
          wants to work with. It is worth hearing in his own words:
        </p>
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/o8NPllzkFhE?start=863&end=985"
          title="Linus Torvalds on good taste in code"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      {/* ── Section 1: Single-line comments ── */}
      <h2>Single-line comments</h2>
      <p>
        Use <code>//</code> to write a comment that spans the rest of the line.
        Everything after <code>//</code> until the end of the line is ignored by
        the compiler.
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
        Comments are stripped during lexing and have no effect on program
        behavior. Operators like <code>/</code> still work normally — the
        scanner only treats
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
          actually served a purpose. In C++ implementation files (
          <code>.cpp</code>), all methods are written at the top level — there
          is no visual distinction between public, private, or protected
          members. Banners helped developers mentally map which section of the
          class they were working in:
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
          Access modifiers are declared directly on the members, the structure
          is visible at a glance, and any decent editor lets you jump to a
          symbol instantly. Banners just add noise.
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
          If you feel the need to separate your code into labeled sections, it
          is usually a sign that your file is doing too much. Split it into
          smaller, well-named files instead.
        </p>
      </Callout>

      <Callout type="tip">
        <strong>Fun fact:</strong> If you should know any language, it should
        probably be Java. Over <strong>3 billion devices</strong> run Java
        worldwide, with more than{' '}
        <strong>73 billion Java Virtual Machines</strong> running globally. Over{' '}
        <strong>90% of Fortune 500 companies</strong> use Java for their
        software development. If there is one language worth knowing inside and
        out, Java is a pretty safe bet.
        <br />
        <br />
        But that&rsquo;s just my opinion. Here is what Bjarne Stroustrup thinks:
        <br />
        <br />
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
        <br />
        <br />
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
        <br />
        All jokes aside &mdash; if you look at where Java and C# stand today
        objectively, .NET has pulled ahead of Spring in several areas, and C# as
        a language has become noticeably more elegant and feature-rich than
        Java. Here are a few concrete examples:
        <br />
        <br />
        <strong>Properties vs. getters/setters:</strong> C# has first-class
        properties built into the language. What takes a field, a getter, and a
        setter in Java is a single line in C#:
        <br />
        <br />
        <code
          style={{
            whiteSpace: 'pre',
            display: 'block',
            padding: '0.75rem 1rem',
            background: 'var(--bg-code)',
            borderRadius: '6px',
            fontSize: '0.835rem',
            lineHeight: '1.65',
          }}
        >
          {`// C#
public string Name { get; set; }

// Java — you need all of this
private String name;
public String getName() { return name; }
public void setName(String name) { this.name = name; }`}
        </code>
        <br />
        <strong>LINQ vs. Streams:</strong> C#&rsquo;s LINQ is integrated into
        the language with query syntax and is far more readable than
        Java&rsquo;s Stream API, which often turns into a wall of chained method
        calls:
        <br />
        <br />
        <code
          style={{
            whiteSpace: 'pre',
            display: 'block',
            padding: '0.75rem 1rem',
            background: 'var(--bg-code)',
            borderRadius: '6px',
            fontSize: '0.835rem',
            lineHeight: '1.65',
          }}
        >
          {`// C# — LINQ query syntax
var adults = from p in people
             where p.Age >= 18
             orderby p.Name
             select p;

// Java — Streams
List<Person> adults = people.stream()
    .filter(p -> p.getAge() >= 18)
    .sorted(Comparator.comparing(Person::getName))
    .toList();`}
        </code>
        <br />
        <strong>Async/await:</strong> C# had <code>async</code>/
        <code>await</code> since 2012. Java only got virtual threads (Project
        Loom) in 2023, and it still lacks the syntactic elegance &mdash; there
        is no <code>await</code> keyword, you just call <code>.join()</code> and
        hope for the best.
        <br />
        <br />
        <strong>Pattern matching:</strong> C# supports pattern matching with{' '}
        <code>switch</code> expressions, type patterns, property patterns, and
        relational patterns. Java has been catching up since Java 17, but C# has
        had this for years and its syntax is cleaner:
        <br />
        <br />
        <code
          style={{
            whiteSpace: 'pre',
            display: 'block',
            padding: '0.75rem 1rem',
            background: 'var(--bg-code)',
            borderRadius: '6px',
            fontSize: '0.835rem',
            lineHeight: '1.65',
          }}
        >
          {`// C# — switch expression with pattern matching
string Classify(object obj) => obj switch
{
    int n when n > 0 => "positive",
    int n when n < 0 => "negative",
    string s         => $"text: {s}",
    null             => "nothing",
    _                => "unknown"
};`}
        </code>
        <br />
        <strong>
          Value types (<code>struct</code>):
        </strong>{' '}
        C# lets you define value types that live on the stack, avoiding heap
        allocations and GC pressure entirely. Java is still working on this with
        Project Valhalla, which has been in development for nearly a decade.
        <br />
        <br />
        <strong>.NET minimal APIs vs. Spring Boot:</strong> On the framework
        side, .NET&rsquo;s minimal APIs let you spin up a web server in a few
        lines with zero boilerplate. Spring Boot has improved, but it still
        relies heavily on annotations, reflection, and a large dependency tree:
        <br />
        <br />
        <code
          style={{
            whiteSpace: 'pre',
            display: 'block',
            padding: '0.75rem 1rem',
            background: 'var(--bg-code)',
            borderRadius: '6px',
            fontSize: '0.835rem',
            lineHeight: '1.65',
          }}
        >
          {`// .NET minimal API — that's the whole thing
var app = WebApplication.CreateBuilder(args).Build();
app.MapGet("/hello", () => "Hello, World!");
app.Run();`}
        </code>
        <br />
        None of this means Java is bad &mdash; it is a proven workhorse that
        powers half the enterprise world. But if you line the two languages up
        side by side today, C# simply has more modern features, less ceremony,
        and a runtime that has been iterating faster. Credit where credit is
        due.
      </Callout>
    </>
  );
}
