import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Memory() {
  return (
    <>
      <h1>Memory Management</h1>

      {/* ── Section 1: Manual Memory Management ── */}
      <h2>Manual Memory Management</h2>
      <p>
        jlang uses manual memory management. You allocate memory on the heap
        with <code>alloc&lt;T&gt;()</code> and release it with{' '}
        <code>free()</code>. There is no garbage collector and no complex
        ownership or borrow-checking system.
      </p>

      <CodeBlock code={`var p: Person* = alloc<Person>();

p.Name = "Robert MeDiro";
p.Age = 30;

printf("Name: %s, Age: %d", p.Name, p.Age);

free(p);`} />

      <h3>Allocation and Deallocation</h3>
      <p>
        The <code>alloc&lt;T&gt;()</code> function allocates enough memory on
        the heap to hold one instance of type <code>T</code> and returns a
        pointer to it. When you are done with the memory, call{' '}
        <code>free()</code> to release it back to the system.
      </p>

      <CodeBlock code={`// Allocate
var config: Config* = alloc<Config>();

// Use the allocated memory
config.Name = "Production";

// Free when done
free(config);`} />

      <Callout type="important">
        You are responsible for freeing all allocated memory. Forgetting to
        call <code>free()</code> leads to memory leaks. Using a pointer after
        calling <code>free()</code> on it is undefined behavior.
      </Callout>

      {/* ── Section 2: Design Philosophy ── */}
      <h2>Design Philosophy</h2>
      <p>
        jlang deliberately keeps memory management simple and explicit. There
        is no garbage collector pausing your program, no reference counting
        overhead, and no complex ownership rules to learn. You allocate, you
        use, you free.
      </p>

      <CodeBlock code={`fn createPerson(name: char*, age: i32) -> Person* {
    var p: Person* = alloc<Person>();
    p.Name = name;
    p.Age = age;
    return p;
}

fn main() -> i32 {
    var robert: Person* = createPerson("Robert MeDiro", 30);
    printf("Name: %s", robert.Name);
    free(robert);
    return 0;
}`} />

      <Callout type="note">
        This model is similar to C's <code>malloc</code> and <code>free</code>,
        but with type-safe generics. <code>alloc&lt;Person&gt;()</code> always
        returns a <code>Person*</code>, so there is no need for manual casting.
      </Callout>
    </>
  )
}
