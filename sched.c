#include "decls.h"
#include "sched.h"

#define MAX_TASK 10

static struct Task Tasks[MAX_TASK];
static struct Task *current;


uint32_t edi;
uint32_t esi;
uint32_t ebp;
uint32_t esp;
uint32_t ebx;
uint32_t edx;
uint32_t ecx;
uint32_t eax;
/* below here defined by x86 hardware */
uint32_t eip;
uint16_t cs;
uint16_t padding;
uint32_t eflags;


static void testtest() {
  while (1) {
    vga_write("kern2 testing.............", 2, 0x70);
  }
}


void sched_init() {
    for (int i = 0; i < MAX_TASK; i++) {
      Tasks[i].status = FREE;
      Tasks[i].pid = i;
    }

    current = &Tasks[0];

    uint8_t* stack = &current->stack[4096];
    current->frame = stack - sizeof(struct TaskFrame);
    current->frame->edi = 0;
    current->frame->esi = 0;
    current->frame->ebp = 0;
    current->frame->esp = current->frame;
    current->frame->ebx = 0;
    current->frame->edx = 0;
    current->frame->ecx = 0;
    current->frame->eax = 0;
    current->frame->eip = 0;
    current->frame->cs = CODE_SEGMENT;
    current->frame->padding = 0;
    current->frame->eflags = INTERRUPT_ENABLE_FLAG;
    current->status = RUNNING;

}

void spawn(void (*entry)(void)) {
    int index = 0;
    while (Tasks[index].status != FREE) {
      index++;
    }
    uint8_t* stack = &Tasks[index].stack[4096];
    Tasks[index].frame = stack - sizeof(struct TaskFrame);

    Tasks[index].frame->edi = 0;
    Tasks[index].frame->esi = 0;
    Tasks[index].frame->ebp = 0;
    Tasks[index].frame->esp = Tasks[index].frame;
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
