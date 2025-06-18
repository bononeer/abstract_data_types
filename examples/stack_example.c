#include <stdio.h>
#include <string.h>
#include "adt/stack.h"

/* ############################### Algorithms ############################### */

// Check if a stack of integers is ordered. The stack is sorted if, from top to 
// bottom, the following number is greater than the last one seen.
bool ordered_stack(stack_t *stack) {
    // an empty stack will be considered ordered
    if (stack_is_empty(stack)) {
        return true;
    }

    // already checked that the stack is not empty, won't have to worry about
    // ENODATA, can pop freely. (Could still have ENOMEM, but not probable for
    // this example)
    int top = *(int *) stack_pop(stack);

    bool ok = ordered_stack(stack);
    if (!ok) {
        return false;
    }

    // check for that stack is not empty to prevent ENODATA when all the elements
    // were popped recursively
    if (!stack_is_empty(stack)) {
        if (top > *(int *) stack_top(stack)) {
            return false;
        }
    }

    // add back the top recurisvely to not lose any data, and to keep the checks
    // correctly for each recursion
    stack_push(stack, &top);

    return true;
}

// for a string that may only contain the following characters: (), {}, []
// check that the sequence is balanced
// examples:
// [{([])}] => true
// [{} => false
// ()[{]} => false 
bool balanced_sequence(char *seq) {
    stack_t *aux = stack_create(NULL);
    size_t len = strlen(seq);

    for (int i = 0 ; i < len ; i++) {
        if (seq[i] == '(' || seq[i] == '{' || seq[i] == '[') {
            stack_push(aux, &seq[i]);
            continue;
        }

        if ((seq[i] == ')' || seq[i] == '}' || seq[i] == ']') && stack_is_empty(aux)) {
            return false;
        }

        if (seq[i] == ')' && *(char *) stack_top(aux) != '(') {
            return false;
        }

        if (seq[i] == '}' && *(char *) stack_top(aux) != '{') {
            return false;
        }

        if (seq[i] == ']' && *(char *) stack_top(aux) != '[') {
            return false;
        }

        stack_pop(aux);
    }

    return stack_is_empty(aux);
}

/* ################################## Main ################################## */

int main(void) {
    errno = 0;

    // Stacks that won't free the memory of its elements.
    stack_t *my_stack = stack_create(NULL);
    stack_t *other_stack = stack_create(NULL);
    int nums[] = {5, 4, 3, 2, 1};
    int nums2[] = {10, 6, 2, 4, 3};

    for (int i = 0 ; i < 5 ; i++) {
        stack_push(my_stack, &nums[i]);
        stack_push(other_stack, &nums2[i]);
    }


    printf("my_stack is an ordered Stack: %s\n", ordered_stack(my_stack) ? "true" : "false");
    printf("other_stack is an ordered Stack: %s\n", ordered_stack(other_stack) ? "true" : "false");

    stack_destroy(my_stack);
    stack_destroy(other_stack);

    char *s1 = "[{([])}]";
    char *s2 = "[{}";
    char *s3 = "()[{]}";
    char *s4 = "()([]){[[]]}";

    printf("%s is balanced: %s\n", s1, balanced_sequence(s1) ? "true" : "false");
    printf("%s is balanced: %s\n", s2, balanced_sequence(s2) ? "true" : "false");
    printf("%s is balanced: %s\n", s3, balanced_sequence(s3) ? "true" : "false");
    printf("%s is balanced: %s\n", s4, balanced_sequence(s4) ? "true" : "false");

    return 0;
}