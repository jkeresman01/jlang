import { useState, useEffect, useCallback } from 'react';
import { useLocation } from 'react-router-dom';

interface Hint {
  id: string;
  type: 'life-lesson' | 'tip';
  label: string;
  top: number;
}

export default function ScrollHints() {
  const [hints, setHints] = useState<Hint[]>([]);
  const [hovered, setHovered] = useState<string | null>(null);
  const location = useLocation();

  const scan = useCallback(() => {
    const docHeight = document.documentElement.scrollHeight;
    if (docHeight <= 0) return;

    const found: Hint[] = [];

    document
      .querySelectorAll('.callout--life-lesson, .callout--tip')
      .forEach((el, i) => {
        const rect = el.getBoundingClientRect();
        const absTop = rect.top + window.scrollY;
        const pct = (absTop / docHeight) * 100;

        const isLifeLesson = el.classList.contains('callout--life-lesson');
        found.push({
          id: `${isLifeLesson ? 'll' : 'ff'}-${i}`,
          type: isLifeLesson ? 'life-lesson' : 'tip',
          label: isLifeLesson ? 'Life Lesson' : 'Fun Fact',
          top: pct,
        });
      });

    setHints(found);
  }, []);

  useEffect(() => {
    // Delay scan to let the page render
    const timer = setTimeout(scan, 300);
    return () => clearTimeout(timer);
  }, [location.pathname, scan]);

  // Rescan on resize (positions change)
  useEffect(() => {
    window.addEventListener('resize', scan, { passive: true });
    return () => window.removeEventListener('resize', scan);
  }, [scan]);

  if (hints.length === 0) return null;

  return (
    <div className="scroll-hints">
      {hints.map((h) => (
        <button
          key={h.id}
          className={`scroll-hint scroll-hint--${h.type}`}
          style={{ top: `${h.top}%` }}
          onMouseEnter={() => setHovered(h.id)}
          onMouseLeave={() => setHovered(null)}
          onClick={() => {
            const idx = parseInt(h.id.split('-')[1]);
            const all = document.querySelectorAll(
              '.callout--life-lesson, .callout--tip',
            );
            if (all[idx]) {
              all[idx].scrollIntoView({ behavior: 'smooth', block: 'center' });
            }
          }}
          aria-label={`Scroll to ${h.label}`}
        >
          <span
            className={`scroll-hint-label ${hovered === h.id ? 'scroll-hint-label--visible' : ''}`}
          >
            {h.label}
          </span>
        </button>
      ))}
    </div>
  );
}
