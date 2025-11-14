#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include "main.h"
#include "software_timer.h"
#include <stdlib.h>
#include <stdio.h>
typedef struct S_Task {
    void (*pTask)();
    int Delay;
    int Period;
    struct S_Task* next;
} S_Task;

typedef struct {
    S_Task* head;
    int numTask;
} Container;



S_Task* addNode(void(*pFunction)(), int delay, int period);
void SCH_Init(void);
void SCH_Add_Task(void(*pFunction)(), int PERIOD, int DELAY);
void SCH_Delete_Task(void);
void SCH_Update_Task(void);
void SCH_Dispatch_Task(void);


#endif /* INC_SCHEDULER_H_ */
