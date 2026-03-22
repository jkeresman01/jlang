export const chapters = [
  {
    title: 'Getting Started',
    items: [{ label: 'Introduction', path: '/' }],
  },
];

export const allPages = chapters.flatMap((ch) => ch.items);
