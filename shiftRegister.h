#ifndef SHIFT_REGISTER
#define SHIFT_REGISTER

typedef struct SRNode{
    char c;
    SRNode *next = NULL;
} SRNode;

typedef struct {
    SRNode *head = NULL, *tail = NULL;
} ShiftRegister;

void popHead(ShiftRegister *sr);

void addTail(ShiftRegister *sr, char c);

#endif