#ifndef yield_h
#define yield_h

#include <stdbool.h>

// -- Work items --

typedef enum {
    ItemPending,
    ItemYielding,
    ItemDone,
} YieldState;

typedef struct WorkItem {
    int state;
    int here;
    YieldState yieldState;
    void (*DoWork)(struct WorkItem*); // passes in self
    struct WorkItem* child;
} WorkItem;

typedef void (*WorkFunc)(struct WorkItem*);

void YieldItem(WorkItem* item);
bool ContinueHere(WorkItem* item);
bool WaitForFunc(WorkItem* item, WorkFunc fn);

// -- Work queue --

void QueueWorkFunc(WorkFunc fn);
void QueueWorkItem(WorkItem* item);
void ProcessWorkItems();

#endif
