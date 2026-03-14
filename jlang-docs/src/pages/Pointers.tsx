import CodeBlock from '../components/CodeBlock';
import Callout from '../components/Callout';

export default function Pointers() {
  return (
    <>
      <h1>Pointers</h1>

      {/* ── Section 1: What Is a Pointer ── */}
      <h2>What Is a Pointer</h2>
      <p>
        A pointer is a variable that holds a memory address. That is the entire
        definition. Not a reference, not a handle, not a smart wrapper around
        something the runtime manages for you. A pointer is a number &mdash; an
        address &mdash; that tells you where something lives in memory. If you
        can understand that a street address tells you where a building is
        without being the building itself, you already understand pointers. The
        rest is syntax.
      </p>
      <p>
        In jlang, a pointer is declared by appending <code>*</code> to a type.{' '}
        <code>Person*</code> means &ldquo;a pointer to a Person.&rdquo; It does
        not mean &ldquo;a Person.&rdquo; It means &ldquo;I know where a Person
        lives.&rdquo;
      </p>

      <CodeBlock
        code={`struct Person {
    Name: char*;
    Age: i32;
}

fn main() -> i32 {
    var p: Person* = alloc<Person>();
    p.Name = "Alice";
    p.Age = 30;
    printf("Name: %s, Age: %d", p.Name, p.Age);
    free(p);
    return 0;
}`}
      />

      <Callout type="note">
        Pointers were the first concept that truly broke my brain. That feeling
        where someone is drawing boxes and arrows on a whiteboard, explaining
        how this address points to that address, and you are nodding but nothing
        is clicking &mdash; just a wall of fog between you and the idea. If you
        are reading this and feeling that fog, keep going. It clears.
      </Callout>

      {/* ── Section 2: Declaring Pointers ── */}
      <h2>Declaring Pointers</h2>
      <p>
        The only way to obtain a pointer in jlang is through allocation or by
        receiving one as a function parameter. There is no address-of operator.
        You cannot write <code>&amp;x</code> to take the address of a local
        variable. This is deliberate &mdash; stack addresses are dangerous to
        hand out because the moment the function returns, that address points to
        nothing. Languages that allow it give you a loaded gun and hope you
        remember not to pull the trigger after the safety window closes.
      </p>

      <CodeBlock
        code={`// The only way to get a pointer: allocate on the heap
var p: Person* = alloc<Person>();

// Or receive one as a parameter
fn greet(person: Person*) -> void {
    printf("Hello, %s!", person.Name);
}

// You CANNOT do this:
// var x: i32 = 42;
// var ptr: i32* = &x;  // ERROR: no address-of operator`}
      />

      <h3>Pointer to Arrays</h3>
      <p>
        Arrays can also live on the heap. The syntax is <code>Type[N]*</code>{' '}
        &mdash; a pointer to an array of <code>N</code> elements.
      </p>

      <CodeBlock
        code={`var heap: i32[10]* = alloc<i32[10]>();
heap[0] = 100;
heap[1] = 200;
printf("First: %d, Second: %d", heap[0], heap[1]);
free(heap);`}
      />

      {/* ── Section 3: Member Access ── */}
      <h2>Member Access</h2>
      <p>
        In C, if you have a pointer to a struct, you need the arrow operator{' '}
        <code>-&gt;</code> to access its fields. If you have the struct itself,
        you use dot. Two operators for the same logical action, distinguished
        only by whether you remembered to add an asterisk to the type. C++
        inherited this. So did Objective-C. Generations of programmers have
        spent collective centuries typing the wrong one, getting a compiler
        error, and fixing it.
      </p>
      <p>
        jlang does not have an arrow operator. There is only dot. Whether you
        have a struct or a pointer to a struct, you write <code>obj.field</code>{' '}
        and it works. The compiler knows the type. The compiler can figure out
        whether to dereference. You should not have to.
      </p>

      <CodeBlock
        code={`var p: Person* = alloc<Person>();

// Dot works on pointers — no -> needed
p.Name = "Bob";
p.Age = 25;
printf("Name: %s", p.Name);

free(p);`}
      />

      <Callout type="tip">
        <strong>Fun fact:</strong> The <code>-&gt;</code> operator in C exists
        because <code>(*ptr).field</code> is ugly and error-prone. Kernighan and
        Ritchie added it as syntactic sugar in the original C language. Go took
        the same position as jlang &mdash; just use dot and let the compiler
        sort it out. Rust went a step further with auto-dereferencing through
        the <code>Deref</code> trait. The arrow operator is one of those things
        that feels essential until you use a language that doesn&rsquo;t have
        it, and then you never miss it.
      </Callout>

      {/* ── Section 4: No Pointer Arithmetic ── */}
      <h2>No Pointer Arithmetic</h2>
      <p>
        In C, a pointer is just a number, and you can do math on it. Add one,
        subtract two, compare two pointers and get the distance between them.
        This is extraordinarily powerful and extraordinarily dangerous. Pointer
        arithmetic is the reason buffer overflows exist. It is the reason
        Heartbleed happened. It is the reason entire classes of security
        vulnerabilities have a name.
      </p>
      <p>
        jlang does not support pointer arithmetic. You cannot write{' '}
        <code>ptr + 1</code> to advance to the next element. If you want to
        access the third element of an array, you write <code>arr[2]</code>{' '}
        &mdash; and the compiler generates the address calculation for you, with
        bounds that are explicit and readable.
      </p>

      <CodeBlock
        code={`var arr: i32[5]* = alloc<i32[5]>();
arr[0] = 10;
arr[1] = 20;
arr[2] = 30;

// Access by index — clear, readable, safe
printf("Third element: %d", arr[2]);

// You CANNOT do this:
// var next: i32* = arr + 1;   // ERROR: no pointer arithmetic
// var val: i32 = *(arr + 2);  // ERROR: no dereference operator

free(arr);`}
      />

      <Callout type="note">
        Go made the same choice. Rob Pike once explained that pointer arithmetic
        was left out of Go not because it is useless, but because the damage it
        causes far outweighs the convenience it provides. Every buffer overflow
        in the history of computing started with someone doing math on an
        address. jlang agrees &mdash; if you need the third element, ask for the
        third element. Do not hand-calculate where it lives.
      </Callout>

      {/* ── Section 5: No Dereference Operator ── */}
      <h2>No Dereference Operator</h2>
      <p>
        There is no <code>*ptr</code> syntax in jlang. You cannot explicitly
        dereference a pointer. If you have a <code>Person*</code>, you access
        its fields with dot notation and the compiler handles the rest. There is
        no scenario where you need to manually dereference because member access
        already does it for you.
      </p>
      <p>
        This also means there is no ambiguity between multiplication and
        dereferencing. In C, <code>*a * *b</code> is a valid expression that
        dereferences two pointers and multiplies the results. It is also the
        kind of line that makes you question whether the person who wrote it was
        trying to write code or summon a demon. jlang does not have this
        problem.
      </p>

      {/* ── Section 6: No References ── */}
      <h2>No References</h2>
      <p>
        C++ has references &mdash; <code>T&amp;</code> &mdash; which are
        essentially pointers that pretend not to be pointers. They cannot be
        null, they cannot be reseated, and they use dot syntax instead of arrow.
        They were introduced to make operator overloading cleaner and to give
        C++ a way to pass by reference without the caller having to write{' '}
        <code>&amp;</code> everywhere.
      </p>
      <p>
        jlang does not have references. It has pointers, and pointers are
        already non-nullable by default, already use dot syntax, and already
        make it clear at the call site that you are passing an address. Adding a
        second way to spell the same concept would be redundant &mdash; and
        redundancy in a language is how you end up with three ways to initialize
        a variable and a committee to decide which one is
        &ldquo;idiomatic.&rdquo;
      </p>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Feature</th>
            <th>
              jlang Pointer (<code>T*</code>)
            </th>
            <th>
              C++ Reference (<code>T&amp;</code>)
            </th>
            <th>
              C++ Pointer (<code>T*</code>)
            </th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>Non-nullable by default</td>
            <td>Yes</td>
            <td>Yes</td>
            <td>No</td>
          </tr>
          <tr>
            <td>Dot syntax for access</td>
            <td>Yes</td>
            <td>Yes</td>
            <td>
              No (uses <code>-&gt;</code>)
            </td>
          </tr>
          <tr>
            <td>Can be null</td>
            <td>
              Only with <code>*?</code>
            </td>
            <td>No</td>
            <td>Yes</td>
          </tr>
          <tr>
            <td>Arithmetic</td>
            <td>No</td>
            <td>No</td>
            <td>Yes</td>
          </tr>
          <tr>
            <td>Explicit at call site</td>
            <td>Yes</td>
            <td>No</td>
            <td>Yes</td>
          </tr>
        </tbody>
      </table>

      <Callout type="important">
        The difference between a jlang pointer and a C++ reference is honesty. A
        C++ reference is a pointer wearing a disguise &mdash; it holds an
        address, it follows an address, but it pretends to be the object itself.
        That disguise can be useful, but it can also hide what is really
        happening. In jlang, a pointer looks like a pointer, behaves like a
        pointer, and never pretends to be something it is not.
      </Callout>

      {/* ── Section 7: The Self Parameter ── */}
      <h2>
        The <code>self</code> Parameter
      </h2>
      <p>
        Methods in jlang are just functions whose first parameter is named{' '}
        <code>self</code> and is a pointer to the struct. There is no implicit{' '}
        <code>this</code> that appears out of nowhere. You declare the receiver
        explicitly, just like Python and Rust do.
      </p>

      <CodeBlock
        code={`struct Point {
    X: i32;
    Y: i32;
}

fn print(self: Point*) {
    printf("Point(%d, %d)", self.X, self.Y);
}

fn translate(self: Point*, dx: i32, dy: i32) -> void {
    self.X = self.X + dx;
    self.Y = self.Y + dy;
}

fn main() -> i32 {
    var pt: Point* = alloc<Point>();
    pt.X = 10;
    pt.Y = 20;

    pt.print();          // Calls Point_print(pt)
    pt.translate(5, 3);  // Calls Point_translate(pt, 5, 3)
    pt.print();          // Point(15, 23)

    free(pt);
    return 0;
}`}
      />

      <p>
        When you call <code>pt.print()</code>, the compiler rewrites it to{' '}
        <code>Point_print(pt)</code>. The method name is mangled to{' '}
        <code>StructName_methodName</code>, and the pointer you called it on
        becomes the first argument. There is no vtable dispatch for regular
        methods, no hidden indirection &mdash; just a function call with an
        explicit receiver.
      </p>

      <Callout type="note">
        C++ chose implicit <code>this</code>. Java chose implicit{' '}
        <code>this</code>. C# chose implicit <code>this</code>. Python chose
        explicit <code>self</code> and got criticized for it for twenty years.
        Rust chose explicit <code>self</code> and got praised for it. Go chose
        explicit receivers and the world moved on. Turns out, making the
        receiver visible is not a burden &mdash; it is clarity.
      </Callout>

      {/* ── Section 8: Self-Referential Structs ── */}
      <h2>Self-Referential Structs</h2>
      <p>
        A struct can contain a pointer to its own type. This is how you build
        linked lists, trees, graphs &mdash; any data structure where one node
        needs to know about another.
      </p>

      <CodeBlock
        code={`struct Node {
    Value: i32;
    Next: Node*;    // Points to the next node
    prev: Node*;    // Points to the previous node (private)
}

fn main() -> i32 {
    var a: Node* = alloc<Node>();
    var b: Node* = alloc<Node>();
    var c: Node* = alloc<Node>();

    a.Value = 1;
    b.Value = 2;
    c.Value = 3;

    a.Next = b;
    b.Next = c;

    // Walk the list
    printf("%d -> ", a.Value);
    printf("%d -> ", a.Next.Value);
    printf("%d", a.Next.Next.Value);

    free(a);
    free(b);
    free(c);
    return 0;
}`}
      />

      <p>
        Notice that <code>a.Next.Value</code> works naturally &mdash; the first
        dot dereferences <code>a</code> to access <code>Next</code>, and the
        second dot dereferences <code>Next</code> to access <code>Value</code>.
        No arrows, no explicit dereferencing, no ceremony. Just dots all the way
        down.
      </p>

      {/* ── Section 9: Strings Are Pointers ── */}
      <h2>Strings Are Pointers</h2>
      <p>
        There is no dedicated string type in jlang. A string is a{' '}
        <code>char*</code> &mdash; a pointer to a null-terminated sequence of
        characters. This is the same representation C has used since 1972, and
        while it is not the safest or most ergonomic approach, it is honest. A
        string literal like <code>"hello"</code> is placed in the read-only data
        section of the binary and you get a pointer to it.
      </p>

      <CodeBlock
        code={`fn main() -> i32 {
    var greeting: char* = "Hello, World!";
    printf("%s", greeting);
    return 0;
}`}
      />

      <Callout type="tip">
        If you have ever wondered why C functions like <code>printf</code> take{' '}
        <code>char*</code> instead of some <code>String</code> type &mdash; this
        is why. At the hardware level, a string is just bytes in memory and an
        address that tells you where they start. Everything else is abstraction
        built on top. Sometimes the abstraction is worth it. Sometimes you just
        need to print a name.
      </Callout>

      {/* ── Section 10: Pointer Casting ── */}
      <h2>Pointer Casting</h2>
      <p>
        jlang supports casting between pointer types using C-style cast syntax.
        This is primarily used for interface conversions and interoperability
        with low-level code.
      </p>

      <CodeBlock
        code={`var p: Person* = alloc<Person>();
var raw: Node* = (Node*) p;  // Cast Person* to Node*`}
      />

      <Callout type="warning">
        Pointer casting is a sharp tool. It tells the compiler &ldquo;I know
        what I am doing, trust me.&rdquo; The compiler will trust you. If you
        are wrong, the program will not crash with a helpful error message
        &mdash; it will crash in ways that make you question your career
        choices. Use casts sparingly and only when you genuinely understand the
        memory layout of both types.
      </Callout>

      {/* ── Section 11: What jlang Removes ── */}
      <h2>What jlang Deliberately Removes</h2>
      <p>
        Most of the decisions in jlang&rsquo;s pointer design are subtractions,
        not additions. The language is defined as much by what it refuses to
        include as by what it provides.
      </p>

      <table className="doc-table">
        <thead>
          <tr>
            <th>Feature</th>
            <th>C / C++</th>
            <th>jlang</th>
            <th>Why</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>
              Address-of (<code>&amp;</code>)
            </td>
            <td>Yes</td>
            <td>No</td>
            <td>Stack addresses are dangerous to hand out</td>
          </tr>
          <tr>
            <td>
              Dereference (<code>*ptr</code>)
            </td>
            <td>Yes</td>
            <td>No</td>
            <td>Dot handles it automatically</td>
          </tr>
          <tr>
            <td>
              Arrow operator (<code>-&gt;</code>)
            </td>
            <td>Yes</td>
            <td>No</td>
            <td>Dot works on both values and pointers</td>
          </tr>
          <tr>
            <td>Pointer arithmetic</td>
            <td>Yes</td>
            <td>No</td>
            <td>Use array indexing instead</td>
          </tr>
          <tr>
            <td>
              References (<code>T&amp;</code>)
            </td>
            <td>C++ only</td>
            <td>No</td>
            <td>Pointers already cover this</td>
          </tr>
          <tr>
            <td>
              Implicit <code>this</code>
            </td>
            <td>Yes</td>
            <td>No</td>
            <td>
              Explicit <code>self</code> is clearer
            </td>
          </tr>
          <tr>
            <td>Nullable by default</td>
            <td>Yes</td>
            <td>No</td>
            <td>
              Non-nullable by default; opt in with <code>*?</code>
            </td>
          </tr>
        </tbody>
      </table>

      <p>
        Every feature removed is a class of bugs removed with it. Pointer
        arithmetic gone means no buffer overflows from hand-calculated
        addresses. No address-of means no dangling pointers to dead stack
        frames. No implicit <code>this</code> means no wondering where a
        variable came from. No arrow operator means no remembering which
        operator to use based on whether you have a value or a pointer.
      </p>

      <p>
        Subtraction is underrated. Most languages are defined by what they add.
        The best ones are defined by what they had the discipline to leave out.
      </p>

      <Callout type="note">
        <strong>
          Antoine de Saint-Exup&eacute;ry said it better than any programmer
          ever could:
        </strong>
        <br />
        <br />
        <em>
          &ldquo;Perfection is achieved, not when there is nothing more to add,
          but when there is nothing left to take away.&rdquo;
        </em>
        <br />
        <br />
        He was talking about aircraft design. He might as well have been talking
        about programming languages.
      </Callout>
    </>
  );
}
