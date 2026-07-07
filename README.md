*This project has been created as part of the 42 curriculum by obakri.*
# Codexion

## Description
codexion is a multithreaded simulation, conceptually similar to the classic "Dining Philosophers" exercise. Instead of philosophers and forks, the simulation models **coders** sitting around a shared co-working hub, competing for a limited number of **USB dongles** needed to compile quantum code.

Each coder repeatedly cycles through three phases: **compiling**, **debugging**, and **refactoring**. Compiling requires holding two dongles simultaneously (one on each side), and dongles are shared between adjacent coders in a circular arrangement meaning the number of dongles always equals the number of coders. After a dongle is released, it enters a **cooldown** period during which it cannot be reused. Coders that fail to start a new compile cycle before their personal `time_to_burnout` deadline expires **burn out**, which immediately ends the simulation.

The goal of the project is to implement a correct, deadlock-free, starvation-free concurrent system using POSIX threads (`pthread`), mutexes, and condition variables, while also implementing a basic scheduling policy (FIFO or EDF) to arbitrate fair access to contested dongles.

The simulation stops under exactly one of two conditions:
- A coder burns out (detected by a dedicated monitor thread, within 10ms of the actual burnout moment), or
- Every coder has completed at least `number_of_compiles_required` compile cycles.


## Instructions
### Compilation

```bash
make
```
This builds the `codexion` binary using `cc` with `-Wall -Wextra -Werror -pthread`, as required by the subject.

### Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
All arguments are mandatory and must be valid positive integers (except `scheduler`, which must be exactly `fifo` or `edf`). Invalid input (negative numbers, non-integer values, zero where not allowed, or an unrecognized scheduler) is rejected with an explanatory error message.

**Example:**

```bash
./codexion 5 800 100 100 100 3 50 fifo
```
This runs a simulation with 5 coders, an 800ms burnout window, 100ms for each phase, 3 required compiles per coder, a 50ms dongle cooldown, and FIFO arbitration.

### Output format

Every state change is logged as:

```
timestamp_in_ms coder_id has taken a dongle
timestamp_in_ms coder_id is compiling
timestamp_in_ms coder_id is debugging
timestamp_in_ms coder_id is refactoring
timestamp_in_ms coder_id burned out
```

## Blocking cases handled
**Deadlock prevention (Coffman's conditions):** The classic deadlock risk here is hold-and-wait: a coder grabs one dongle and waits indefinitely for the second, while its neighbor does the symmetric thing with the other dongle, and neither can proceed. To break this condition, dongle acquisition is made atomic: a coder never holds one dongle while waiting for another. All decisions about who gets which dongles happen under a single global state mutex, and a coder only physically locks its two dongle mutexes once it has already been granted both under that mutex, back to back, with no waiting in between. This removes the possibility of a partial acquisition ever being observed by another thread, which in turn removes the circular-wait condition needed for deadlock.

**Starvation prevention:** Because arbitration is delegated to a scheduler policy (FIFO or EDF) rather than left to raw lock contention, no coder can be perpetually skipped by an adjacent coder that simply happens to request dongles more often. Under FIFO, requests are served strictly in arrival order. Under EDF, the coder closest to burning out is always prioritized, which directly protects against starvation in the exact dimension that matters for this simulation: nobody gets starved out of time, even if they occasionally lose short-term races for a dongle.

**Cooldown handling:** After a coder releases a dongle, that dongle must remain unavailable for dongle_cooldown milliseconds before it can be claimed again. This is enforced by stamping each dongle with the timestamp of its next eligible acquisition time on release, and having any coder that wants it wait (via a timed condition wait) until that deadline has passed, rather than busy-polling or ignoring the cooldown window.

**Precise burnout detection:** A dedicated monitor thread continuously polls every coder's last compile start time against the burnout threshold at a fine-grained interval, so that a coder who fails to start compiling in time is detected and logged within a few milliseconds of the actual violation, not at the mercy of coarse scheduling. Once a burnout is detected, the monitor immediately flags a global stop condition and wakes every waiting coder thread so the whole simulation halts promptly instead of letting other threads linger in their current phase.

**Log serialization:** All state-change messages (taking a dongle, compiling, debugging, refactoring, burning out) are written through a single dedicated print function that holds one shared logging mutex for the duration of the print. This guarantees that two threads can never have their output interleaved on the same line, regardless of how many coders are running concurrently.

## Thread synchronization mechanisms
Each coder is represented by its own thread, and coordination between them relies on a small number of primitives, each with a specific responsibility.

**Per-coder mutex (safe_check):** Each coder has its own private mutex protecting fields that are read by both its own thread and the monitor thread concurrently: its last compile start time, its finish flag, its stop flag, and its compile count. Every read or write of these fields goes through this mutex, which is what lets the monitor safely inspect a coder's progress from a different thread without racing against the coder updating that same state.

**Per-dongle mutex (mutex_v):** Separately from the arbitration bookkeeping, each dongle also has its own mutex, which is held for the actual duration a coder is compiling with that dongle. This is what physically prevents two coders from being considered to be holding the same dongle's resource at once, complementing the logical is_available flag protected by m_check.

**Logging mutex (p_safe):** A dedicated mutex used exclusively around the print statement itself, ensuring atomic, non-interleaved output across all threads.

**Coordinating coders and the monitor:** The monitor thread never directly halts a coder mid-instruction; instead, it sets each coder's stop flag (under that coder's own safe_check mutex) and broadcasts on the shared condition variable. Every blocking point in a coder's lifecycle waiting for dongles, waiting out a cooldown, sleeping through a compile/debug/refactor phase periodically checks this stop flag, so a stop request is honored promptly from whatever state the coder happens to be in, without needing to forcibly interrupt a thread or risk it missing the signal.

## Resources
- [Dining philosophers problem](http://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2)
- [Philosophers 42 Guide— “The Dining Philosophers Problem”](https:/medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2)
- [Operating System - Process Scheduling](https://www.tutorialspoint.com/operating_system/os_process_scheduling.htm)
- [Multithreading in OS](https://www.geeksforgeeks.org/operating-systems/multithreading-in-operating-system/)
- [Mutex Vs Condition Variables](https://medium.com/@abhisheksagar_59776/mutex-vs-condition-variables-e0a15b2226a4)
- [CS110 Lecture 15: Mutexes and Condition Variables](https://web.stanford.edu/class/cs110/lectures/cs110-win2122-lecture-15.pdf)




Made by ❤️