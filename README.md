# c-yield

Yielding in C.

## About

Yielding is a mechanism to pause/continue function execution. This project explores simple patterns to do that in C.

### How does it work?

The current pattern uses state machines:
- work items are added to a queue
- a worker picks up an item and runs its attached callback
- the callback yields by modifying the state before returning
- the next time it is called, it uses the state to determine which parts to execute
