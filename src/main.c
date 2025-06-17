#include <stdio.h>
#include "yield.h"

void DoThingA(WorkItem* item) {
    YIELD_AFTER(
        printf("A 1\n");
    );
    YIELD_AFTER(
        printf("A 2\n");
    );
    printf("A 3\n");
}

void DoThingB(WorkItem* item) {
    YIELD_AFTER(
        printf("B 1\n");
    );
    printf("B 2\n");
    printf("B 3\n");
}

int main() {
    WorkItemQueue queue = {0};
    QueueWorkFunc(&queue, &DoThingA);
    QueueWorkFunc(&queue, &DoThingB);
    ProcessWorkItems(&queue);
}
