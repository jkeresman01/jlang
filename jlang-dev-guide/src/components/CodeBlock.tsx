import { useEffect, useRef } from 'react';
import Prism from 'prismjs';
import 'prismjs/themes/prism-tomorrow.css';

// Register a custom jlang grammar based on C-like syntax
Prism.languages['jlang'] = {
  comment: [
    { pattern: /\/\/.*/, greedy: true },
    { pattern: /\/\*[\s\S]*?\*\//, greedy: true },
  ],
  preprocessor: {
    pattern: /^#\s*(?:include|define|ifdef|ifndef|else|endif|undef)\b.*/m,
    greedy: true,
    alias: 'property',
  },
  string: [
    { pattern: /"(?:\\.|[^"\\])*"/, greedy: true },
    { pattern: /'(?:\\.|[^'\\])'/, greedy: true },
  ],
  keyword:
    /\b(?:fn|var|val|return|if|else|while|for|in|struct|interface|switch|case|default|break|continue|match|alloc|free|null|true|false|and|or|Ok|Err|Result)\b/,
  builtin: /\b(?:printf|sizeof)\b/,
  'class-name': [
    /\b(?:i8|i16|i32|i64|u8|u16|u32|u64|f32|f64|bool|char|void)\b/,
    /\bstd::Vector\b/,
    /\b[A-Z][a-zA-Z0-9]*\b/,
  ],
  number: /\b\d+(?:\.\d+)?\b/,
  operator: /=>|->|:=|\?\:|\+\+|--|&&|\|\||[+\-*/%=<>!&|^~]=?|<<=?|>>=?|::/,
  punctuation: /[{}[\]();,.:]/,
};

// Alias so ```rust blocks also use our grammar
Prism.languages['rust'] = Prism.languages['jlang'];

interface Props {
  code: string;
  language?: string;
}

export default function CodeBlock({ code, language = 'jlang' }: Props) {
  const ref = useRef<HTMLElement>(null);

  useEffect(() => {
    if (ref.current) {
      Prism.highlightElement(ref.current);
    }
  }, [code]);

  return (
    <div className="code-block">
      <pre>
        <code ref={ref} className={`language-${language}`}>
          {code.trim()}
        </code>
      </pre>
    </div>
  );
}
