#ifndef yield_h
#define yield_h

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
} WorkItem;

typedef void (*WorkFunc)(struct WorkItem*);

void YieldItem(WorkItem* item);
int ContinueHere(WorkItem* item);

#define YIELD_AFTER(block) \
    do { \
        if (ContinueHere(item)) { \
            block \
            return YieldItem(item); \
        } \
    } while(0)

// -- Work queue --

#define MAX_WORK_ITEMS 1000

typedef struct {
    WorkItem items[MAX_WORK_ITEMS];
    int count;
    int pending;
} WorkItemQueue;

void QueueWorkFunc(WorkItemQueue* queue, WorkFunc fn);
void ProcessWorkItems(WorkItemQueue *queue);

#endif
