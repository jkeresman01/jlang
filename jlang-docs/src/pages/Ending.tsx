import Callout from '../components/Callout';

export default function Ending() {
  return (
    <>
      <h1>The End</h1>
      <p>
        You've reached the end of the Jlang book. Writing in Jlang is hard work,
        so here's a funny football story before you go.
      </p>

      <Callout type="life-lesson">
        <strong>The Ballad of the Busted Knee</strong>
        <p>
          I hurt my knee in some dumb game one day,
          <br />
          an irrelevant match no one remembers anyway.
          <br />
          But a tournament was coming — Thursday, maybe not,
          <br />
          could've been a Wednesday, the memory's a blot.
        </p>
        <p>
          Went to the doctor, knee swollen with water inside,
          <br />
          he said "don't worry son, it won't hurt" — the man lied.
          <br />
          Stuck the needle in the wrong spot, said "oops, my mistake,"
          <br />
          then did it right the second time, for everybody's sake.
        </p>
        <p>
          The doctor said I can't play, he said rest that knee,
          <br />
          but I went to the tournament anyway, stubborn as can be.
          <br />
          Played every game, won some trophy on the shelf,
          <br />
          didn't get the girl though — ah well, I kept to myself.
        </p>
        <p>
          Looked through her Facebook profile just today,
          <br />
          not a shame, not a shame, is what I'll proudly say.
          <br />
          The tournament was nothing, no one really cared,
          <br />
          just a bunch of guys, a trophy no one shared.
        </p>
        <p>
          My knee still hurts, I feel it when the weather starts to change,
          <br />
          my grandma used to tell me that — I thought it sounded strange.
          <br />
          But grandma was right, I believe her now it's true,
          <br />
          just needed some time to understand what grandma always knew.
        </p>
        <p>
          Scored seven goals that semifinal day,
          <br />
          'cause the other guy had six in total, what can I say.
          <br />
          While watching Kobe's mamba mentality moments in my head.
        </p>
        <strong>Kobe Bryant &mdash; Mamba Mentality</strong>
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/sL8f6Dkc8Os?start=110"
          title="Kobe Bryant — Mamba Mentality"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>
    </>
  );
}
