*This project has been created as part of the 42 curriculum by bahbibe.*

# Philosophers

<p align="center">
  <img width="500" alt="Dining Philosophers" src="https://upload.wikimedia.org/wikipedia/commons/thumb/7/7b/An_illustration_of_the_dining_philosophers_problem.png/1024px-An_illustration_of_the_dining_philosophers_problem.png">
</p>

## Description

This project is an implementation of the classic [dining philosophers
problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem), used to
illustrate deadlock and starvation in [concurrent
programming](https://en.wikipedia.org/wiki/Concurrency_(computer_science)).

A number of philosophers sit at a round table with one fork between each
pair of them. Each philosopher endlessly cycles through eating, sleeping and
thinking, and needs both their left and right fork to eat. A philosopher who
does not start eating before `time_to_die` milliseconds have passed since
their last meal (or since the simulation started) dies, and the simulation
stops.

The project has two parts:

- **`philo/`** (mandatory): each philosopher is a `pthread`, forks are
  `pthread_mutex_t`.
- **`philo_bonus/`** (bonus): each philosopher is its own process (`fork`),
  forks are represented by a POSIX semaphore.

The mandatory part has to avoid both **deadlock** (everyone holding one fork,
waiting forever for the other) and **starvation** (one philosopher
permanently losing the race for a fork to its neighbours) using only mutexes
— no semaphores or condition variables are allowed there.

## Instructions

Build:

```sh
cd philo && make        # or: cd philo_bonus && make
```

Run:

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

- `number_of_philosophers` — also the number of forks.
- `time_to_die`, `time_to_eat`, `time_to_sleep` — in milliseconds.
- `number_of_times_each_philosopher_must_eat` — optional; if given, the
  simulation stops once every philosopher has eaten that many times, instead
  of running until someone dies.

Examples:

```sh
./philo 5 800 200 200      # runs until a philosopher dies (or forever if none does)
./philo 5 800 200 200 7    # stops once everyone has eaten 7 times
```

`make clean`, `make fclean`, `make re` behave as usual.

## Technical notes

- **Deadlock avoidance**: all philosophers pick up their forks in the same
  order (left, then right). Deadlock is prevented by a mutex-protected
  "room" counter that lets at most `n - 1` philosophers attempt to pick up
  forks at the same time — with `n` forks and at most `n - 1` philosophers
  competing, at least one of them can always get both of theirs.
- **Starvation mitigation**: room admission additionally gives priority to
  whichever waiting philosopher has gone longest without eating, instead of
  first-come-first-served. Without real-time scheduling guarantees this is a
  mitigation, not a formal proof — see the note below.
- Every log line carries the elapsed time in milliseconds since the start of
  the simulation, and is only ever printed while the simulation is still
  running, so a death message can't race with a state message printed after
  the fact.

### Known limitation

The mandatory subject forbids semaphores and condition variables, so there is
no OS-level FIFO queue to fall back on for forks — fairness comes only from
the room + priority scheme above. Under an artificially tight, perfectly
lockstepped stress test (fresh process, `n` philosophers, `time_to_die`
exactly `2×(time_to_eat+time_to_sleep)`, run back-to-back many times) a
philosopher can, rarely, still lose two consecutive fork races and die right
at the deadline. This was reduced from a near-certain failure to an
occasional one during this review; closing it completely would need a real
FIFO ticket queue, which is out of scope for a mutex-only mandatory part.

## Resources

- [Dining philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthreads(7) man page](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- `man pthread_mutex_lock`, `man pthread_create`, `man sem_overview`
- E.W. Dijkstra's original 1965 formulation of the problem (via the
  Wikipedia article above) for the "waiter"/arbitrator solution to
  starvation.

**AI usage**: AI (Claude, Anthropic) was used during this project's review
pass to: read the subject PDF and diff its requirements against the existing
code, locate and explain three concurrency bugs (a self-deadlock with a
single philosopher, a death-timer that measured the wrong window, and a
false-death after a philosopher had already met its required meal count),
design and implement the mutex-only fairness fix described above, restructure
the repository into `philo/`/`philo_bonus/`, write the `philo_bonus`
process+semaphore implementation, and draft this README. Every change was
reviewed, tested (including repeated stress runs of the classic
`5 800 200 200` benchmark), and understood before being kept.
