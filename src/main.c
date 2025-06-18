#include <stdlib.h>
#include <stdio.h>
#include "yield.h"

void DoSlowNetworkCall(WorkItem* item) {
    if(ContinueHere(item)) {
        printf("A child - Started slow network call\n");
        return YieldItem(item);
    };
    printf("A child  - Ended slow network call\n");
}

void DoThingA(WorkItem* item) {
    if (ContinueHere(item)) {
        printf("A - Started\n");
        bool didYield = WaitForFunc(item, DoSlowNetworkCall);
        if (didYield) {
            return YieldItem(item);
        }
    }
    printf("A - Done waiting for child\n");
    printf("A - Ended\n");
}

void DoThingB(WorkItem* item) {
    printf("B - Started\n");
    printf("B - Ended\n");
}

int main() {
    QueueWorkFunc(&DoThingA);
    QueueWorkFunc(&DoThingB);
    ProcessWorkItems();
}
