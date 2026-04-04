export const chapters = [
  {
    title: 'Getting Started',
    items: [
      { label: 'Preface', path: '/preface' },
      { label: 'Introduction', path: '/' },
      { label: 'Technologies Used', path: '/technologies-used' },
      { label: 'Installation & Build', path: '/getting-started' },
    ],
  },
  {
    title: 'Language Basics',
    items: [
      { label: 'Comments', path: '/comments' },
      { label: 'Variables', path: '/variables' },
      { label: 'Types', path: '/types' },
      { label: 'Operators', path: '/operators' },
    ],
  },
  {
    title: 'Control Flow',
    items: [
      { label: 'If / Else / Loops', path: '/control-flow' },
      { label: 'Functions', path: '/functions' },
    ],
  },
  {
    title: 'Data Structures',
    items: [
      { label: 'Arrays', path: '/arrays' },
      { label: 'Structs', path: '/structs' },
    ],
  },
  {
    title: 'Pointers & Memory',
    items: [
      { label: 'Pointers', path: '/pointers' },
      { label: 'Null Safety', path: '/null-safety' },
      { label: 'Memory Management', path: '/memory' },
    ],
  },
  {
    title: 'Advanced Features',
    items: [
      { label: 'Generics', path: '/generics' },
      { label: 'Error Handling', path: '/error-handling' },
    ],
  },
  {
    title: 'Epilogue',
    items: [{ label: 'The End', path: '/ending' }],
  },
];

export const allPages = chapters.flatMap((ch) => ch.items);
