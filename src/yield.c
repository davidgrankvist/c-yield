#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include "yield.h"

#define MAX_WORK_ITEMS 1000

typedef struct {
    WorkItem* items[MAX_WORK_ITEMS];
    int count;
    int pending;
} WorkItemQueue;

WorkItemQueue queuev = {0};
WorkItemQueue* queue = &queuev;

static WorkItem* CreateWorkItem(WorkFunc fn) {
    WorkItem* item = calloc(1, sizeof(WorkItem));
    assert(item != NULL);

    item->DoWork = fn;
    return item;
}

void QueueWorkFunc(WorkFunc fn) {
    assert(queue->count + 1 < MAX_WORK_ITEMS);
    queue->items[queue->count++] = CreateWorkItem(fn);
    queue->pending++;
}

void QueueWorkItem(WorkItem* item) {
    assert(queue->count + 1 < MAX_WORK_ITEMS);
    queue->items[queue->count++] = item;
    queue->pending++;
}

void YieldItem(WorkItem* item) {
    item->state++;
    item->yieldState = ItemYielding;
}

bool ContinueHere(WorkItem* item) {
    return item->state == item->here++;
}

static bool IsWaitingForChild(WorkItem* item) {
    return item->yieldState == ItemYielding &&
    item->child != NULL && item->child->yieldState == ItemYielding;
}

void ProcessWorkItems() {
    int current = 0;
    while (queue->pending > 0) {
        WorkItem *item = queue->items[current];

        if (item == NULL || item->yieldState == ItemDone || IsWaitingForChild(item)) {
            current = (current + 1) % queue->count;
            continue;
        }

        int prevState = item->state;
        item->here = 0;
        item->DoWork(item);

        if (item->yieldState != ItemDone && item->state == prevState) {
            item->yieldState = ItemDone;
        }

        if (item->yieldState == ItemDone) {
            queue->pending--;
            free(item);
        }

        current = (current + 1) % queue->count;
    }
}

bool WaitForFunc(WorkItem* item, WorkFunc fn) {
    WorkItem* child = CreateWorkItem(fn);

    fn(child);

    bool didYield = child->yieldState == ItemYielding;
    if (didYield) {
        item->child = child;
        QueueWorkItem(child);
    } else {
        free(child);
    }

    return didYield;
}
