import { Link } from 'react-router-dom';

interface Video {
  title: string;
  chapter: string;
  chapterPath: string;
}

const videos: Video[] = [
  {
    title: 'Steve Jobs \u2014 "They paid me $60 to wear a tie"',
    chapter: 'Preface',
    chapterPath: '/preface',
  },
  {
    title: "Here's to the crazy ones",
    chapter: 'Preface',
    chapterPath: '/preface',
  },
  {
    title: 'Mourinho on his most problematic player',
    chapter: 'Preface',
    chapterPath: '/preface',
  },
  {
    title: 'AT&T Archives: The UNIX Operating System',
    chapter: 'Preface',
    chapterPath: '/preface',
  },
  {
    title: 'Understanding vs. performing',
    chapter: 'Introduction',
    chapterPath: '/',
  },
  {
    title: 'Erlang \u2014 Let it crash philosophy',
    chapter: 'Introduction',
    chapterPath: '/',
  },
  {
    title: 'Chris Lattner \u2014 "What is a compiler?"',
    chapter: 'Introduction',
    chapterPath: '/',
  },
  {
    title: 'Jonathan Blow \u2014 "Isn\'t this proof that a five-year-old is in charge of LLVM?"',
    chapter: 'Technologies Used',
    chapterPath: '/technologies-used',
  },
  {
    title: 'Inline keyword in C++ explained',
    chapter: 'Technologies Used',
    chapterPath: '/technologies-used',
  },
  {
    title: 'Linus Torvalds on good taste in code',
    chapter: 'Comments',
    chapterPath: '/comments',
  },
  {
    title: 'Bjarne Stroustrup on how many languages you should know',
    chapter: 'Comments',
    chapterPath: '/comments',
  },
  {
    title: 'Java vs C#',
    chapter: 'Comments',
    chapterPath: '/comments',
  },
  {
    title: 'Bentley Rules for Optimizing Work',
    chapter: 'Operators',
    chapterPath: '/operators',
  },
  {
    title: 'XOR swap explained',
    chapter: 'Operators',
    chapterPath: '/operators',
  },
  {
    title: "Conway's Game of Life",
    chapter: 'If / Else / Loops',
    chapterPath: '/control-flow',
  },
  {
    title: 'James Gosling on why Java removed pointers',
    chapter: 'Pointers',
    chapterPath: '/pointers',
  },
  {
    title: 'Kobe Bryant \u2014 Mamba Mentality',
    chapter: 'The End',
    chapterPath: '/ending',
  },
];

export default function TableOfVideos() {
  return (
    <>
      <h1>Table of Videos</h1>

      <p>
        A list of all videos featured throughout this book, in the order they
        appear.
      </p>

      <table
        style={{
          width: '100%',
          borderCollapse: 'collapse',
          marginTop: '1.5rem',
        }}
      >
        <thead>
          <tr
            style={{
              borderBottom: '2px solid var(--border)',
              textAlign: 'left',
            }}
          >
            <th style={{ padding: '0.5rem 0.75rem' }}>#</th>
            <th style={{ padding: '0.5rem 0.75rem' }}>Video</th>
            <th style={{ padding: '0.5rem 0.75rem' }}>Chapter</th>
          </tr>
        </thead>
        <tbody>
          {videos.map((video, idx) => (
            <tr
              key={idx}
              style={{ borderBottom: '1px solid var(--border)' }}
            >
              <td style={{ padding: '0.5rem 0.75rem', opacity: 0.6 }}>
                {idx + 1}
              </td>
              <td style={{ padding: '0.5rem 0.75rem' }}>{video.title}</td>
              <td style={{ padding: '0.5rem 0.75rem' }}>
                <Link
                  to={video.chapterPath}
                  style={{ color: 'var(--link)', textDecoration: 'none' }}
                >
                  {video.chapter}
                </Link>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </>
  );
}
