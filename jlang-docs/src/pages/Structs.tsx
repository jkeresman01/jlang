import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Structs() {
  return (
    <>
      <h1>Structs</h1>

      {/* ── Section 1: Declaring Structs ── */}
      <h2>Declaring Structs</h2>
      <p>
        Structs are declared with the <code>struct</code> keyword. Fields use
        colon-separated type annotations, just like variables. To implement an
        interface, use a colon after the struct name.
      </p>

      <CodeBlock code={`struct Person : IPrintable {
    Name: char*;
    age: i32;
}`} />

      {/* ── Section 2: Field Visibility ── */}
      <h2>Field Visibility</h2>
      <p>
        Field visibility is determined by the case of the first character of the
        field name. Fields that start with an <strong>uppercase</strong> letter are
        public; fields that start with a <strong>lowercase</strong> letter are
        private. This convention is inspired by Go.
      </p>

      <table className="doc-table">
        <thead>
          <tr>
            <th>First Character</th>
            <th>Visibility</th>
            <th>Access</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td>Uppercase (<code>A</code>&ndash;<code>Z</code>)</td>
            <td>Public</td>
            <td>Accessible from any module</td>
          </tr>
          <tr>
            <td>Lowercase (<code>a</code>&ndash;<code>z</code>)</td>
            <td>Private</td>
            <td>Accessible only within the declaring module</td>
          </tr>
        </tbody>
      </table>

      <h3>Full Example</h3>
      <p>
        The following struct demonstrates both public and private fields:
      </p>

      <CodeBlock code={`struct Person : IPrintable {
    // Public fields (uppercase first letter)
    Name: char*;
    Age: i32;

    // Private fields (lowercase first letter)
    id: i32;
    cache: char*;
}`} />

      <p>
        External code can read and write <code>Name</code> and <code>Age</code>,
        but <code>id</code> and <code>cache</code> are only accessible within the
        module that defines <code>Person</code>.
      </p>

      <Callout type="tip">
        Use uppercase names for fields that form your public API and lowercase
        names for internal implementation details. This makes visibility obvious
        at a glance without needing extra keywords.
      </Callout>

      {/* ── Section 3: Methods ── */}
      <h2>Methods</h2>
      <p>
        Methods are regular functions that take an explicit <code>self</code>{' '}
        parameter pointing to the struct instance.
      </p>

      <CodeBlock code={`fn print(self: Person*) {
    printf("Name: %s, Age: %d", self.Name, self.Age);
}

fn getName(self: Person*) -> char* {
    return self.Name;
}

fn setAge(self: Person*, newAge: i32) -> void {
    self.Age = newAge;
}`} />

      <Callout type="note">
        There is no implicit <code>this</code> pointer. The receiver is always
        an explicit parameter, making it clear which struct the method operates
        on. This follows the same philosophy as Python and Rust.
      </Callout>
    </>
  )
}
