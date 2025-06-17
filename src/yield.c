#include <assert.h>
#include "yield.h"

void QueueWorkFunc(WorkItemQueue* queue, WorkFunc fn) {
    assert(queue->count + 1 < MAX_WORK_ITEMS);
    queue->items[queue->count++] = (WorkItem) { 0, 0, ItemPending, fn };
    queue->pending++;
}

void YieldItem(WorkItem* item) {
    item->state++;
    item->yieldState = ItemYielding;
}

int ContinueHere(WorkItem* item) {
    return item->state == item->here++;
}

void ProcessWorkItems(WorkItemQueue* queue) {
    int current = 0;
    while (queue->pending > 0) {
        WorkItem *item = &queue->items[current];

        if (item->yieldState == ItemDone) {
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
        }

        current = (current + 1) % queue->count;
    }
}
