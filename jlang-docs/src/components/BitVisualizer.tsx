import { useState, useCallback } from 'react';

type Op = '&' | '|' | '^' | '~' | '<<' | '>>';

const OPS: { value: Op; label: string; title: string }[] = [
  { value: '&', label: 'AND', title: 'Bitwise AND — both bits must be 1' },
  { value: '|', label: 'OR', title: 'Bitwise OR — either bit can be 1' },
  { value: '^', label: 'XOR', title: 'Bitwise XOR — bits must differ' },
  { value: '~', label: 'NOT', title: 'Bitwise NOT — invert all bits of A' },
  { value: '<<', label: 'SHL', title: 'Shift left by B positions' },
  { value: '>>', label: 'SHR', title: 'Shift right by B positions' },
];

function compute(a: number, b: number, op: Op): number {
  switch (op) {
    case '&':
      return a & b;
    case '|':
      return a | b;
    case '^':
      return a ^ b;
    case '~':
      return ~a & 0xff;
    case '<<':
      return (a << b) & 0xff;
    case '>>':
      return (a >>> b) & 0xff;
  }
}

function toBits(n: number): number[] {
  const bits: number[] = [];
  for (let i = 7; i >= 0; i--) {
    bits.push((n >> i) & 1);
  }
  return bits;
}

function BitCell({
  value,
  onClick,
  highlight,
  dimmed,
}: {
  value: number;
  onClick?: () => void;
  highlight?: 'on' | 'off' | 'changed';
  dimmed?: boolean;
}) {
  const interactive = !!onClick;

  let bg = 'var(--bg-code)';
  let color = 'var(--text-muted)';
  let border = 'var(--border)';
  let shadow = 'none';

  if (value === 1) {
    color = 'var(--text-heading)';
    border = 'var(--accent-dim)';
  }

  if (highlight === 'on') {
    bg = 'rgba(108, 140, 255, 0.18)';
    color = 'var(--accent)';
    border = 'var(--accent)';
    shadow = '0 0 8px rgba(108, 140, 255, 0.25)';
  } else if (highlight === 'changed') {
    bg = 'rgba(52, 211, 153, 0.12)';
    color = 'var(--callout-tip)';
    border = 'var(--callout-tip)';
    shadow = '0 0 8px rgba(52, 211, 153, 0.2)';
  } else if (highlight === 'off') {
    bg = 'var(--bg-code)';
    color = 'var(--text-muted)';
    border = 'var(--border)';
  }

  if (dimmed) {
    color = 'var(--text-muted)';
    bg = 'var(--bg-code)';
    border = 'var(--border)';
    shadow = 'none';
  }

  return (
    <button
      onClick={onClick}
      disabled={!interactive}
      style={{
        width: 38,
        height: 38,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        fontFamily: "'JetBrains Mono', 'Fira Code', monospace",
        fontSize: '0.95rem',
        fontWeight: 600,
        background: bg,
        color,
        border: `1.5px solid ${border}`,
        borderRadius: 6,
        cursor: interactive ? 'pointer' : 'default',
        transition: 'all 0.15s ease',
        boxShadow: shadow,
        padding: 0,
        outline: 'none',
      }}
      aria-label={
        interactive ? `Toggle bit, currently ${value}` : `Bit value ${value}`
      }
    >
      {value}
    </button>
  );
}

function ValueLabel({ label, value }: { label: string; value: number }) {
  return (
    <span
      style={{
        display: 'flex',
        alignItems: 'center',
        gap: 6,
        fontSize: '0.8rem',
        color: 'var(--text-muted)',
        fontFamily: "'JetBrains Mono', monospace",
        minWidth: 90,
        flexShrink: 0,
      }}
    >
      <span style={{ color: 'var(--text-secondary)', fontWeight: 600 }}>
        {label}
      </span>
      <span style={{ color: 'var(--syntax-number)' }}>{value}</span>
      <span style={{ color: 'var(--text-muted)', fontSize: '0.75rem' }}>
        0x{value.toString(16).toUpperCase().padStart(2, '0')}
      </span>
    </span>
  );
}

