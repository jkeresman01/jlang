import { useState, useEffect, useCallback } from 'react';
import confetti from 'canvas-confetti';

const JOKES = [
  'Just kidding! This is free and open source.',
  'Gotcha! Knowledge should be free.',
  "Ha! You thought we'd charge for docs?",
  'Fooled you! Enjoy the free docs.',
  'Nice try, capitalism! Docs are free.',
];

export default function Paywall() {
  const [visible, setVisible] = useState(false);
  const [dismissed, setDismissed] = useState(false);
  const [jokeText, setJokeText] = useState('');

  useEffect(() => {
    if (dismissed) return;
    const timer = setTimeout(() => setVisible(true), 1500);
    return () => clearTimeout(timer);
  }, [dismissed]);

  const fireConfetti = useCallback(() => {
    const duration = 2000;
    const end = Date.now() + duration;

    const frame = () => {
      confetti({
        particleCount: 4,
        angle: 60,
        spread: 55,
        origin: { x: 0, y: 0.6 },
        colors: ['#6c8cff', '#c792ea', '#c3e88d', '#f78c6c', '#ffcb6b'],
      });
      confetti({
        particleCount: 4,
        angle: 120,
        spread: 55,
        origin: { x: 1, y: 0.6 },
        colors: ['#6c8cff', '#c792ea', '#c3e88d', '#f78c6c', '#ffcb6b'],
      });
      if (Date.now() < end) requestAnimationFrame(frame);
    };
    frame();

    confetti({
      particleCount: 100,
      spread: 100,
      origin: { y: 0.5 },
      colors: ['#6c8cff', '#c792ea', '#c3e88d', '#f78c6c', '#ffcb6b'],
    });
  }, []);

  const handleClick = useCallback(() => {
    const joke = JOKES[Math.floor(Math.random() * JOKES.length)];
    setJokeText(joke);
    fireConfetti();
    setTimeout(() => {
      setVisible(false);
      setDismissed(true);
    }, 2500);
  }, [fireConfetti]);

  if (!visible || dismissed) return null;

  return (
    <div
      style={{
        position: 'fixed',
        inset: 0,
        zIndex: 9999,
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center',
        backdropFilter: 'blur(8px)',
        WebkitBackdropFilter: 'blur(8px)',
        backgroundColor: 'rgba(0, 0, 0, 0.5)',
        animation: 'paywall-fade-in 0.4s ease-out',
      }}
    >
      <style>{`
        @keyframes paywall-fade-in {
          from { opacity: 0; }
          to { opacity: 1; }
        }
        @keyframes paywall-card-in {
          from { opacity: 0; transform: scale(0.9) translateY(20px); }
          to { opacity: 1; transform: scale(1) translateY(0); }
        }
        @keyframes paywall-joke-in {
          from { opacity: 0; transform: scale(0.8); }
          to { opacity: 1; transform: scale(1); }
        }
        .paywall-btn {
          padding: 14px 32px;
          border: none;
          border-radius: 10px;
          font-size: 1rem;
          font-weight: 600;
          cursor: pointer;
          transition: transform 0.15s, box-shadow 0.15s;
          font-family: 'Inter', sans-serif;
        }
        .paywall-btn:hover {
          transform: translateY(-2px);
          box-shadow: 0 6px 20px rgba(108, 140, 255, 0.3);
        }
        .paywall-btn:active {
          transform: translateY(0);
        }
      `}</style>
      <div
        style={{
          background: 'var(--bg-secondary)',
          border: '1px solid var(--border)',
          borderRadius: '16px',
          padding: '48px 40px',
          maxWidth: '460px',
          width: '90%',
          textAlign: 'center',
          animation: 'paywall-card-in 0.4s ease-out',
          boxShadow: '0 25px 60px rgba(0, 0, 0, 0.4)',
        }}
      >
        {!jokeText ? (
          <>
            <div style={{ fontSize: '48px', marginBottom: '16px' }}>🔒</div>
            <h2
              style={{
                color: 'var(--text-heading)',
                fontSize: '1.5rem',
                fontWeight: 700,
                margin: '0 0 8px 0',
                fontFamily: "'Inter', sans-serif",
              }}
            >
              Premium Content
            </h2>
            <p
              style={{
                color: 'var(--text-secondary)',
                fontSize: '0.95rem',
                margin: '0 0 32px 0',
                lineHeight: 1.6,
              }}
            >
              You've reached the free preview limit. Pay a small fee to continue
              reading the Jlang documentation.
            </p>
            <div
              style={{
                display: 'flex',
                gap: '12px',
                justifyContent: 'center',
                flexWrap: 'wrap',
              }}
            >
              <button
                className="paywall-btn"
                onClick={handleClick}
                style={{
                  background: 'linear-gradient(135deg, #6c8cff, #c792ea)',
                  color: '#fff',
                }}
              >
                Pay 2 Euros
              </button>
              <button
                className="paywall-btn"
                onClick={handleClick}
                style={{
                  background: 'var(--bg-hover)',
                  color: 'var(--text-primary)',
                  border: '1px solid var(--border)',
                }}
              >
                Cancel
              </button>
            </div>
          </>
        ) : (
          <div style={{ animation: 'paywall-joke-in 0.3s ease-out' }}>
            <div style={{ fontSize: '64px', marginBottom: '16px' }}>🎉</div>
            <p
              style={{
                color: 'var(--text-heading)',
                fontSize: '1.4rem',
                fontWeight: 700,
                margin: 0,
                fontFamily: "'Inter', sans-serif",
                lineHeight: 1.5,
              }}
            >
              {jokeText}
            </p>
          </div>
        )}
      </div>
    </div>
  );
}
