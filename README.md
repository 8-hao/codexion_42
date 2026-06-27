*This project has been created as part of the 42 curriculum by obakri.*
# Codexion

## Description
codexion is a multithreaded simulation, conceptually similar to the classic "Dining Philosophers" exercise. Instead of philosophers and forks, the simulation models **coders** sitting around a shared co-working hub, competing for a limited number of **USB dongles** needed to compile quantum code.

Each coder repeatedly cycles through three phases: **compiling**, **debugging**, and **refactoring**. Compiling requires holding two dongles simultaneously (one on each side), and dongles are shared between adjacent coders in a circular arrangement — meaning the number of dongles always equals the number of coders. After a dongle is released, it enters a **cooldown** period during which it cannot be reused. Coders that fail to start a new compile cycle before their personal `time_to_burnout` deadline expires **burn out**, which immediately ends the simulation.

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
