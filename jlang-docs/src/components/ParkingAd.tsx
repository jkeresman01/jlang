import { useEffect, useRef, useState } from 'react';

export default function ParkingAd() {
  const [visible, setVisible] = useState(false);
  const [dismissed, setDismissed] = useState(false);
  const sentinelRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    if (dismissed) return;
    const el = sentinelRef.current;
    if (!el) return;

    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) setVisible(true);
      },
      { threshold: 0.3 },
    );
    observer.observe(el);
    return () => observer.disconnect();
  }, [dismissed]);

  return (
    <>
      <div ref={sentinelRef} style={{ height: 1 }} />
      {visible && !dismissed && (
        <div
          style={{
            position: 'fixed',
            right: '24px',
            bottom: '24px',
            zIndex: 999,
            width: '300px',
            background: 'var(--bg-secondary)',
            border: '2px solid var(--callout-warning)',
            borderRadius: '12px',
            padding: '20px',
            boxShadow: '0 12px 40px rgba(0, 0, 0, 0.35)',
            animation:
              'parking-ad-slide-in 0.5s cubic-bezier(0.22, 1, 0.36, 1)',
            fontFamily: "'Inter', sans-serif",
          }}
        >
          <style>{`
            @keyframes parking-ad-slide-in {
              from { opacity: 0; transform: translateX(120%); }
              to { opacity: 1; transform: translateX(0); }
            }
          `}</style>
          <button
            onClick={() => setDismissed(true)}
            aria-label="Close"
            style={{
              position: 'absolute',
              top: '8px',
              right: '10px',
              background: 'none',
              border: 'none',
              color: 'var(--text-muted)',
              fontSize: '1.1rem',
              cursor: 'pointer',
              lineHeight: 1,
            }}
          >
            x
          </button>
          <div style={{ fontSize: '28px', marginBottom: '8px' }}>🅿️</div>
          <div
            style={{
              fontSize: '0.95rem',
              fontWeight: 700,
              color: 'var(--callout-warning)',
              marginBottom: '6px',
              textTransform: 'uppercase',
              letterSpacing: '0.04em',
            }}
          >
            Urgent Notice
          </div>
          <p
            style={{
              fontSize: '0.85rem',
              color: 'var(--text-secondary)',
              lineHeight: 1.55,
              margin: '0 0 12px 0',
            }}
          >
            The author of this documentation owes{' '}
            <strong style={{ color: 'var(--text-primary)' }}>120 euros</strong>{' '}
            in parking fines to Prora&#x10D;un Grada Zagreba. Any donations
            toward keeping him out of debt collections are appreciated.
          </p>
          <div
            style={{
              fontSize: '0.75rem',
              color: 'var(--text-muted)',
              fontStyle: 'italic',
            }}
          >
            This ad will self-destruct once the fines are paid. So probably
            never.
          </div>
        </div>
      )}
    </>
  );
}
