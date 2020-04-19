#include <stdlib.h>
#include "shiftRegister.h"


char popHead(ShiftRegister *sr) {
    
    
    SRNode *head = sr -> head;
    SRNode *tail = sr -> tail;
    if (head == NULL) 
        return 0;

    char c = head-> val;
    if (head == tail) {
        free head;
        head = tail = NULL;
        return c;        
    }

    SRNode *tmp = head;
    head = head -> next;
    free tmp;

    return c;
    
    
}

void addTail(ShiftRegister *sr, char c) {
    SRNode *head = sr -> head;
    SRNode *tail = sr -> tail;

    SRNode *newNode = (SRNode*) calloc(1, sizeof(SRNode));
    newNode -> c = c;

    if (head == NULL) {
        head = newNode;
        tail = head;
        
    }
    else {
        SRNode *tmp = tail;
        tail = newNode;
        tmp-> next = newNode;
    }
        
}

void destroyShiftRegister(ShiftRegister *sr) {
    while (sr -> head != NULL) 
        popHead(sr);
    free sr;
    sr = NULL;
}