export default function BitVisualizer() {
  const [a, setA] = useState(0b11001010); // 202
  const [b, setB] = useState(0b10110001); // 177
  const [op, setOp] = useState<Op>('&');

  const toggleBit = useCallback((which: 'a' | 'b', bitIndex: number) => {
    const setter = which === 'a' ? setA : setB;
    setter((prev) => prev ^ (1 << (7 - bitIndex)));
  }, []);

  const result = compute(a, b, op);
  const aBits = toBits(a);
  const bBits = toBits(b);
  const rBits = toBits(result);

  const isShift = op === '<<' || op === '>>';
  const isNot = op === '~';

  const getResultHighlight = (i: number): 'on' | 'off' | 'changed' => {
    const rb = rBits[i];
    const ab = aBits[i];
    if (rb === 1 && ab === 0) return 'changed';
    if (rb === 1) return 'on';
    return 'off';
  };

  const opInfo = OPS.find((o) => o.value === op)!;

  return (
    <div
      style={{
        margin: '1.5rem 0',
        padding: '1.25rem 1.5rem',
        background: 'var(--bg-secondary)',
        border: '1px solid var(--border)',
        borderRadius: 10,
        overflow: 'hidden',
      }}
    >
      {/* Header */}
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          justifyContent: 'space-between',
          marginBottom: '1rem',
          flexWrap: 'wrap',
          gap: '0.5rem',
        }}
      >
        <span
          style={{
            fontSize: '0.8rem',
            fontWeight: 600,
            color: 'var(--text-muted)',
            textTransform: 'uppercase',
            letterSpacing: '0.05em',
          }}
        >
          Bit Playground
        </span>
        <span
          style={{
            fontSize: '0.75rem',
            color: 'var(--text-muted)',
          }}
        >
          Click bits to toggle
        </span>
      </div>

      {/* Bit position labels */}
      <div
        style={{
          display: 'flex',
          gap: 4,
          marginBottom: 4,
          paddingLeft: 90,
        }}
      >
        {[7, 6, 5, 4, 3, 2, 1, 0].map((pos) => (
          <div
            key={pos}
            style={{
              width: 38,
              textAlign: 'center',
              fontSize: '0.65rem',
              color: 'var(--text-muted)',
              fontFamily: "'JetBrains Mono', monospace",
            }}
          >
            {pos}
          </div>
        ))}
      </div>

      {/* Row A */}
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          gap: 4,
          marginBottom: 6,
          flexWrap: 'wrap',
        }}
      >
        <ValueLabel label="A" value={a} />
        <div style={{ display: 'flex', gap: 4 }}>
          {aBits.map((bit, i) => (
            <BitCell key={i} value={bit} onClick={() => toggleBit('a', i)} />
          ))}
        </div>
      </div>

      {/* Operator row */}
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          gap: 8,
          margin: '8px 0',
          paddingLeft: 90,
        }}
      >
        <div
          style={{
            display: 'flex',
            gap: 4,
            flexWrap: 'wrap',
          }}
        >
          {OPS.map((o) => (
            <button
              key={o.value}
              onClick={() => setOp(o.value)}
              title={o.title}
              style={{
                padding: '4px 10px',
                fontSize: '0.75rem',
                fontWeight: 600,
                fontFamily: "'JetBrains Mono', monospace",
                background: op === o.value ? 'var(--accent)' : 'var(--bg-code)',
                color: op === o.value ? '#fff' : 'var(--text-secondary)',
                border: `1px solid ${op === o.value ? 'var(--accent)' : 'var(--border)'}`,
                borderRadius: 5,
                cursor: 'pointer',
                transition: 'all 0.15s ease',
                outline: 'none',
              }}
            >
              {o.label}
            </button>
          ))}
        </div>
        <span
          style={{
            fontSize: '0.75rem',
            color: 'var(--text-muted)',
            fontStyle: 'italic',
          }}
        >
          {opInfo.title}
        </span>
      </div>

      {/* Row B */}
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          gap: 4,
          marginBottom: 10,
          flexWrap: 'wrap',
          opacity: isNot ? 0.35 : 1,
          transition: 'opacity 0.2s',
        }}
      >
        <ValueLabel label="B" value={b} />
        <div style={{ display: 'flex', gap: 4 }}>
          {bBits.map((bit, i) => (
            <BitCell
              key={i}
              value={bit}
              onClick={isNot ? undefined : () => toggleBit('b', i)}
              dimmed={isNot}
            />
          ))}
        </div>
      </div>

      {/* Divider */}
      <div
        style={{
          borderTop: '1.5px solid var(--border)',
          margin: '4px 0 10px',
        }}
      />

      {/* Result row */}
      <div
        style={{
          display: 'flex',
          alignItems: 'center',
          gap: 4,
          flexWrap: 'wrap',
        }}
      >
        <ValueLabel label="=" value={result} />
        <div style={{ display: 'flex', gap: 4 }}>
          {rBits.map((bit, i) => (
            <BitCell key={i} value={bit} highlight={getResultHighlight(i)} />
          ))}
        </div>
      </div>

      {/* Expression */}
      <div
        style={{
          marginTop: 12,
          paddingLeft: 90,
          fontSize: '0.8rem',
          fontFamily: "'JetBrains Mono', monospace",
          color: 'var(--text-secondary)',
        }}
      >
        {isNot ? (
          <>
            <span style={{ color: 'var(--syntax-operator)' }}>~</span>
            <span style={{ color: 'var(--syntax-number)' }}>{a}</span>
            <span style={{ color: 'var(--text-muted)' }}> = </span>
            <span style={{ color: 'var(--accent)' }}>{result}</span>
          </>
        ) : isShift ? (
          <>
            <span style={{ color: 'var(--syntax-number)' }}>{a}</span>
            <span style={{ color: 'var(--syntax-operator)' }}> {op} </span>
            <span style={{ color: 'var(--syntax-number)' }}>{b}</span>
            <span style={{ color: 'var(--text-muted)' }}> = </span>
            <span style={{ color: 'var(--accent)' }}>{result}</span>
            {b > 7 && (
              <span
                style={{
                  color: 'var(--callout-warning)',
                  fontSize: '0.75rem',
                  marginLeft: 8,
                }}
              >
                shift exceeds 8 bits
              </span>
            )}
          </>
        ) : (
          <>
            <span style={{ color: 'var(--syntax-number)' }}>{a}</span>
            <span style={{ color: 'var(--syntax-operator)' }}> {op} </span>
            <span style={{ color: 'var(--syntax-number)' }}>{b}</span>
            <span style={{ color: 'var(--text-muted)' }}> = </span>
            <span style={{ color: 'var(--accent)' }}>{result}</span>
          </>
        )}
      </div>
    </div>
  );
}
