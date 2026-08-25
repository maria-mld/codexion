*This project has been created as part of the 42 curriculum by marmoldo.*

# Codexion

## Description

Codexion is a multithreaded C simulation of a classic resource-sharing and
synchronization problem, inspired by the "dining philosophers" problem. A
group of coders sit in a circular co-working hub, sharing a limited number
of USB dongles. Each coder cycles endlessly through three phases —
**compiling**, **debugging**, and **refactoring** — but compiling requires
holding two dongles at once (one shared with each neighbour).

The simulation ends either when every coder has compiled at least a
required number of times, or when a coder "burns out" from waiting too
long without compiling. The goal of the project is to design a fair,
deadlock-free, and starvation-free protocol for sharing the dongles between
concurrent threads, using POSIX threads, mutexes, condition variables, and
a custom-built priority queue for scheduling.

## Instructions

### Compilation

```bash
make
```

This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror
-pthread`.

Other Makefile rules:

```bash
make clean   # remove object files
make fclean  # remove object files and the binary
make re      # rebuild from scratch
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) in the simulation |
| `time_to_burnout` | Time (ms) before a coder burns out without compiling |
| `time_to_compile` | Time (ms) spent compiling |
| `time_to_debug` | Time (ms) spent debugging |
| `time_to_refactor` | Time (ms) spent refactoring |
| `number_of_compiles_required` | Simulation stops once every coder reaches this count |
| `dongle_cooldown` | Time (ms) a dongle stays unavailable after being released |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

Example:

```bash
./codexion 3 5000 800 400 400 3 200 fifo
```

## Resources

- Dijkstra, E. W. — *Hierarchical ordering of sequential processes* (origin
  of the dining philosophers problem)
- `man pthread_mutex_lock`, `man pthread_cond_wait`, `man gettimeofday`
- POSIX Threads Programming (LLNL tutorial)
- Introduction to binary heaps and priority queues (standard CS data
  structures reference material)

### AI usage

AI assistance (Claude) was used throughout this project as a learning and
design aid, not as a code-generation shortcut:

- **Architecture planning**: discussing the overall data structures
  (`t_sim`, `t_coder`, `t_dongle`, `t_heap`) and the separation of
  responsibilities between files before writing any code.
- **Concept explanations**: understanding mutexes, condition variables,
  the dining philosophers analogy, and how a binary min-heap works, in
  order to implement them from scratch ourselves.
- **Debugging guidance**: interpreting Valgrind, Helgrind, AddressSanitizer
  and ThreadSanitizer output to locate and fix a real bug (an
  uninitialised `next_turn_id` field in `t_dongle`, caught by Valgrind's
  memcheck as a "conditional jump depends on uninitialised value").
- **Test scenario design**: suggesting edge cases to test manually
  (single coder, burnout scenarios, high-contention scheduling) to
  validate the implementation's correctness.

All code was written, reviewed, and understood by us. AI was not used to
generate unreviewed code dropped directly into the project.

## Blocking cases handled

- **Deadlock prevention (Coffman conditions)**: dongles are always
  acquired in a fixed order (lowest `id` first), regardless of which one
  is a coder's "left" or "right" dongle. This breaks the circular wait
  condition that causes the classic dining philosophers deadlock, where
  every coder holds one resource while waiting for another held by their
  neighbour.
- **Starvation prevention**: each dongle maintains its own waiting queue,
  implemented as a binary min-heap. Under `fifo`, the coder who requested
  the dongle earliest is always served first; under `edf`, the coder
  closest to burnout is prioritized. This guarantees no coder is
  indefinitely skipped while parameters remain feasible.
- **Dongle cooldown handling**: after a dongle is released, its
  `free_since` timestamp is recorded, and it cannot be reacquired until
  `dongle_cooldown` milliseconds have elapsed. This check happens while
  still holding the dongle's mutex, so no other thread can acquire it
  during the cooldown window.
- **Precise burnout detection**: a dedicated monitor thread polls every
  coder's last compile start time at short intervals, guaranteeing burnout
  is logged within 10 ms of the actual deadline being missed.
- **Log serialization**: all state-change messages go through a single
  logging function protected by a dedicated mutex, ensuring two threads
  can never interleave partial output on the same line.
- **Single-coder edge case**: when `number_of_coders` is 1, the coder's
  left and right dongle point to the same object; the acquisition logic
  detects this and takes it only once, avoiding a self-deadlock on the
  same mutex.

## Thread synchronization mechanisms

- **`pthread_mutex_t` per dongle**: protects `is_taken`, `free_since`, and
  `next_turn_id`. All reads and modifications of a dongle's state happen
  while holding this lock.
- **`pthread_cond_t` per dongle**: coders waiting for a dongle sleep on
  this condition variable instead of busy-waiting, and are woken via
  `pthread_cond_broadcast` whenever the dongle is released. Each waiting
  thread re-checks, after waking, whether it is actually its turn
  (`next_turn_id == coder->id`) before proceeding — this correctly
  handles spurious wakeups and signals meant for other threads.
- **`pthread_mutex_t data_lock` per coder**: protects `last_compile_start`
  and `compiles_done`, which are written by the coder's own thread and
  read by the monitor thread. Without this lock, a data race would occur
  between the coder thread's writes and the monitor's reads; this was
  confirmed by ThreadSanitizer during development, and fixing it removed
  the race entirely.
- **Global `stop_lock`**: protects the shared `stop` flag, checked by all
  coder threads and set by the monitor thread when a burnout occurs or
  when every coder has met the required compile count.
- **Global `log_lock`**: serializes all `printf` calls across every
  thread.
- **Custom binary min-heap (`t_heap`)**: used as the waiting queue for
  each dongle. The heap key is either the request timestamp (`fifo`) or
  the coder's burnout deadline (`edf`), letting both scheduling policies
  share the same underlying data structure. No standard library priority
  queue is used, as required by the subject.

Race conditions were actively tested for and ruled out using Valgrind
(memcheck and helgrind), AddressSanitizer, UndefinedBehaviorSanitizer, and
ThreadSanitizer — all four report zero errors across multiple test
scenarios (normal completion, burnout, single-coder, and high-contention
multi-coder runs).