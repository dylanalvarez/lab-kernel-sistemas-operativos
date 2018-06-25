#include "decls.h"
#include "sched.h"

#define MAX_TASK 10

static struct Task Tasks[MAX_TASK];
static struct Task *current;

void sched_init() {
    for (int i = 0; i < MAX_TASK; i++) {
      Tasks[i].status = FREE;
      Tasks[i].pid = i;
    }
    current = &Tasks[0];
    current->frame = (struct TaskFrame *) (&Tasks[0].stack[4096]);
    current->status = RUNNING;
}

void spawn(void (*entry)(void)) {
    int index = 0;
    while (Tasks[index].status != FREE) {
      index++;
    }
    uint8_t* stack = &Tasks[index].stack[4096];
    Tasks[index].frame = (struct TaskFrame *)(stack - sizeof(struct TaskFrame));
    Tasks[index].frame->edi = 0;
    Tasks[index].frame->esi = 0;
    Tasks[index].frame->ebp = 0;
    Tasks[index].frame->esp = (uint32_t) Tasks[index].frame;
    Tasks[index].frame->ebx = 0;
    Tasks[index].frame->edx = 0;
    Tasks[index].frame->ecx = 0;
    Tasks[index].frame->eax = 0;
    Tasks[index].frame->eip = (uint32_t) entry;
    Tasks[index].frame->cs = CODE_SEGMENT;
    Tasks[index].frame->padding = 0;
    Tasks[index].frame->eflags = INTERRUPT_ENABLE_FLAG;
    Tasks[index].status = READY;
}

void sched(struct TaskFrame *tf) {
  int index = current->pid;
  do {
    index++;
    bool is_out_of_bounds = index >= MAX_TASK;
    index = is_out_of_bounds ? 0 : index;
  } while (Tasks[index].status != READY);
  current->status = READY;
  current->frame = tf;
  current = &Tasks[index];
  current->status = RUNNING;
  asm("movl %0, %%esp\n"
      "popa\n"
      "iret\n"
      :
      : "g"(current->frame)
      : "memory");
}

