import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function ErrorHandling() {
  return (
    <>
      <h1>Error Handling</h1>

      {/* ── Section 1: Result Type ── */}
      <h2>The Result Type</h2>
      <p>
        jlang uses a generic <code>Result&lt;T, E&gt;</code> type for error
        handling. A <code>Result</code> holds either a success value
        (<code>Ok</code>) or an error value (<code>Err</code>). This approach is
        inspired by Rust and makes error handling explicit and type-safe.
      </p>

      <h3>Creating Results</h3>

      <CodeBlock code={`var result: Result<i32, char*> = Ok(42);`} />

      <CodeBlock code={`var error: Result<i32, char*> = Err("Something went wrong");`} />

      {/* ── Section 2: Pattern Matching with Match ── */}
      <h2>Pattern Matching with Match</h2>
      <p>
        Use <code>match</code> expressions to extract the value from a{' '}
        <code>Result</code>. You can use match as both an expression and a
        statement.
      </p>

      <h3>Match as an Expression</h3>
      <CodeBlock code={`var message: char* = match result {
    Ok(value) => printf("Success: %d", value),
    Err(e) => printf("Error: %s", e),
};`} />

      <h3>Match as a Statement</h3>
      <CodeBlock code={`match result {
    Ok(value) => {
        printf("Got value: %d", value);
    },
    Err(e) => {
        printf("Failed: %s", e);
    },
};`} />

      <h3>Exhaustiveness</h3>
      <p>
        The compiler requires that you handle <strong>both</strong>{' '}
        <code>Ok</code> and <code>Err</code> variants. Forgetting to handle
        either case is a compile-time error.
      </p>

      <CodeBlock code={`match result {
    Ok(value) => printf("Got: %d", value),
    // ERROR: non-exhaustive match, missing 'Err' case
};`} />

      <table className="doc-table">
        <thead>
          <tr>
            <th>Pattern</th>
            <th>Binds</th>
            <th>Description</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>Ok(value)</code></td>
            <td><code>value: T</code></td>
            <td>Matches the success variant and binds the inner value</td>
          </tr>
          <tr>
            <td><code>Err(e)</code></td>
            <td><code>e: E</code></td>
            <td>Matches the error variant and binds the error value</td>
          </tr>
        </tbody>
      </table>

      {/* ── Section 3: Functions Returning Result ── */}
      <h2>Functions Returning Result</h2>
      <p>
        Functions that can fail return a <code>Result</code> type. The caller is
        then responsible for handling both the success and error cases.
      </p>

      <CodeBlock code={`fn divide(a: i32, b: i32) -> Result<i32, char*> {
    if (b == 0) {
        return Err("Division by zero");
    }
    return Ok(a / b);
}`} />

      <h3>Handling at the Call Site</h3>
      <p>
        The caller uses <code>match</code> to handle the result:
      </p>

      <CodeBlock code={`var result := divide(10, 0);

match result {
    Ok(value) => {
        printf("Result: %d", value);
    },
    Err(e) => {
        printf("Error: %s", e);
    },
};`} />

      <Callout type="note">
        By returning <code>Result</code> instead of using exceptions or error
        codes, jlang makes the possibility of failure visible in the function
        signature. Callers cannot accidentally ignore errors because the compiler
        enforces exhaustive matching.
      </Callout>
    </>
  )
}
