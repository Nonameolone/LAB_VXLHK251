#include "Scheduler.h"

static Container* container = NULL;

S_Task* addNode(void (*pFunction)(), int delay, int period) {
    S_Task* newNode = (S_Task*)malloc(sizeof(S_Task));
    if (newNode == NULL) return NULL;
    newNode->pTask = pFunction;
    newNode->Delay = delay;
    newNode->Period = period;
    newNode->next = NULL;
    return newNode;
}

void SCH_Init(void) {
    container = (Container*)malloc(sizeof(Container));
    container->head = NULL;
    container->numTask = 0;
}

void SCH_Add_Task(void (*pFunction)(), int PERIOD, int DELAY) {
    if (container == NULL) SCH_Init();

    S_Task* newNode = addNode(pFunction, DELAY, PERIOD);
    if (newNode == NULL) return;

    if (container->head == NULL) {
        container->head = newNode;
    } else {
        S_Task* temp = container->head;
        S_Task* prev = NULL;
        int totalDelay = 0;

        while (temp != NULL && (totalDelay + temp->Delay) <= DELAY) {
            totalDelay += temp->Delay;
            prev = temp;
            temp = temp->next;
        }

        newNode->Delay -= totalDelay;

        if (prev == NULL) {
            // Thêm đầu
            newNode->next = container->head;
            container->head->Delay -= newNode->Delay;
            container->head = newNode;
        } else {
            // Thêm giữa hoặc cuối
            newNode->next = temp;
            prev->next = newNode;
            if (temp != NULL)
                temp->Delay -= newNode->Delay;
        }
    }
    container->numTask++;
}

void SCH_Delete_Task(void) {
    if (container == NULL || container->head == NULL) return;
    S_Task* temp = container->head;
    container->head = temp->next;
    free(temp);
    container->numTask--;
}

void SCH_Update_Task(void) {
    if (container != NULL && container->head != NULL) {
        if (container->head->Delay > 0)
            container->head->Delay--;
    }
}

void SCH_Dispatch_Task(void) {
    if (container != NULL && container->head != NULL && container->head->Delay == 0) {
        S_Task* task = container->head;
        task->pTask();

        if (task->Period > 0) {
            SCH_Add_Task(task->pTask, task->Period, task->Period);
        }
        SCH_Delete_Task();
    }
}





