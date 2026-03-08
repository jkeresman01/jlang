import { Link, useLocation } from 'react-router-dom'
import { allPages } from '../chapters'

export default function PageNav() {
  const { pathname } = useLocation()
  const index = allPages.findIndex((p) => p.path === pathname)

  if (index === -1) return null

  const prev = index > 0 ? allPages[index - 1] : null
  const next = index < allPages.length - 1 ? allPages[index + 1] : null

  return (
    <nav className="page-nav">
      {prev ? (
        <Link to={prev.path} className="page-nav-link page-nav-link--prev">
          <span className="page-nav-direction">Previous</span>
          <span className="page-nav-label">{prev.label}</span>
        </Link>
      ) : (
        <span />
      )}
      {next ? (
        <Link to={next.path} className="page-nav-link page-nav-link--next">
          <span className="page-nav-direction">Next</span>
          <span className="page-nav-label">{next.label}</span>
        </Link>
      ) : (
        <span />
      )}
    </nav>
  )
}
