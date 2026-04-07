import { Link } from 'react-router-dom';
import { chapters } from '../chapters';

const romanNumerals: [number, string][] = [
  [10, 'X'],
  [9, 'IX'],
  [5, 'V'],
  [4, 'IV'],
  [1, 'I'],
];

function toRoman(n: number): string {
  let result = '';
  for (const [value, numeral] of romanNumerals) {
    while (n >= value) {
      result += numeral;
      n -= value;
    }
  }
  return result;
}

export default function TableOfContents() {
  return (
    <>
      <h1>Table of Contents</h1>

      {chapters.map((chapter, chapterIdx) => (
        <div key={chapter.title} style={{ marginBottom: '1.5rem' }}>
          <h3 style={{ marginBottom: '0.5rem' }}>
            {toRoman(chapterIdx + 1)}. {chapter.title}
          </h3>
          <ul style={{ listStyle: 'none', padding: 0, margin: 0 }}>
            {chapter.items.map((item) => (
              <li key={item.path} style={{ marginBottom: '0.25rem' }}>
                <Link
                  to={item.path}
                  style={{ color: 'var(--link)', textDecoration: 'none' }}
                >
                  {item.label}
                </Link>
              </li>
            ))}
          </ul>
        </div>
      ))}
    </>
  );
}
