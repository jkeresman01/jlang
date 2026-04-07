import { Link } from 'react-router-dom';

interface Video {
  title: string;
  chapter: string;
  chapterPath: string;
  youtubeUrl: string;
}

const videos: Video[] = [
  {
    title: 'Steve Jobs \u2014 "They paid me $60 to wear a tie"',
    chapter: 'Preface',
    chapterPath: '/preface',
    youtubeUrl: 'https://www.youtube.com/watch?v=t9HmOz8H0qI',
  },
  {
    title: "Here's to the crazy ones",
    chapter: 'Preface',
    chapterPath: '/preface',
    youtubeUrl: 'https://www.youtube.com/watch?v=keCwRdbwNQY&t=198',
  },
  {
    title: 'Mourinho on his most problematic player',
    chapter: 'Preface',
    chapterPath: '/preface',
    youtubeUrl: 'https://www.youtube.com/watch?v=AwXgfBzK_L4&t=26',
  },
  {
    title: 'AT&T Archives: The UNIX Operating System',
    chapter: 'Preface',
    chapterPath: '/preface',
    youtubeUrl: 'https://www.youtube.com/watch?v=tc4ROCJYbm0',
  },
  {
    title: 'Understanding vs. performing',
    chapter: 'Introduction',
    chapterPath: '/',
    youtubeUrl: 'https://www.youtube.com/watch?v=Nb2tebYAaOA&t=1373',
  },
  {
    title: 'Erlang \u2014 Let it crash philosophy',
    chapter: 'Introduction',
    chapterPath: '/',
    youtubeUrl: 'https://www.youtube.com/watch?v=SOqQVoVai6s',
  },
  {
    title: 'Chris Lattner \u2014 "What is a compiler?"',
    chapter: 'Introduction',
    chapterPath: '/',
    youtubeUrl: 'https://www.youtube.com/watch?v=yCd3CzGSte8&t=2120',
  },
  {
    title: 'Jonathan Blow \u2014 "Isn\'t this proof that a five-year-old is in charge of LLVM?"',
    chapter: 'Technologies Used',
    chapterPath: '/technologies-used',
    youtubeUrl: 'https://www.youtube.com/watch?v=ns7jP1-SRvw&t=132',
  },
  {
    title: 'Inline keyword in C++ explained',
    chapter: 'Technologies Used',
    chapterPath: '/technologies-used',
    youtubeUrl: 'https://www.youtube.com/watch?v=ulJm7_aTiQM&t=3330',
  },
  {
    title: 'Linus Torvalds on good taste in code',
    chapter: 'Comments',
    chapterPath: '/comments',
    youtubeUrl: 'https://www.youtube.com/watch?v=o8NPllzkFhE&t=863',
  },
  {
    title: 'Bjarne Stroustrup on how many languages you should know',
    chapter: 'Comments',
    chapterPath: '/comments',
    youtubeUrl: 'https://www.youtube.com/watch?v=NvWTnIoQZj4',
  },
  {
    title: 'Java vs C#',
    chapter: 'Comments',
    chapterPath: '/comments',
    youtubeUrl: 'https://www.youtube.com/watch?v=RnqAXuLZlaE',
  },
  {
    title: 'Bentley Rules for Optimizing Work',
    chapter: 'Operators',
    chapterPath: '/operators',
    youtubeUrl: 'https://www.youtube.com/watch?v=H-1-X9bkop8&t=2890',
  },
  {
    title: 'XOR swap explained',
    chapter: 'Operators',
    chapterPath: '/operators',
    youtubeUrl: 'https://www.youtube.com/watch?v=ZusiKXcz_ac&t=1260',
  },
  {
    title: "Conway's Game of Life",
    chapter: 'If / Else / Loops',
    chapterPath: '/control-flow',
    youtubeUrl: 'https://www.youtube.com/watch?v=CgOcEZinQ2I',
  },
  {
    title: 'James Gosling on why Java removed pointers',
    chapter: 'Pointers',
    chapterPath: '/pointers',
    youtubeUrl: 'https://www.youtube.com/watch?v=RrMptmNYkSw',
  },
  {
    title: 'Kobe Bryant \u2014 Mamba Mentality',
    chapter: 'The End',
    chapterPath: '/ending',
    youtubeUrl: 'https://www.youtube.com/watch?v=sL8f6Dkc8Os&t=110',
  },
];

interface Image {
  title: string;
  chapter: string;
  chapterPath: string;
}

const images: Image[] = [
  {
    title: 'Unsolicited recruiter email',
    chapter: 'Preface',
    chapterPath: '/preface',
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
              <td style={{ padding: '0.5rem 0.75rem' }}>
                <a
                  href={video.youtubeUrl}
                  target="_blank"
                  rel="noopener noreferrer"
                  style={{ color: 'var(--link)', textDecoration: 'none' }}
                >
                  {video.title}
                </a>
              </td>
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

      <h1 style={{ marginTop: '3rem' }}>Table of Images</h1>

      <p>
        A list of all images featured throughout this book, in the order they
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
            <th style={{ padding: '0.5rem 0.75rem' }}>Image</th>
            <th style={{ padding: '0.5rem 0.75rem' }}>Chapter</th>
          </tr>
        </thead>
        <tbody>
          {images.map((image, idx) => (
            <tr
              key={idx}
              style={{ borderBottom: '1px solid var(--border)' }}
            >
              <td style={{ padding: '0.5rem 0.75rem', opacity: 0.6 }}>
                {idx + 1}
              </td>
              <td style={{ padding: '0.5rem 0.75rem' }}>{image.title}</td>
              <td style={{ padding: '0.5rem 0.75rem' }}>
                <Link
                  to={image.chapterPath}
                  style={{ color: 'var(--link)', textDecoration: 'none' }}
                >
                  {image.chapter}
                </Link>
              </td>
            </tr>
          ))}
        </tbody>
      </table>
    </>
  );
}
