#include <linux/sched.h>
#include <stdio.h>
struct task_struct *task;

for_each_process(task) {
/* on each iteration task points to the next task */
}