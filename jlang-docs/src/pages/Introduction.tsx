import CodeBlock from '../components/CodeBlock'
import Callout from '../components/Callout'

export default function Introduction() {
  return (
    <>
      <section className="hero">
        <h1>jlang</h1>
        <p className="tagline">
          Journey Through a 25-Year-Old&rsquo;s Thoughts, Bundled with a Language
        </p>
        <p className="sub-tagline">
          A procedural programming language mostly inspired by Go/Rust &mdash; clean syntax,
          explicit memory management, and modern language features compiled to native code via LLVM.
        </p>
        <div className="hero-badges">
          <span className="hero-badge">C++</span>
          <span className="hero-badge">LLVM</span>
        </div>
      </section>

      <Callout type="note">
        This project is still very much a work in progress. Syntax and features
        may change.
      </Callout>

      <Callout type="warning">
        jlang is in its early stages &mdash; things may not work as expected or
        may be outright broken. As the language evolves, parts of this
        documentation can fall out of date or become inaccurate. If something
        looks off, it probably is. Corrections and feedback are always welcome.
      </Callout>

      <h2>About Me</h2>
      <p>
        I am a clueless twenty-five-year-old who programs every day and is now,
        for reasons I cannot fully explain, writing this. If you ever see me in
        person, you will probably see me in a black hoodie, staring at nothing in
        particular, and you might wonder what is going on in that guy&rsquo;s
        head &mdash; or why he codes every day, or why he thinks he can write
        something that people would actually read. Truth be told, I do not know
        the answer to that last one. But I can try to answer the rest.
      </p>
      <p>
        I code every day because it is an escape. An escape from this cruel world
        of judgment, fear, and wars. An escape from your everyday troubles, from
        the fight with someone you love. An escape into a world of thoughts,
        creation, and innovation. When you are deep enough inside a problem, the
        rest of the world simply stops existing.
      </p>
      <p>
        And why write this? Truth be told, because I am terrified. Terrified that
        my obsession is coming to an end. I have done every programming project I
        have ever wanted to do, and now I am terrified that I will drift back into
        the world of reality. So this seemed like the next natural step &mdash;
        one more thing to keep me in this world a little longer.
      </p>

      <p>
        In programming, you can get stuck on a problem and keep thinking about it
        for days. I sometimes walk for half an hour to find my car because I have
        completely forgotten where I parked it &mdash; too deep inside this
        wonderful world to notice something as trivial as where I left a two-ton
        piece of metal. Sometimes I walk right past it, more than once, without
        realizing it is sitting right in front of my nose. Sometimes I walk down
        the street and hit a traffic light pole. It is probably a good thing that
        every week or two I get a parking ticket &mdash; I have started to
        remember spots by the tickets I have collected there, and at this point
        they have become almost enjoyable to pay. In fact, I got one just today
        &mdash; or yesterday, I am not sure if it was before or after midnight.
      </p>
      <p>
        I have also realized that the best books and the best authors are those
        who speak out of their own experience &mdash; the ones in whose skin you
        can walk, the ones who can make you feel something. Sometimes it is
        frustration. Sometimes it is admiration. Sometimes it is hate. Sometimes
        your eyes fill with tears of joy. But the thing about people like that is
        that you can feel their passion and their dedication bleeding through
        every page.
      </p>
      <p>
        Robert Nystrom, in the acknowledgements
        of <em>Crafting Interpreters</em>, writes:
      </p>
      <blockquote style={{ borderLeft: '3px solid var(--border)', paddingLeft: '1rem', margin: '1.5rem 0', fontStyle: 'italic', opacity: 0.85 }}>
        Finally, the deepest thanks go to my wife Megan and my daughters Lily and
        Gretchen. You patiently endured the time I had to sink into the book, and
        my stress while writing it. There&rsquo;s no one I&rsquo;d rather be
        stuck at home with.
      </blockquote>
      <p>
        You can feel his obsession and his love toward the thing he is talking
        about &mdash; and toward the people who gave him the space to pursue it.
      </p>
      <p>
        So here are my apologies. I am sorry that I have been so absent to my
        family while doing all of this. I am sorry to the police officer who had
        to let me go when he asked for my identity card &mdash; I had not found
        enough time in the last two years to renew it. And I am sorry to the
        parking ticket guy.
      </p>
      <p>
        People with that kind of passion &mdash; the ability to move others, to
        make them feel something real &mdash; are extremely rare. You can glorify
        them or vilify them, but the only thing you cannot do is ignore them.
        Because the people who are crazy enough to think they can change the world
        are the ones who do.
      </p>

      <Callout type="note">
        <strong>Here&rsquo;s to the crazy ones</strong>
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/keCwRdbwNQY?start=198"
          title="Here's to the crazy ones"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <p>
        So in this book I will try to make you feel &mdash; to look at
        programming through my eyes and see the core beliefs that, through my
        experiences, have shaped me into the person I am today: craftsmanship,
        discipline, dedication, integrity, and passion.
      </p>
      <p>
        But passion is not the only thing that makes us feel. Our insecurities
        do too &mdash; sometimes even more so. Throughout this book you will find
        examples that were born not from confidence, but from moments where I had
        none. Moments of imposter syndrome, of not knowing something I felt I
        should have known. In the chapter on operators you
        will find an example about endian swapping. It earned its place there
        because I still remember filing my first bug report &mdash; a crash
        during an endian swap &mdash; and not being entirely clear on what an
        endian even was. The embarrassment of that moment has never fully left
        me. There are many more examples like that scattered across these pages.
        They are here because they are honest, and because the things that
        embarrass us tend to be the things that teach us the most. If anything,
        they have taught me that you should never compare yourself to others
        &mdash; only to who you were yesterday.
      </p>

      <Callout type="life-lesson">
        <strong>Life lesson:</strong> I have been listening to that one crazy dude
        that you might or might not know, depending on the place from where you
        know me, just some day ago. And you should probably be listening more
        carefully, since he just wants what is best for you. I remembered him
        saying something
        along the lines of: <em>how do you know that you are doing a good
        job?</em> And the answer was &mdash; <strong>when the job finds you,
        and not you the job</strong>. So I dug up one email from last year:
        <br /><br />
        <img
          src={`${import.meta.env.BASE_URL}assets/email-dev.png`}
          alt="Unsolicited recruiter email — the job found me"
          style={{ width: '100%', borderRadius: '8px', border: '1px solid var(--border)' }}
        />
        <br />
        This one is a scam by the way.
        <br /><br />
        And yes &mdash; dude, your domain is &ldquo;cryptoguru.digital&rdquo;.
        <br /><br />
        Do not act out of emotion &mdash; act out of logic. Do background
        checks, check the facts, and seek the truth.
      </Callout>

      <Callout type="tip">
        <strong>Fun fact:</strong> Steve Jobs believed he didn&rsquo;t need a license
        plate on his car &mdash; and he was technically right. California law at the
        time gave new cars a six-month grace period before requiring plates. Jobs
        exploited this loophole by leasing a new silver Mercedes-Benz SL55 AMG every
        six months, allowing him to perpetually drive without a license plate. He did
        this for years, and it was completely legal. California finally closed the
        loophole in 2019, eight years after his passing.
      </Callout>

      <h2>About This Document</h2>
      <p>
        This is less of a traditional language reference and more of a small book.
        Beyond the usual syntax and semantics, you will find a collection of useful
        materials, cool tricks, and insights gathered from a wide range of educational
        resources over the years. Sprinkled between the chapters are fun facts, video
        deep-dives, and programming challenges that are meant to keep things interesting
        and maybe teach you something unexpected along the way.
      </p>
      <p>
        Many of these materials I found extremely valuable &mdash; and increasingly rare.
        In an era of vibe-coding, where shipping fast matters more than understanding
        deeply, we have quietly forgotten the core concepts, the foundational books, and
        the hard-won lessons that this craft was built on. Some of the videos featured here
        contain words and thoughts from people like Ken Thompson, Dennis Ritchie,
        Bjarne Stroustrup, and James Gosling &mdash; programmers whose work we all stand on, whether we
        realize it or not.
      </p>
      <p>
        Of course, calling this a &ldquo;book&rdquo; or a &ldquo;reference&rdquo; is
        generous &mdash; for that to be true, it would need at least one reader. There are
        far better authors and far better books that deserve your time first. Consider this
        more of a curated collection of things I wish someone had shown me earlier,
        bundled alongside a language that gave me an excuse to put them all in one place.
      </p>
      <p>
        I have been reading <em>UNIX: A History and a Memoir</em> by Brian Kernighan,
        and it reminded me of something worth saying out loud: by reading a book you can
        gain knowledge and insights into the thoughts of some truly inspiring people &mdash;
        people who poured their whole lives, their mistakes, and the things they learned the
        hard way into those pages. And yet there will always be someone who dismisses them as
        outdated or no longer relevant. That is a shame, because a book is one of the few
        places where decades of hard-won experience are handed to you for the price of your
        attention. The tools change, the languages change, but the thinking behind them rarely
        does.
      </p>

      <Callout type="note">
        <strong>AT&T Archives: The UNIX Operating System</strong>
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/tc4ROCJYbm0"
          title="AT&T Archives: The UNIX Operating System"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <p>
        We call them <em>funny</em> challenges for a reason &mdash; not because they are
        jokes, but because each one can be solved in a completely different way depending
        on your background. A mathematician will reach for a formula, a systems programmer
        will reach for bitwise tricks, and an engineer will wonder why you are not just
        checking the logs. Everyone is convinced their approach is the right one, and
        they will defend it stubbornly. That is exactly what makes them fun.
      </p>
      <Callout type="tip">
        <strong>Fun fact:</strong> Some of these challenges are also known to show up in
        technical interviews &mdash; the kind of LeetCode-style questions that catch you off
        guard at a whiteboard at various IT companies. A few of them I remember being asked
        myself. Array rotation was one. The Sieve of Eratosthenes was another &mdash; and I
        still remember how unreasonably long it took me to work through it when I was just
        starting out. The interviewer was patient enough to walk me through the algorithm
        afterward, and that conversation taught me more than any textbook explanation could
        have. Edit distance was yet another &mdash; the kind of problem that looks deceptively
        simple until you sit down and realize you need a 2D matrix and a recurrence relation
        you definitely didn't have memorized. Those moments tend to stick with you.
      </Callout>

      <h2>Why Procedural?</h2>
      <p>
        jlang is a <strong>procedural</strong> language &mdash; no classes, no inheritance,
        no method overloading. Just functions, structs, and data you control. Languages
        like Go and Rust have shown that you can build serious, large-scale software
        without classical OOP, and jlang follows that same spirit: keep it simple,
        keep it explicit.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> Most people call C++ an object-oriented language, but
        Bjarne Stroustrup himself disagrees. He has repeatedly said that C++ is a
        <em>multi-paradigm</em> language and that forcing everything into classes is
        a misuse of it. Here he is, making exactly that point:
        {' '}
        <a
          href="https://www.youtube.com/watch?v=uTxRF5ag27A&t=2550"
          target="_blank"
          rel="noopener noreferrer"
        >
          Watch on YouTube (42:30)
        </a>
      </Callout>

      <h2>Jlang Philosophy</h2>
      <p>
        A programming language should respect the programmer. No magic behind
        the scenes, no decisions made on your behalf, no runtime quietly managing
        your memory. jlang is built on a simple belief &mdash; <strong>if you wrote it,
        you should understand exactly what it does</strong>. Every allocation is yours to
        make, every free is yours to call, and every line of code means precisely
        what it says.
      </p>
      <p>
        Writing in jlang is an act of craftsmanship. The language asks something
        of you &mdash; <strong>discipline</strong>. Every byte you allocate is a
        promise you make to eventually return it. Every variable you declare is a
        statement of intent: that this name matters, that it will be used, that it
        earns its place on the stack. There is no room for clutter, no tolerance
        for loose ends. A declared variable that serves no purpose is not just
        waste &mdash; it is a lie in your code, a name given to nothing. jlang
        believes that good software is built by programmers who care about every
        line they write. At its core, jlang is about <strong>understanding</strong>.
        But do you really know the difference between understanding what your code
        does and merely following a stack of recipes until something compiles?
      </p>

      <Callout type="note">
        <strong>Understanding vs. performing</strong>
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/Nb2tebYAaOA?start=1373"
          title="Understanding vs. performing"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="note">
        <strong>Every language has a philosophy &mdash; and they rarely agree.</strong>
        <br /><br />
        jlang says: <em>you own your memory, you own your mistakes</em>.
        C++ says: <em>you can do anything, but we won&rsquo;t stop you from shooting
        yourself in the foot</em>. Rust says: <em>we will stop you from shooting yourself
        in the foot, even if you really want to</em>. Go says: <em>here are 3 ways to do
        it, pick one and move on</em>. And then there is Erlang, which takes a completely
        different stance: <strong>let it crash</strong>. Instead of trying to prevent every
        possible failure, Erlang assumes things <em>will</em> go wrong and builds
        supervision trees that detect crashes and restart processes automatically. The
        philosophy is not about writing code that never fails &mdash; it is about writing
        systems that recover gracefully when it does.
        <br /><br />
        For a deeper dive into Erlang&rsquo;s philosophy:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/SOqQVoVai6s"
          title="Erlang - Let it crash philosophy"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <h2>A Quick Taste</h2>
      <p>
        Every language starts the same way:
      </p>
      <CodeBlock code={`fn main() -> i32 {
    printf("Hello, World!\\n");
    return 0;
}`} />

      <Callout type="note">
        <strong>Fun fact:</strong> The tradition of using &ldquo;Hello, World!&rdquo; as the
        first example program was started by Brian Kernighan in{' '}
        <em>The C Programming Language</em> (1978), co-authored with Dennis Ritchie. Every
        language since has followed suit, and at this point it would feel wrong not to.
      </Callout>

      <p>
        Here is something a bit more involved &mdash; finding all prime numbers up to a given
        limit using the Sieve of Eratosthenes:
      </p>
      <CodeBlock code={`fn main() -> i32 {
    val limit: i32 = 100;
    var sieve: bool[101];

    for (var i: i32 = 0; i <= limit; i++) {
        sieve[i] = true;
    }

    for (var p: i32 = 2; p * p <= limit; p++) {
        if (sieve[p]) {
            for (var j: i32 = p * p; j <= limit; j += p) {
                sieve[j] = false;
            }
        }
    }

    printf("Primes up to %d:\\n", limit);
    for (var i: i32 = 2; i <= limit; i++) {
        if (sieve[i]) {
            printf("%d ", i);
        }
    }

    return 0;
}`} />

      <h2>Why Implement a Language from Scratch?</h2>
      <p>
        Long distance runners sometimes train with weights strapped to their ankles or at high
        altitudes where the atmosphere is thin. When they later unburden themselves, the new
        relative ease of light limbs and oxygen-rich air enables them to run farther and faster.
      </p>
      <p>
        Implementing a language is a real test of programming skill. The code is complex and
        performance critical. You must master recursion, dynamic arrays, trees, graphs, and hash
        tables. You probably use hash tables at least in your day-to-day programming, but do you
        really understand them? Well, after we've crafted our own from scratch, I guarantee you
        will.
      </p>
      <p>
        While I intend to show you that an interpreter isn't as daunting as you might believe,
        implementing one well is still a challenge. Rise to it, and you'll come away a stronger
        programmer, and smarter about how you use data structures and algorithms in your day job.
      </p>
      <p style={{ fontSize: '0.9rem', fontStyle: 'italic', opacity: 0.7 }}>
        &mdash; Robert Nystrom, <em>Crafting Interpreters</em>
      </p>

      <h2>Fun Facts</h2>

      <Callout type="note">
        <strong>The <code>inline</code> keyword in C++ doesn't guarantee inlining</strong>
        <br /><br />
        A common misconception is that marking a function <code>inline</code> in C++ guarantees
        the compiler will inline it. In reality, <code>inline</code> is just a hint &mdash; the
        compiler is free to ignore it. Modern compilers decide on their own whether to inline
        based on heuristics like function size, call frequency, and optimization level.
        <br /><br />
        For a deeper dive, check out the explanation at 55:30:
        <br /><br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/ulJm7_aTiQM?start=3330"
          title="Inline keyword in C++ explained"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="note">
        <strong>No performance difference between prefix and postfix increment</strong>
        <br /><br />
        Unlike C++ iterators, there is no performance benefit to using <code>++x</code> over{' '}
        <code>x++</code> in jlang. Both generate the same three operations (load, add, store),
        and the only difference is which already-computed register value gets returned.
        No temporary copy is created.
        <br /><br />
        The &quot;prefer <code>++i</code> over <code>i++</code>&quot; advice comes from C++
        where postfix on complex objects (like iterators) requires constructing a temporary copy:
        <br /><br />
        <code style={{ whiteSpace: 'pre', display: 'block', padding: '0.75rem 1rem', background: 'var(--bg-code)', borderRadius: '6px', fontSize: '0.835rem', lineHeight: '1.65' }}>
{`// C++ iterator postfix - expensive!
Iterator operator++(int) {
    Iterator copy = *this;  // make a copy
    ++(*this);              // increment original
    return copy;            // return the copy
}`}
        </code>
        <br />
        For primitive types, modern compilers optimize both to identical machine code.
        Choose based on semantics, not performance.
      </Callout>
    </>
  )
}
