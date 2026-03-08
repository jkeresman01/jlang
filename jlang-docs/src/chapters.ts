export const chapters = [
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
      { label: 'Comments', path: '/comments' },
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

export const allPages = chapters.flatMap((ch) => ch.items)
