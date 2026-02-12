import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Arrays() {
  return (
    <>
      <h1>Arrays</h1>

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
    </>
  )
}
