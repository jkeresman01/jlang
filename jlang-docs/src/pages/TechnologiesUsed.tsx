import Callout from '../components/Callout';

export default function TechnologiesUsed() {
  return (
    <>
      <h1>Technologies Used</h1>

      <p>
        Every project stands on the shoulders of tools that came before it, and
        jlang is no exception. It is written in C++ and uses LLVM for code
        generation. If you just felt a chill run down your spine, you are not
        alone. Let me explain why I chose these two, and then let me explain why
        I questioned that decision at least once a week for the entire duration
        of this project.
      </p>

      <h2>C++</h2>
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
        itself. But don&rsquo;t get me wrong &mdash; listening to someone in a
        lecture who can distill years of experience into an hour is invaluable.
        Those people just tend to be extremely rare.
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

      <h2>LLVM</h2>
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
    </>
  );
}
