import CodeBlock from '../components/CodeBlock';
import Callout from '../components/Callout';
import ParkingAd from '../components/ParkingAd';

export default function Introduction() {
  return (
    <>
      <section className="hero">
        <h1>jlang</h1>
        <p className="tagline">
          Journey Through a 25-Year-Old&rsquo;s Thoughts, Bundled with a
          Language
        </p>
        <p className="sub-tagline">
          A procedural programming language mostly inspired by Go/Rust &mdash;
          clean syntax, explicit memory management, and modern language features
          compiled to native code via LLVM.
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

      <h2>Introduction</h2>
      <p>
        Most introductions start with a formal greeting, a mission statement,
        and a bullet list of what you are about to read. This is not that kind
        of introduction. If you want corporate polish and carefully curated
        professionalism, you are in the wrong place. Here, the dress code is a
        black hoodie and the only tie you will ever see is the one Steve Jobs
        got paid sixty dollars to wear &mdash; and even he looked like he wanted
        to rip it off the entire time. I have always believed that only
        bartenders need to wear shirts and ties. The rest of us should be judged
        by what we build, not by what we button up to our necks. A well-ironed
        shirt has never fixed a bug, and a Windsor knot has never shipped a
        feature. If your code compiles and your tests pass, nobody cares whether
        you wrote it in a three-piece suit or in yesterday&rsquo;s pajamas.
      </p>
      <p>
        The same goes for titles. Senior, staff, principal, lead, architect
        &mdash; these are just words on a business card. They tell you where
        someone sits in an org chart, not how well they think through a problem
        of any nature. A title is a label someone else gave you. Oh, almost
        forgot Senior Specialist is yet another one that someone probably saw
        from some cartoon, solution architect is yet another like what do you
        do, create solutions in visual studio, not sure why people care for
        this, will try to mock them trough this book, the only reason being is
        that I don't really care for them, we are all people doing something,
        titles and roles are of no relevance. Your code is the only title that
        cannot be faked &mdash; and no amount of seniority will make a broken
        build pass.
      </p>

      <Callout type="note">
        <strong>
          Steve Jobs &mdash; &ldquo;They paid me $60 to wear a tie&rdquo;
        </strong>
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/t9HmOz8H0qI?start=0&end=14"
          title="Steve Jobs — They paid me $60 to wear a tie"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="tip">
        <strong>Fun fact:</strong> When Apple&rsquo;s board of directors first
        saw the finished &ldquo;1984&rdquo; Super Bowl commercial, they
        unanimously hated it. The CEO at the time, John Sculley, instructed the
        ad agency to sell back the Super Bowl airtime. But Steve Jobs refused to
        back down and defied the entire leadership team. The 60-second ad aired
        on January 22, 1984, during the third quarter of Super Bowl XVIII. Two
        days later, the Macintosh went on sale. Apple sold over 72,000 units in
        the first 100 days &mdash; $155 million worth of Macintoshes &mdash; far
        exceeding expectations. By the end of the first year, 250,000 units had
        been sold. The ad is now widely regarded as the greatest television
        commercial ever made and raised the Apple brand to its legendary status.
      </Callout>

      <p>
        Now, relaxed and open communication without perfectly curated
        professionalism might sound like the norm &mdash; and in most small to
        medium-sized places, it genuinely is. But if you ever step inside a
        large corporate entity, you will witness the exact opposite. Everyone
        says precisely what they are supposed to say. If someone declares that
        the Earth is flat, you nod your head and agree &mdash; because
        disagreeing is a career risk nobody is willing to take. Managers reward
        presence over output, perfectly content with people sitting at their
        desks all day staring at a screen &mdash; even if most of that screen
        time is spent reading football stats. Groupthink replaces independent
        thought. People huddle together beforehand to agree on what to say, then
        invest their energy in games of power and battles of egos instead of the
        craft itself. They take things out of context and frame them in whatever
        story benefits their own self-interest, spreading subtle lies in circles
        where they have influence and playing on crowd control to shape the
        narrative. Even those with the mental capacity to see through the facade
        stay silent, afraid that speaking up will make them the next target.
        People act out of insecurity, constantly seeking approval and validation
        from whoever sits above them in the hierarchy. And those who refuse to
        play that game &mdash; the ones who just want to build things &mdash;
        are quietly pushed to the margins through instruments of structural
        sidelining (not even sure if this is a real term &mdash; quite outside
        my domain of interest) &mdash; set up to fail, or simply managed out
        until they leave on their own. Assignments outside the person&rsquo;s
        expertise with no training, reputation undermining &mdash; suddenly they
        are &ldquo;not working,&rdquo; &ldquo;difficult,&rdquo; and who knows
        what else might come to mind. You have to give them credit, they get
        remarkably creative with it. And it&rsquo;s interesting to watch how
        different groups of people fall into these patterns &mdash; some lead
        the charge, some follow along, and some just look the other way. I have
        seen people cry when they see someone completing those things they were
        never allowed to finish, around a year ago. But I have also seen steam
        coming out of people&rsquo;s ears in their inability to think clearly at
        the moment. But I have also seen the spark in people&rsquo;s eyes when
        they connect the dots on their own. And that is the key &mdash; you
        cannot force someone to see it. You can only point them in the right
        direction and let them arrive at the conclusion themselves. So in this
        book, beyond just the elements of jlang, you will find suggestions on
        how to use the language &mdash; because I can only point you in the
        right direction, the rest is yours. I have also noticed that women
        usually just have an eighth sense for this kind of thing. There is an
        old saying: if an egg is broken by an outside force, life ends &mdash;
        but if it is broken from within, life begins.
      </p>

      <Callout type="note">
        <strong>This is how you get treated?</strong>
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/Lg2dqFCU67Q?start=38"
          title="If it is broken from within, life begins"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <Callout type="note">
        <strong>
          Was thinking about writing a song recently, house is used
          figuratively:{' '}
        </strong>
        <br />
        <em>
          In my house, in my house
          <br />
          Who's gonna find even the smallest irregularity on that guy?
          <br />
          Let the bounty hunt begin
          <br />
          Search every corner, search with clarity
          <br />
          <br />
          In my house, in my house
          <br />
          Every crack and every flaw
          <br />
          Nothing's hidden, nothing's safe
          <br />
          From the sharpest eye you ever saw
          <br />
          <br />
          Check the corners, check the floor
          <br />
          Look behind every single door
          <br />
          Under tables, under beds
          <br />
          Every place where something's fled
          <br />
          <br />
          In my house, in my house
          <br />
          The hunt is long but it's almost done
          <br />
          Every shadow has been chased
          <br />
          Every secret, every one
          <br />
          <br />
          And when the last flaw has been found
          <br />
          When there's no more searching ground
          <br />
          The bounty's claimed, the hunt is through
          <br />
        </em>
      </Callout>

      <Callout type="life-lesson">
        <strong>Life lesson:</strong> Watch out &mdash; find those good people
        of strong character and learn from them. And learn how to spot the
        greedy ones, those who act purely out of self-interest, and those whose
        entire value system is built on money, titles, and positions inside some
        organization. Those people will smile at you while pulling the ladder up
        behind them. Real character is not measured by what someone has, but by
        what they do when no one is watching.
      </Callout>

      <p>
        So here, this is the opposite of that. This is open, transparent, and
        opinionated &mdash; because there is nothing worse than a person without
        an opinion. Or worse still, a person whose opinion shifts with the tide,
        bending whichever way the wind blows, saying whatever the room wants to
        hear. If you stand for nothing, you will fall for anything. And if your
        convictions change with every conversation, they were never convictions
        at all &mdash; just noise dressed up as thought.
      </p>

      <h2>About Me</h2>
      <p>
        I am a clueless twenty-five-year-old who programs every day and is now,
        for reasons I cannot fully explain, writing this. If you ever see me in
        person, you will probably see me in a black hoodie, staring at nothing
        in particular, and you might wonder what is going on in that guy&rsquo;s
        head &mdash; or why he codes every day, or why he thinks he can write
        something that people would actually read. Truth be told, I do not know
        the answer to that last one. But I can try to answer the rest.
      </p>
      <p>
        I code every day because it is an escape. An escape from this cruel
        world of judgment, fear, and wars. An escape from your everyday
        troubles, from the fight with someone you love. An escape into a world
        of thoughts, creation, and innovation. When you are deep enough inside a
        problem, the rest of the world simply stops existing.
      </p>

      <p>
        And why write this? Truth be told, because I am terrified. Terrified
        that my obsession is coming to an end. I have done every programming
        project I have ever wanted to do, and now I am terrified that I will
        drift back into the world of reality. So this seemed like the next
        natural step &mdash; one more thing to keep me in this world a little
        longer.
      </p>

      <p>
        And world of reality is cruel a place so you need to find something to
        keep you obssesed. Sat trough few lectures just this wednesday, saw young
        fellas mostly uninterested in the topics and who can blame them. Remembered 
        myself at their age. During my first 2 years of univeristy I was a complete 
        mess, being uninterested in anything. Would sit trough lecture or not
        come to any of them just on the exam day, realised some years later that
        I should have been listening better. Got my things together in the final year.
        Can see simalar patterns in other young fellas, we all have a moment where 
        something starts to interest us. Remembered myself saying words like: "when will
        I ever need this", but things clicked few years later down the road. Altough 
        those years were a mess and I feel quite good and organised right now and for 
        past 3 years and this "messy" period wasn't something that was benefital 
        to my life, mostly wasting time, I have accumulated a lot of interesting stories
        during that time.
        You'll find car sleeping one in pointers chapter. It doesn't fit top 100, some 
        of them are really hard to fit in a book about programming to be honest.
      </p>

      <p>
        In programming, you can get stuck on a problem and keep thinking about
        it for days. I sometimes walk for half an hour to find my car because I
        have completely forgotten where I parked it &mdash; too deep inside this
        wonderful world to notice something as trivial as where I left a two-ton
        piece of metal. Sometimes I walk right past it, more than once, without
        realizing it is sitting right in front of my nose. Sometimes I walk down
        the street and hit a traffic light pole. It is probably a good thing
        that every week or two I get a parking ticket &mdash; I have started to
        remember spots by the tickets I have collected there, and at this point
        they have become almost enjoyable to pay. In fact, I got one just today
        &mdash; or yesterday, I am not sure if it was before or after midnight.
      </p>

      <Callout type="note">
        There is a difference between programming and a job &mdash; whether that
        job is software engineering or anything else. A job is a set of tasks
        you execute so you can get paid, pay the bills, and put food on the
        table. Programming, the way I mean it throughout this book, is something
        else entirely. It is where your individuality, your creativity, and your
        uniqueness come into play. It is a form of self-expression &mdash; a
        space where you are not following someone else&rsquo;s checklist but
        building something that is distinctly yours. When I talk about
        programming here, that is what I am talking about.
      </Callout>
      <p>
        I have also realized that the best books and the best authors are those
        who speak out of their own experience &mdash; the ones in whose skin you
        can walk, the ones who can make you feel something. Sometimes it is
        frustration. Sometimes it is admiration. Sometimes it is hate. Sometimes
        your eyes fill with tears of joy. But the thing about people like that
        is that you can feel their passion and their dedication bleeding through
        every page.
      </p>
      <p>
        Robert Nystrom, in the acknowledgements of{' '}
        <em>Crafting Interpreters</em>, writes:
      </p>
      <blockquote
        style={{
          borderLeft: '3px solid var(--border)',
          paddingLeft: '1rem',
          margin: '1.5rem 0',
          fontStyle: 'italic',
          opacity: 0.85,
        }}
      >
        Finally, the deepest thanks go to my wife Megan and my daughters Lily
        and Gretchen. You patiently endured the time I had to sink into the
        book, and my stress while writing it. There&rsquo;s no one I&rsquo;d
        rather be stuck at home with.
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
        make them feel something real &mdash; are extremely rare. You can
        glorify them or vilify them, but the only thing you cannot do is ignore
        them. Because the people who are crazy enough to think they can change
        the world are the ones who do.
      </p>

      <Callout type="note">
        <strong>Here&rsquo;s to the crazy ones</strong>
        <br />
        <br />
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
        do too &mdash; sometimes even more so. Throughout this book you will
        find examples that were born not from confidence, but from moments where
        I had none. Moments of imposter syndrome, of not knowing something I
        felt I should have known. In the chapter on operators you will find an
        example about endian swapping. It earned its place there because I still
        remember filing my first bug report &mdash; a crash during an endian
        swap &mdash; and not being entirely clear on what an endian even was.
        The embarrassment of that moment has never fully left me. There are many
        more examples like that scattered across these pages. They are here
        because they are honest, and because the things that embarrass us tend
        to be the things that teach us the most. If anything, they have taught
        me that you should never compare yourself to others &mdash; only to who
        you were yesterday.
      </p>

      <Callout type="life-lesson">
        <strong>Life lesson:</strong> I have been listening to that one crazy
        dude that you might or might not know, depending on the place from where
        you know me, just some day ago <strong>(The final boss)</strong>. And you should probably be listening
        more carefully, since he just wants what is best for you. I remembered
        him saying something, some time ago (not that day), along the lines of:{' '}
        <em>how do you know that you are doing a good job?</em> And the answer
        was &mdash; <strong>when the job finds you, and not you the job</strong>
        . So I dug up one email from last year:
        <br />
        <br />
        <img
          src={`${import.meta.env.BASE_URL}assets/email-dev.png`}
          alt="Unsolicited recruiter email — the job found me"
          style={{
            width: '100%',
            borderRadius: '8px',
            border: '1px solid var(--border)',
          }}
        />
        <br />
        This one is a scam by the way.
        <br />
        <br />
        And yes &mdash; dude, your domain is &ldquo;cryptoguru.digital&rdquo;.
        <br />
        <br />
        Do not act out of emotion &mdash; act out of logic. Do background
        checks, check the facts, and seek the truth.
      </Callout>

      <Callout type="tip">
        <strong>Fun fact:</strong> Steve Jobs believed he didn&rsquo;t need a
        license plate on his car &mdash; and he was technically right.
        California law at the time gave new cars a six-month grace period before
        requiring plates. Jobs exploited this loophole by leasing a new silver
        Mercedes-Benz SL55 AMG every six months, allowing him to perpetually
        drive without a license plate. He did this for years, and it was
        completely legal. California finally closed the loophole in 2019, eight
        years after his passing.
      </Callout>

      <h2>About This Document</h2>
      <p>
        This is less of a traditional language reference and more of a small
        book. Beyond the usual syntax and semantics, you will find a collection
        of useful materials, cool tricks, and insights gathered from a wide
        range of educational resources over the years. Sprinkled between the
        chapters are fun facts, video deep-dives, and programming challenges
        that are meant to keep things interesting and maybe teach you something
        unexpected along the way.
      </p>

      <h3>Why a Website?</h3>
      <p>
        You might wonder why this is a website and not a PDF, not a printed
        book, and not written in LaTeX. The answer is simple &mdash;
        accessibility. A website is the lowest barrier to entry there is. You
        open a link and you are reading. No downloads, no special software, no
        figuring out which PDF viewer renders the fonts correctly. It works on
        your phone, on your laptop, on a ten-year-old tablet your grandmother
        uses to read recipes. Anyone with a browser and an internet connection
        can read this, and that is exactly the point.
      </p>
      <p>
        LaTeX is a phenomenal tool &mdash; if you are writing a scientific
        paper, a thesis, or a mathematical proof. It produces beautiful
        typesetting and handles equations, citations, and formal formatting
        better than anything else out there. But this is not a scientific paper.
        This is a collection of thoughts, examples, and stories from someone who
        programs every day and wanted to share what he has learned along the
        way. Using LaTeX here would add complexity for the sake of complexity
        &mdash; and that goes against everything this project stands for.
      </p>
      <p>
        A PDF would have been another option, but PDFs are static. You cannot
        embed a video in a PDF. You cannot have syntax-highlighted code blocks
        that look the same on every device. You cannot have a dark mode toggle
        for reading at 2 AM when you should be sleeping. And you certainly
        cannot update a PDF after someone points out a mistake &mdash; you would
        have to redistribute the entire file and hope people download the new
        version.
      </p>
      <p>
        A printed book carries weight &mdash; both literally and figuratively
        &mdash; and I have deep respect for the medium. But printing costs
        money, distribution takes time, and updates are impossible once ink hits
        paper. For something that is still evolving, that would be premature.
      </p>
      <p>
        A website lets me embed videos, update content the moment I spot an
        error, and keep everything in one place that anyone can reach. It is not
        the most prestigious format, and it will never sit on a shelf next to
        Kernighan and Ritchie. But it is honest, it is practical, and it is
        free. And for what this is &mdash; a twenty-five-year-old&rsquo;s
        attempt to share what he knows &mdash; that feels like the right fit.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> If this were a book on a shelf, it would not
        be filed under drama &mdash; it would be a comedy. And every comedy
        needs a protagonist. Here is Jos&eacute; Mourinho talking about his most
        problematic player:
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/AwXgfBzK_L4?start=26"
          title="Mourinho on his most problematic player"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <p>
        Many of these materials I found extremely valuable &mdash; and
        increasingly rare. In an era of vibes, where shipping fast matters more
        than understanding deeply, we have quietly forgotten the core concepts,
        the foundational books, and the hard-won lessons that this craft was
        built on. This book tries to bring some of those voices back. Some of
        the videos featured here contain words and thoughts from people like Ken
        Thompson, Dennis Ritchie, Bjarne Stroustrup, and James Gosling &mdash;
        programmers whose work we all stand on, whether we realize it or not.
        You will also find quotes from great craftsmen, athletes, coaches,
        musicians, and visionaries &mdash; people whose dedication to their
        craft transcends the field they practiced it in. Notable ones include
        Kobe Bryant, Elvis Presley, Michael Jordan, Muhammad Ali, and
        Jos&eacute; Mourinho &mdash; to be honest, the only reason Mourinho is
        here is because he had a funny video; I do not particularly care for
        him. You will also notice that Steve Jobs appears throughout these pages
        more than anyone else &mdash; not because I idolize the man, but because
        there was something weirdly unique about the way he saw the world that
        reminds me of my own weirdness.
      </p>
      <p>
        None of this is to say that abstraction is the enemy. Every time we have
        raised the level of abstraction, we have unlocked entirely new
        possibilities for innovation, enabling solutions to problems that were
        previously out of reach and empowering more people to build things that
        genuinely help others. The tool itself, in its current form, is nothing
        short of extraordinary. What is worth mourning is not progress, but the
        forgetting &mdash; when we stop asking why things work the way they do,
        we lose the very foundation that makes the next leap possible.
      </p>
      <p>
        And this got me thinking about something. Software is becoming cheaper,
        easier to build — this website has been written mostly with Claude's
        Opus 4.6. Will there still be a need for companies that purely live on
        providing software development services? Perhaps yes, perhaps no. But
        one thing is for sure — those that have their own products will thrive
        since they can bring value to their users faster and cheaper than ever.
      </p>
      <p>
       And with AI improving every day, there comes a question. Will there be
       a need for software developers in the future ? And here is my answer, yes, the
       only reason being that software is too complex and while AI excels in creating 
       small prototypes and automating boring stuff, when it comes to real world problems 
       where you need to create something that runs everywhere on every device and always
       there are tons of edge cases and small little details, like security for example that
       everyday Joe will probably not think of while vibing he's application. But don't get
       me wrong same goes for almost every discipline. AI can write a song but without personal 
       touch the song has no soul.
      </p>
      <p>
        Of course, calling this a &ldquo;book&rdquo; or a
        &ldquo;reference&rdquo; is generous &mdash; for that to be true, it
        would need at least one reader. There are far better authors and far
        better books that deserve your time first. Consider this more of a
        curated collection of things I wish someone had shown me earlier,
        bundled alongside a language that gave me an excuse to put them all in
        one place.
      </p>
      <p>
        I have been reading <em>UNIX: A History and a Memoir</em> by Brian
        Kernighan, and it reminded me of something worth saying out loud: by
        reading a book you can gain knowledge and insights into the thoughts of
        some truly inspiring people &mdash; people who poured their whole lives,
        their mistakes, and the things they learned the hard way into those
        pages. And yet there will always be someone who dismisses them as
        outdated or no longer relevant. That is a shame, because a book is one
        of the few places where decades of hard-won experience are handed to you
        for the price of your attention. The tools change, the languages change,
        but the thinking behind them rarely does.
      </p>

      <Callout type="note">
        <strong>AT&T Archives: The UNIX Operating System</strong>
        <br />
        <br />
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
        We call them <em>funny</em> challenges for a reason &mdash; not because
        they are jokes, but because each one can be solved in a completely
        different way depending on your background. A mathematician will reach
        for a formula, a systems programmer will reach for bitwise tricks, and
        an engineer will wonder why you are not just checking the logs. Everyone
        is convinced their approach is the right one, and they will defend it
        stubbornly. That is exactly what makes them fun.
      </p>
      <Callout type="life-lesson">
        <strong>Life lesson:</strong> I owe 120 euros in parking fines to
        Proračun Grada Zagreba from last month for the way I park my car &mdash;
        haven&rsquo;t paid them yet. Will I stop parking there? Absolutely not.
        But these tickets are getting unreasonably expensive, and at this rate I
        might end up funding an entire city block renovation with my violations
        alone.
      </Callout>

      <ParkingAd />

      <Callout type="tip">
        <strong>Fun fact:</strong> Some of these challenges are also known to
        show up in technical interviews &mdash; the kind of LeetCode-style
        questions that catch you off guard at a whiteboard at various IT
        companies. A few of them I remember being asked myself. Array rotation
        was one. The Sieve of Eratosthenes was another &mdash; and I still
        remember how unreasonably long it took me to work through it when I was
        just starting out. The interviewer was patient enough to walk me through
        the algorithm afterward, and that conversation taught me more than any
        textbook explanation could have. Edit distance was yet another &mdash;
        the kind of problem that looks deceptively simple until you sit down and
        realize you need a 2D matrix and a recurrence relation you definitely
        didn&rsquo;t have memorized. Those moments tend to stick with you.
      </Callout>

      <Callout type="important">
        Every example throughout this book is here because, at some point, it
        made me feel embarrassed or insecure &mdash; I didn&rsquo;t know the
        answer, and I felt like I should have. But instead of letting that
        feeling sit, I wrote it down. That simple habit &mdash; noting things
        down &mdash; became the beginning of a journey to patch every insecurity
        I could name. Over the years I collected answers to those questions, and
        with time I became composed, patient, and full of understanding &mdash;
        probably because I stopped pretending I already knew everything and
        started being honest about what I didn&rsquo;t.
        <br />
        <br />
        Every programming project I have ever built was, in some way, a response
        to an insecurity. And now that I have collected enough answers to sit
        down and write about them, I realize this is exactly how we should
        handle the moments where we act irrationally or irresponsibly: note them
        down, learn from them, and make sure they do not happen again &mdash;
        while being honest and transparent, because only through honesty can we
        move forward.
        <br />
        <br />
        Most of the examples throughout this book also come from Java and C++
        &mdash; the two languages I have spent the most time with. If you check
        my stats you will see around 50% Java and around 31% C++, so it only
        felt natural to draw from the worlds I know best. And when I say
        &ldquo;worlds,&rdquo; I mean exactly that &mdash; not the jobs I have
        worked in those languages, but the worlds I have lived in as a
        programmer, where my curiosity, creativity, and individuality shaped the
        way I think and build.
        <br />
        <br />
        Be better than you were yesterday.
      </Callout>

      <h2>Why Procedural?</h2>
      <p>
        jlang is a <strong>procedural</strong> language &mdash; no classes, no
        inheritance, no method overloading. Just functions, structs, and data
        you control. Languages like Go and Rust have shown that you can build
        serious, large-scale software without classical OOP, and jlang follows
        that same spirit: keep it simple, keep it explicit.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> Most people call C++ an object-oriented
        language, but Bjarne Stroustrup himself disagrees. He has repeatedly
        said that C++ is a <em>multi-paradigm</em> language and that forcing
        everything into classes is a misuse of it. Here he is, making exactly
        that point:{' '}
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
        the scenes, no decisions made on your behalf, no runtime quietly
        managing your memory. jlang is built on a simple belief &mdash;{' '}
        <strong>
          if you wrote it, you should understand exactly what it does
        </strong>
        . Every allocation is yours to make, every free is yours to call, and
        every line of code means precisely what it says.
      </p>
      <p>
        Writing in jlang is an act of craftsmanship. The language asks something
        of you &mdash; <strong>discipline</strong>. Every byte you allocate is a
        promise you make to eventually return it. Every variable you declare is
        a statement of intent: that this name matters, that it will be used,
        that it earns its place on the stack. There is no room for clutter, no
        tolerance for loose ends. A declared variable that serves no purpose is
        not just waste &mdash; it is a lie in your code, a name given to
        nothing. jlang believes that good software is built by programmers who
        care about every line they write. At its core, jlang is about{' '}
        <strong>understanding</strong>. But do you really know the difference
        between understanding what your code does and merely following a stack
        of recipes until something compiles?
      </p>

      <Callout type="note">
        <strong>Understanding vs. performing</strong>
        <br />
        <br />
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
        <strong>
          Every language has a philosophy &mdash; and they rarely agree.
        </strong>
        <br />
        <br />
        jlang says: <em>you own your memory, you own your mistakes</em>. C++
        says:{' '}
        <em>
          you can do anything, but we won&rsquo;t stop you from shooting
          yourself in the foot
        </em>
        . Rust says:{' '}
        <em>
          we will stop you from shooting yourself in the foot, even if you
          really want to
        </em>
        . Go says: <em>here are 3 ways to do it, pick one and move on</em>. And
        then there is Erlang, which takes a completely different stance:{' '}
        <strong>let it crash</strong>. Instead of trying to prevent every
        possible failure, Erlang assumes things <em>will</em> go wrong and
        builds supervision trees that detect crashes and restart processes
        automatically. The philosophy is not about writing code that never fails
        &mdash; it is about writing systems that recover gracefully when it
        does.
        <br />
        <br />
        For a deeper dive into Erlang&rsquo;s philosophy:
        <br />
        <br />
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

      <h2>Why the Name &ldquo;jlang&rdquo;?</h2>
      <p>
        Most of my personal projects get a &ldquo;j&rdquo; prefixed to them. Not
        because it stands for something profound &mdash; it does not. It is just
        a reflex at this point, the way some people always park in the same spot
        or always order the same coffee. I needed a name, I had a keyboard in
        front of me, and the letter was right there. The language got its name
        in about two seconds, and the compiler took considerably longer.
      </p>

      <h2>A Quick Taste</h2>
      <p>Every language starts the same way:</p>
      <CodeBlock
        code={`fn main() -> i32 {
    printf("Hello, World!\\n");
    return 0;
}`}
      />

      <Callout type="note">
        <strong>Fun fact:</strong> The tradition of using &ldquo;Hello,
        World!&rdquo; as the first example program was started by Brian
        Kernighan in <em>The C Programming Language</em> (1978), co-authored
        with Dennis Ritchie. Every language since has followed suit, and at this
        point it would feel wrong not to.
      </Callout>

      <p>
        Here is something a bit more involved &mdash; finding all prime numbers
        up to a given limit using the Sieve of Eratosthenes:
      </p>
      <CodeBlock
        code={`fn main() -> i32 {
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
}`}
      />

      <h2>Compiled, Not Interpreted</h2>
      <p>
        If you have ever used Python, Ruby, or JavaScript, you are used to
        languages where your code is read and executed line by line at runtime
        &mdash; that is interpretation. Java and C# take a middle road: they
        are both compiled <em>and</em> interpreted. Java compiles your source
        into bytecode that runs on the JVM, and C# compiles to bytecode that
        runs on the CLR. In both cases, the bytecode is then interpreted or
        JIT-compiled at runtime by the virtual machine. jlang does neither. It is a <strong>compiled language</strong> &mdash; your
        source code goes through scanning, parsing, and semantic analysis, then
        gets translated into LLVM Intermediate Representation, and finally LLVM
        turns that IR into native machine code for your platform. The result is
        a real, standalone executable &mdash; no interpreter, no VM, no runtime
        sitting between your code and the hardware.
      </p>
      <p>
        This puts jlang in the same category as C, C++, Go, and Rust. Your
        program runs at native speed, benefits from LLVM&rsquo;s battle-tested
        optimization passes, and can be distributed as a single binary with no
        dependencies. When you run <code>./output</code>, there is nothing
        between you and the metal.
      </p>

      <Callout type="note">
        <strong>
          Chris Lattner &mdash; &ldquo;What is a compiler?&rdquo;
        </strong>
        <br />
        <br />
        Chris Lattner, the creator of LLVM and Clang, gives a high-level
        explanation of what a compiler actually is.
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/yCd3CzGSte8?start=2120"
          title="Chris Lattner — What is a compiler?"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <h2>How Difficult are compilers </h2>
      <p>
        Here is one video that explain exactly how difficult to understand 
        they are:
        <Callout type="note">
            <strong>
             &mdash; &ldquo;How difficult are compilers ?&rdquo;
            </strong>
            <br />
            <br />
            <iframe
              width="100%"
              height="315"
              src="https://www.youtube.com/embed/Mlr7iqVVnBk"
              title="100 Drama, How can't you understand?"
              frameBorder="0"
              allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
              allowFullScreen
              style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
            ></iframe>
          </Callout>
      </p>

      <h2>Why Implement a Language from Scratch?</h2>
      <p>
        Long distance runners sometimes train with weights strapped to their
        ankles or at high altitudes where the atmosphere is thin. When they
        later unburden themselves, the new relative ease of light limbs and
        oxygen-rich air enables them to run farther and faster.
      </p>
      <p>
        Implementing a language is a real test of programming skill. The code is
        complex and performance critical. You must master recursion, dynamic
        arrays, trees, graphs, and hash tables. You probably use hash tables at
        least in your day-to-day programming, but do you really understand them?
        Well, after we&rsquo;ve crafted our own from scratch, I guarantee you
        will.
      </p>
      <p>
        While I intend to show you that an interpreter isn&rsquo;t as daunting
        as you might believe, implementing one well is still a challenge. Rise
        to it, and you&rsquo;ll come away a stronger programmer, and smarter
        about how you use data structures and algorithms in your day job.
      </p>
      <p style={{ fontSize: '0.9rem', fontStyle: 'italic', opacity: 0.7 }}>
        &mdash; Robert Nystrom, <em>Crafting Interpreters</em>
      </p>

      <p>Muhammad Ali said it better than anyone:</p>
      <blockquote
        style={{
          borderLeft: '3px solid var(--border)',
          paddingLeft: '1rem',
          margin: '1.5rem 0',
          fontStyle: 'italic',
          opacity: 0.85,
        }}
      >
        I hated every minute of training, but I said, &ldquo;Don&rsquo;t quit.
        Suffer now and live the rest of your life as a champion.&rdquo;
        <br />
        <span style={{ fontStyle: 'normal', opacity: 0.7 }}>
          &mdash; Muhammad Ali
        </span>
      </blockquote>
      <p>
        That is exactly what implementing a language from scratch feels like.
        Every moment you spend wrestling with symbol tables, type resolution,
        and code generation is a weight strapped to your ankle. But when you
        finally take it off &mdash; when you go back to writing everyday code
        &mdash; you see everything differently. The struggle is the point.
      </p>

      <h2>Technologies Used</h2>
      <p>
        Every project stands on the shoulders of tools that came before it, and
        jlang is no exception. It is written in C++ and uses LLVM for code
        generation. If you just felt a chill run down your spine, you are not
        alone. Let me explain why I chose these two, and then let me explain why
        I questioned that decision at least once a week for the entire duration
        of this project.
      </p>

      <h3>C++</h3>
      <p>
        C++ is an arcane language that only people who hunt mammoths with spears
        for breakfast used to write in. It was born in 1979 as &ldquo;C with
        Classes,&rdquo; and since then it has absorbed every programming
        paradigm, every design pattern, and every feature that has ever existed
        in this world &mdash; and a few that probably should not. It has
        templates, concepts, coroutines, modules, ranges, constexpr, consteval,
        constinit, and roughly forty-seven different ways to initialize a
        variable, each with subtly different behavior depending on whether
        Mercury is in retrograde.
      </p>
      <p>
        It has this thing called <em>smart pointers</em> &mdash; which, despite
        the name, are not particularly smart. A <code>unique_ptr</code> gets
        confused the moment you try to copy it. A <code>shared_ptr</code> drags
        around a reference count like a ball and chain and will happily create
        circular references that leak memory forever if you do not babysit it
        with <code>weak_ptr</code>, the pointer equivalent of a friend who only
        shows up when you explicitly ask. They are called &ldquo;smart&rdquo; in
        the same way that a smoke detector is &ldquo;smart&rdquo; &mdash; it
        will scream at you when something is already on fire, but it will not
        stop you from lighting the match.
      </p>
      <p>
        And then there is the ecosystem. C++ has a standards committee that
        meets roughly once every three years to add another thousand pages to a
        specification that is already longer than most legal codes. Every new
        standard promises to make the language &ldquo;simpler&rdquo; and
        &ldquo;more modern,&rdquo; and every new standard makes it harder for
        any single human being to hold the entire language in their head. C++11
        was the renaissance. C++14 was a polish pass. C++17 was getting
        ambitious. C++20 introduced concepts, coroutines, and modules &mdash;
        three features so large that most compilers still do not fully support
        all of them. And C++23 kept going, because stopping was never an option.
      </p>
      <p>
        There is also a strange cultural gatekeeping around C++ &mdash; this
        unspoken belief that you need a university diploma to do anything
        meaningful with it. As if a four-year degree is a prerequisite for
        understanding RAII or template metaprogramming. This is the year 2026.
        Every lecture, every textbook, every tutorial, every conference talk
        that has ever been recorded is sitting on the internet waiting to be
        watched for free. The library at some university is no longer the only
        place where knowledge can be obtained. It never really was &mdash; it
        was just the only place where knowledge came with a stamp of approval.
        And somewhere along the way, we confused the stamp with the knowledge
        itself. But don't get me wrong, listening someone in lecture or a talk
        that can summarize you knowledge that they've spent years acquiring is
        worth listening to, but those tend to be extremely rare.
      </p>
      <p>
        But here is the thing. After all of that &mdash; after the arcane
        syntax, the footguns, the incomprehensible error messages that span
        three terminal screens when you forget a semicolon inside a template
        &mdash; C++ is <em>everywhere</em>. It powers operating systems,
        browsers, game engines, databases, compilers, and embedded systems. It
        is fast, it is reliable, and a staggering number of people know it. When
        you write something in C++, you can be reasonably sure that it will
        still compile and run twenty years from now. Very few languages can make
        that promise. So yes, I chose C++ &mdash; not because it is elegant, but
        because it is battle-tested in ways that most languages will never be.
        It is the kind of tool that makes you earn every single thing it gives
        you, and maybe that is exactly why the things built with it tend to
        last.
      </p>

      <Callout type="tip">
        <strong>Fun fact:</strong> The C++ standard document (ISO/IEC
        14882:2020) is over 1,800 pages long. For comparison, the Go
        specification fits in about 90 pages. The Lua reference manual is around
        100. The entire K&amp;R C book &mdash; which defined the language that
        C++ was built on &mdash; is 272 pages, and that includes the tutorial,
        the reference, and the appendix. C++ is not a language &mdash; it is a
        lifestyle.
      </Callout>

      <h3>LLVM</h3>
      <p>
        LLVM is the backend that turns jlang code into actual machine
        instructions. In theory, it is one of the most impressive pieces of
        software engineering ever created &mdash; a modular compiler
        infrastructure that supports dozens of targets, performs hundreds of
        optimization passes, and powers production compilers like Clang, Rust,
        and Swift. In practice, it is the kind of project where you spend three
        hours reading documentation only to discover that the API you were
        studying was deprecated two versions ago and the replacement has no
        documentation at all.
      </p>
      <p>
        LLVM&rsquo;s API surface is vast, inconsistent, and changes between
        versions with a regularity that borders on hostile. Functions get
        renamed, signatures get reshuffled, and entire subsystems get replaced
        without so much as a migration guide. You write code against LLVM 16,
        update to LLVM 17, and suddenly half your code does not compile because
        someone decided that the function you were calling should take an extra
        parameter that did not exist before. The error message, naturally, will
        not tell you this &mdash; it will instead give you a wall of template
        instantiation failures that makes you question every decision you have
        ever made.
      </p>
      <p>
        And yet, LLVM is indispensable. Without it, I would be writing my own
        register allocator, my own instruction selector, and my own optimization
        passes &mdash; and they would all be worse. LLVM takes the IR I hand it
        and turns it into genuinely good machine code across multiple
        architectures. It is a remarkable piece of engineering buried under
        layers of accidental complexity and an API that seems designed to punish
        anyone who did not contribute to it.
      </p>

      <Callout type="note">
        <strong>
          Jonathan Blow &mdash; &ldquo;Isn&rsquo;t this proof that a
          five-year-old is in charge of LLVM?&rdquo;
        </strong>
        <br />
        <br />
        Jonathan Blow, the creator of <em>Braid</em> and <em>The Witness</em>,
        has been building his own programming language called Jai &mdash; and he
        has had some <em>thoughts</em> about LLVM along the way. In this clip,
        he walks through a real LLVM issue that perfectly captures the
        experience of working with the project &mdash; the kind of thing that
        makes you pause, stare at the screen, and wonder who exactly is steering
        the ship.
        <br />
        <br />
        <iframe
          width="100%"
          height="315"
          src="https://www.youtube.com/embed/ns7jP1-SRvw?start=132"
          title="Jonathan Blow — Isn't this proof that a five-year-old is in charge of LLVM?"
          frameBorder="0"
          allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture"
          allowFullScreen
          style={{ borderRadius: '8px', border: '1px solid var(--border)' }}
        ></iframe>
      </Callout>

      <p>
        So there you have it. jlang is written in a language that adds every
        feature known to mankind and compiled through a backend that breaks its
        own API with the enthusiasm of a toddler rearranging furniture. And
        somehow, despite all of that, it works. Maybe that says something
        profound about the tools we use &mdash; that the best ones are never the
        prettiest, never the most intuitive, and never the ones you would choose
        if you were starting from a blank slate. They are the ones that showed
        up, survived, and refused to die. And in this industry, that counts for
        more than elegance ever will.
      </p>

      <h2>Fun Facts</h2>

      <Callout type="note">
        <strong>
          The <code>inline</code> keyword in C++ doesn't guarantee inlining
        </strong>
        <br />
        <br />A common misconception is that marking a function{' '}
        <code>inline</code> in C++ guarantees the compiler will inline it. In
        reality, <code>inline</code> is just a hint &mdash; the compiler is free
        to ignore it. Modern compilers decide on their own whether to inline
        based on heuristics like function size, call frequency, and optimization
        level.
        <br />
        <br />
        For a deeper dive, check out the explanation at 55:30:
        <br />
        <br />
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
        <strong>
          No performance difference between prefix and postfix increment
        </strong>
        <br />
        <br />
        Unlike C++ iterators, there is no performance benefit to using{' '}
        <code>++x</code> over <code>x++</code> in jlang. Both generate the same
        three operations (load, add, store), and the only difference is which
        already-computed register value gets returned. No temporary copy is
        created.
        <br />
        <br />
        The &quot;prefer <code>++i</code> over <code>i++</code>&quot; advice
        comes from C++ where postfix on complex objects (like iterators)
        requires constructing a temporary copy:
        <br />
        <br />
        <code
          style={{
            whiteSpace: 'pre',
            display: 'block',
            padding: '0.75rem 1rem',
            background: 'var(--bg-code)',
            borderRadius: '6px',
            fontSize: '0.835rem',
            lineHeight: '1.65',
          }}
        >
          {`// C++ iterator postfix - expensive!
Iterator operator++(int) {
    Iterator copy = *this;  // make a copy
    ++(*this);              // increment original
    return copy;            // return the copy
}`}
        </code>
        <br />
        For primitive types, modern compilers optimize both to identical machine
        code. Choose based on semantics, not performance.
      </Callout>
    </>
  );
}
