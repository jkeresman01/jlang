import CodeBlock from '../components/CodeBlock'

export default function Types() {
  return (
    <>
      <h1>Types &amp; Literals</h1>

      <h2>Integer Sizes</h2>
      <p>
        Jlang supports all standard integer types: <code>i8</code>, <code>i16</code>,{' '}
        <code>i32</code>, <code>i64</code>, <code>u8</code>, <code>u16</code>,{' '}
        <code>u32</code>, and <code>u64</code>. Integer literals are parsed as{' '}
        <code>i32</code> by default.
      </p>
      <CodeBlock
        code={`var a: i8 = 127;
var b: i16 = 32767;
var c: i32 = 42;
var d: i64 = 9223372036854775807;
var e: u8 = 255;
var f: u16 = 65535;
var g: u32 = 100;
var h: u64 = 18446744073709551615;`}
      />

      <h2>Floating Point</h2>
      <p>
        Single-precision (<code>f32</code>) and double-precision (<code>f64</code>)
        floating-point types are available.
      </p>
      <CodeBlock
        code={`var pi: f32 = 3.14;
var precise: f64 = 3.141592653589793;`}
      />

      <h2>Character Literals</h2>
      <p>
        Character literals use single quotes and support escape sequences (
        <code>\n</code>, <code>\t</code>, <code>\r</code>, <code>\\</code>,{' '}
        <code>\'</code>, <code>\0</code>). Characters are stored as <code>i8</code>{' '}
        values.
      </p>
      <CodeBlock
        code={`var letter: char = 'A';
var newline: char = '\\n';`}
      />

      <h2>Comments</h2>
      <p>
        Jlang supports single-line comments with <code>//</code> and multi-line block
        comments with <code>/* */</code>.
      </p>
      <CodeBlock
        code={`// Single-line comment
/* Multi-line
   block comment */
var x: i32 = 42; // inline comment`}
      />

      <h2>Type Reference</h2>
      <table className="doc-table">
        <thead>
          <tr>
            <th>Type</th>
            <th>Description</th>
            <th>Size</th>
            <th>Range/Values</th>
          </tr>
        </thead>
        <tbody>
          <tr>
            <td><code>i8</code></td>
            <td>Signed 8-bit integer</td>
            <td>1 byte</td>
            <td>-128 to 127</td>
          </tr>
          <tr>
            <td><code>i16</code></td>
            <td>Signed 16-bit integer</td>
            <td>2 bytes</td>
            <td>-32,768 to 32,767</td>
          </tr>
          <tr>
            <td><code>i32</code></td>
            <td>Signed 32-bit integer</td>
            <td>4 bytes</td>
            <td>-2^31 to 2^31-1</td>
          </tr>
          <tr>
            <td><code>i64</code></td>
            <td>Signed 64-bit integer</td>
            <td>8 bytes</td>
            <td>-2^63 to 2^63-1</td>
          </tr>
          <tr>
            <td><code>u8</code></td>
            <td>Unsigned 8-bit integer</td>
            <td>1 byte</td>
            <td>0 to 255</td>
          </tr>
          <tr>
            <td><code>u16</code></td>
            <td>Unsigned 16-bit integer</td>
            <td>2 bytes</td>
            <td>0 to 65,535</td>
          </tr>
          <tr>
            <td><code>u32</code></td>
            <td>Unsigned 32-bit integer</td>
            <td>4 bytes</td>
            <td>0 to 2^32-1</td>
          </tr>
          <tr>
            <td><code>u64</code></td>
            <td>Unsigned 64-bit integer</td>
            <td>8 bytes</td>
            <td>0 to 2^64-1</td>
          </tr>
          <tr>
            <td><code>f32</code></td>
            <td>Single-precision float</td>
            <td>4 bytes</td>
            <td>&plusmn;3.4&times;10^38</td>
          </tr>
          <tr>
            <td><code>f64</code></td>
            <td>Double-precision float</td>
            <td>8 bytes</td>
            <td>&plusmn;1.8&times;10^308</td>
          </tr>
          <tr>
            <td><code>bool</code></td>
            <td>Boolean</td>
            <td>1 bit</td>
            <td>true or false</td>
          </tr>
          <tr>
            <td><code>char</code></td>
            <td>Character</td>
            <td>1 byte</td>
            <td>ASCII 0-255</td>
          </tr>
          <tr>
            <td><code>char*</code></td>
            <td>String (char pointer)</td>
            <td>8 bytes</td>
            <td>Memory address</td>
          </tr>
          <tr>
            <td><code>void</code></td>
            <td>No value</td>
            <td>-</td>
            <td>Functions only</td>
          </tr>
          <tr>
            <td><code>Type[N]</code></td>
            <td>Fixed-size array</td>
            <td>N &times; element</td>
            <td>Stack-allocated</td>
          </tr>
          <tr>
            <td><code>Result&lt;T, E&gt;</code></td>
            <td>Success or error</td>
            <td>varies</td>
            <td>Ok(T) or Err(E)</td>
          </tr>
        </tbody>
      </table>
    </>
  )
}
