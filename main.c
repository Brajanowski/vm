/* gcc vm.c -o vm */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* define some arithmetic instructions */
#define IADD 0x00
#define ISUB 0x01
#define IMUL 0x02
#define IDIV 0x03

/* print instruction */
#define PRNT 0x04

/* push instruction */
#define PUSH 0x05

/* define stack size */
#define STACK_SIZE 0x80

/* implementation of stack based virtual machine */
typedef struct vm
{
    unsigned int esp; /* instruction pointer */
    unsigned char* code; /* pointer to code */
    unsigned int size; /* size of code */
    unsigned int stack[STACK_SIZE]; /* stack */
    unsigned int pos; /* current stack position */
} vm_t;

/* creating new vm */
vm_t* vm_new(unsigned char* code, unsigned int size)
{
    if (!code || !size)
        return 0;

    vm_t* vm = malloc(sizeof(vm_t));
    vm->code = code;
    vm->size = size;
    
    /* clean up our stack */
    memset(vm->stack, 0, sizeof(unsigned int) + STACK_SIZE);

    return vm;
}

/* run vm */
void vm_run(vm_t* vm)
{
    if (!vm)
        return;
    
    /* start from beginning */
    vm->esp = 0;
    vm->pos = 0;
    
    /* do the code */
    while (vm->size > vm->esp)
    {
        switch (vm->code[vm->esp])
        {
            case IADD:
            {
                unsigned int result = vm->stack[--vm->pos] + vm->stack[--vm->pos];
                vm->stack[vm->pos++] = result;
                break;
            }

            case ISUB:
            {
                unsigned int result = vm->stack[--vm->pos] - vm->stack[--vm->pos];
                vm->stack[vm->pos++] = result;
                break;
            }

            case IMUL:
            {
                unsigned int result = vm->stack[--vm->pos] * vm->stack[--vm->pos];
                vm->stack[vm->pos++] = result;
                break;
            }

            case IDIV:
            {
                unsigned int result = vm->stack[--vm->pos] / vm->stack[--vm->pos];
                vm->stack[vm->pos++] = result;
                break;
            }

            case PUSH:
            {
                vm->esp++;
                vm->stack[vm->pos++] = vm->code[vm->esp];
                break;
            }

            case PRNT:
            {
                vm->esp++;
                printf("%d\n", vm->stack[--vm->pos]);
                break;
            }
        }

        vm->esp++; /* incrementing instruction pointer */
    }
}

void vm_del(vm_t* vm)
{
    if (!vm)
        return;
    
    free(vm);
    vm = 0;
}

int main()
{
    /* output should be 6 */
    unsigned char code[] = {
        PUSH, 0x02,
        PUSH, 0x02,
        PUSH, 0x02,
        IMUL,
        IADD,
        PRNT
    };

    vm_t* vm = vm_new(code, sizeof(code) / sizeof(*code));
    vm_run(vm);
    vm_del(vm);
    return 0;
}
