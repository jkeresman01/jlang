import { NavLink } from 'react-router-dom'

const chapters = [
  {
    title: 'Getting Started',
    items: [
      { label: 'Introduction', path: '/' },
      { label: 'Installation & Build', path: '/getting-started' },
    ],
  },
  {
    title: 'Language Basics',
    items: [
      { label: 'Variables', path: '/variables' },
      { label: 'Types', path: '/types' },
      { label: 'Functions', path: '/functions' },
    ],
  },
  {
    title: 'Control Flow',
    items: [
      { label: 'If / Else / Loops', path: '/control-flow' },
    ],
  },
  {
    title: 'Data Structures',
    items: [
      { label: 'Structs', path: '/structs' },
      { label: 'Arrays', path: '/arrays' },
      { label: 'Generics', path: '/generics' },
    ],
  },
  {
    title: 'Safety & Error Handling',
    items: [
      { label: 'Null Safety', path: '/null-safety' },
      { label: 'Error Handling', path: '/error-handling' },
    ],
  },
  {
    title: 'Operators',
    items: [
      { label: 'Operators', path: '/operators' },
    ],
  },
  {
    title: 'Memory',
    items: [
      { label: 'Memory Management', path: '/memory' },
    ],
  },
]

interface Props {
  open: boolean
  onClose: () => void
}

export default function Sidebar({ open, onClose }: Props) {
  return (
    <>
      {open && <div className="sidebar-overlay" onClick={onClose} />}
      <aside className={`sidebar ${open ? 'sidebar--open' : ''}`}>
        <div className="sidebar-header">
          <NavLink to="/" className="sidebar-logo" onClick={onClose}>
            <span className="sidebar-logo-text">jlang</span>
            <span className="sidebar-logo-badge">docs</span>
          </NavLink>
        </div>
        <nav className="sidebar-nav">
          {chapters.map((chapter) => (
            <div key={chapter.title} className="sidebar-section">
              <h3 className="sidebar-section-title">{chapter.title}</h3>
              <ul className="sidebar-list">
                {chapter.items.map((item) => (
                  <li key={item.path}>
                    <NavLink
                      to={item.path}
                      end={item.path === '/'}
                      className={({ isActive }) =>
                        `sidebar-link ${isActive ? 'sidebar-link--active' : ''}`
                      }
                      onClick={onClose}
                    >
                      {item.label}
                    </NavLink>
                  </li>
                ))}
              </ul>
            </div>
          ))}
        </nav>
        <div className="sidebar-footer">
          <a
            href="https://github.com/jlang"
            target="_blank"
            rel="noopener noreferrer"
            className="sidebar-github"
          >
            <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor">
              <path d="M12 0c-6.626 0-12 5.373-12 12 0 5.302 3.438 9.8 8.207 11.387.599.111.793-.261.793-.577v-2.234c-3.338.726-4.033-1.416-4.033-1.416-.546-1.387-1.333-1.756-1.333-1.756-1.089-.745.083-.729.083-.729 1.205.084 1.839 1.237 1.839 1.237 1.07 1.834 2.807 1.304 3.492.997.107-.775.418-1.305.762-1.604-2.665-.305-5.467-1.334-5.467-5.931 0-1.311.469-2.381 1.236-3.221-.124-.303-.535-1.524.117-3.176 0 0 1.008-.322 3.301 1.23.957-.266 1.983-.399 3.003-.404 1.02.005 2.047.138 3.006.404 2.291-1.552 3.297-1.23 3.297-1.23.653 1.653.242 2.874.118 3.176.77.84 1.235 1.911 1.235 3.221 0 4.609-2.807 5.624-5.479 5.921.43.372.823 1.102.823 2.222v3.293c0 .319.192.694.801.576 4.765-1.589 8.199-6.086 8.199-11.386 0-6.627-5.373-12-12-12z" />
            </svg>
            GitHub
          </a>
        </div>
      </aside>
    </>
  )
}
