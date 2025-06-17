#include <stdio.h>
#include "yield.h"

void DoThingA(WorkItem* item) {
    if (ContinueHere(item)) {
        printf("A 1\n");
        return YieldItem(item);
    } else if (ContinueHere(item)) {
        printf("A 2\n");
        return YieldItem(item);
    }
    printf("A 3\n");
}

void DoThingB(WorkItem* item) {
    if (ContinueHere(item)) {
        printf("B 1\n");
        return YieldItem(item);
    }
    printf("B 2\n");
    printf("B 3\n");
}

int main() {
    WorkItemQueue queue = {0};
    QueueWorkFunc(&queue, &DoThingA);
    QueueWorkFunc(&queue, &DoThingB);
    ProcessWorkItems(&queue);
}
