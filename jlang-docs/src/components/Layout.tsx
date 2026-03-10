import { useState, useEffect } from 'react'
import { Outlet, useLocation } from 'react-router-dom'
import Sidebar from './Sidebar'
import ThemeToggle from './ThemeToggle'
import Paywall from './Paywall'
import PageNav from './PageNav'
import ScrollHints from './ScrollHints'

export default function Layout() {
  const [sidebarOpen, setSidebarOpen] = useState(
    () => typeof window !== 'undefined' && window.innerWidth > 860
  )
  const location = useLocation()

  const [scrollProgress, setScrollProgress] = useState(0)

  useEffect(() => {
    window.scrollTo(0, 0)
    if (window.innerWidth <= 860) {
      setSidebarOpen(false)
    }
  }, [location.pathname])

  useEffect(() => {
    const handleScroll = () => {
      const { scrollTop, scrollHeight, clientHeight } = document.documentElement
      const progress = scrollHeight <= clientHeight ? 0 : scrollTop / (scrollHeight - clientHeight)
      setScrollProgress(progress)
    }
    window.addEventListener('scroll', handleScroll, { passive: true })
    return () => window.removeEventListener('scroll', handleScroll)
  }, [])

  return (
    <div className={`layout ${sidebarOpen ? 'layout--sidebar-open' : ''}`}>
      <Sidebar open={sidebarOpen} onClose={() => setSidebarOpen(false)} />
      <div className="layout-main">
        <div className="scroll-progress" style={{ transform: `scaleX(${scrollProgress})` }} />
        <header className="header">
          <button
            className={`header-menu-btn ${sidebarOpen ? 'header-menu-btn--open' : ''}`}
            onClick={() => setSidebarOpen(!sidebarOpen)}
            aria-label="Toggle menu"
          >
            {sidebarOpen ? (
              <svg className="menu-arrow" width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2" strokeLinecap="round" strokeLinejoin="round">
                <polyline points="15 18 9 12 15 6" />
              </svg>
            ) : (
              <svg width="22" height="22" viewBox="0 0 24 24" fill="none" stroke="currentColor" strokeWidth="2">
                <line x1="3" y1="6" x2="21" y2="6" />
                <line x1="3" y1="12" x2="21" y2="12" />
                <line x1="3" y1="18" x2="21" y2="18" />
              </svg>
            )}
          </button>
          <span className="header-title">25 Years of Thoughts, Compiled into a Language</span>
          <div className="header-spacer" />
          <ThemeToggle />
        </header>
        <main className="content">
          <div className="content-inner">
            <Outlet />
            <PageNav />
          </div>
        </main>
      </div>
      <Paywall />
      <ScrollHints />
    </div>
  )
}
