# c-yield

Yielding in C.

## About

Yielding is a mechanism to pause/continue function execution. This project explores patterns to do that in C.
The current approach uses state machines.

### How does it work?

Essentially, each function has a wrapper struct with state about its execution. Yielding is simply
modifying the state before returning, so that the continue point is known on the next call.

The control flow is managed by a worker that picks up items from a queue. If a function yields, it goes back into
the queue. Some yields may be nested. This is handled by letting each item have an optional reference to a child item.
The worker makes sure not to unyield any item before its child item is completed.
