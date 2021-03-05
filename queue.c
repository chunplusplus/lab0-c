#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (q == NULL)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q == NULL)
        return;

    while (q->head != NULL) {
        q_remove_head(q, NULL, 0);
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */

    if (newh == NULL)
        return false;

    newh->next = q->head;
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));

    if (newh->value == NULL) {
        free(newh);
        return false;
    }

    strncpy(newh->value, s, strlen(s));
    newh->value[strlen(s)] = '\0';

    q->head = newh;
    if (q->tail == NULL)
        q->tail = newh;
    q->size++;

    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (q == NULL)
        return false;

    list_ele_t *newt;
    newt = malloc(sizeof(list_ele_t));

    if (newt == NULL)
        return false;

    newt->next = NULL;
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));

    if (newt->value == NULL) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s));
    newt->value[strlen(s)] = '\0';

    if (q->tail)
        q->tail->next = newt;
    q->tail = newt;

    if (q->head == NULL)
        q->head = newt;
    q->size++;

    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (q == NULL || q->head == NULL)
        return false;

    list_ele_t *ele = q->head;
    if (sp != NULL) {
        int cpysize = strlen(ele->value) > (bufsize - 1) ? bufsize - 1
                                                         : strlen(ele->value);
        strncpy(sp, ele->value, cpysize);
        sp[cpysize] = '\0';
    }

    q->head = q->head->next;
    if (q->head == NULL)
        q->tail = NULL;
    q->size--;

    free(ele->value);
    free(ele);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q == NULL ? 0 : q->size;
}

void ele_reverse(list_ele_t *ele, list_ele_t *prev)
{
    if (ele == NULL)
        return;
    ele_reverse(ele->next, ele);
    ele->next = prev;
}
/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL)
        return;

    list_ele_t *ele = q->head;

    if (ele == NULL || ele->next == NULL)
        return;

    list_ele_t *next = ele->next;

    while (next != NULL) {
        list_ele_t *temp = next->next;

        next->next = ele;

        ele = next;
        next = temp;
    }

    q->head->next = NULL;

    // ele_reverse(q->head, NULL);
    list_ele_t *temp = q->head;
    q->head = q->tail;
    q->tail = temp;
}


/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if (!l2)
        return l1;
    if (!l1)
        return l2;

    if (strcmp(l1->value, l2->value) > 0) {
        l2->next = merge(l1, l2->next);
        return l2;
    } else {
        l1->next = merge(l1->next, l2);
        return l1;
    }
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);

    return merge(l1, l2);
}

void list_concat(list_ele_t **tail, list_ele_t *right)
{
    //while (*left) {
    //    left = &((*left)->next);
    //}
    //*left = right;
    if (*tail) 
        (*tail)->next = right;
    else 
	*tail = right;
}

//void q_concat

void list_add_ele_t(list_ele_t **list, list_ele_t *ele)
{
    ele->next = *list;
    *list = ele;
}

void q_sort(queue_t *q)
{
    if (q == NULL || q->head == NULL || q->head->next == NULL)
        return;

        // q->head = merge_sort(q->head);
        // q->tail = q->head;
        // while (q->tail && q->tail->next != NULL)
        //    q->tail = q->tail->next;

#define MAX_LEVELS 300

    list_ele_t *beg[MAX_LEVELS],
        *end[MAX_LEVELS];  // use to record range for quick sort
    beg[0] = NULL; //q->head;
    end[0] = NULL;

    int cur_index = 0;

    do {
	
        list_ele_t *pivot = beg[cur_index] ? (beg[cur_index])->next : q->head;
        // char *value = pivot->value;

        list_ele_t *p = pivot->next;
        pivot->next = NULL;

        list_ele_t *left = NULL, *right = NULL;
        list_ele_t *left_tail = NULL, *right_tail = NULL;

        // quick sort in the range
        while (p != end[cur_index]) {
            list_ele_t *n = p;
            p = p->next;

            if (strcmp(n->value, pivot->value) > 0) {
                list_add_ele_t(&right, n);
                if (right_tail == NULL)
                    right_tail = n;
            } else {
                list_add_ele_t(&left, n);
                if (left_tail == NULL)
                    left_tail = n;
            }
        }

        list_ele_t *result = NULL, *tail = NULL;

        // cut the queue to pivot and concat to result
/*
	if (q->head != pivot) {
            list_ele_t *list_head = q->head;
            while (list_head->next && list_head->next != pivot)
                list_head = list_head->next;
            list_head->next = NULL;
            list_concat(&result, q->head);
        }
*/
        	
	if (beg[cur_index])
	{
            tail = beg[cur_index];
	    //->next = NULL;
            list_concat(&result, q->head);	    
	}
        
	if (left_tail) {	
	    list_concat(result ? &tail : &result, left);
	    tail = left_tail;
	}

        list_concat(result ? &tail : &result, pivot);
	tail = pivot;

	if (right_tail) {		
            list_concat(&tail, right);
	    tail = right_tail;
	}

        if (p)
            list_concat(&tail, p);
	else 
	    q->tail = tail;

        cur_index--;

        if (left && left->next) {
            cur_index++;
            //beg[cur_index] = left; // ptr no need to change
            end[cur_index] = left_tail->next;
        }

        if (right && right->next) {
            cur_index++;
            beg[cur_index] = pivot;
            end[cur_index] = right_tail->next;
        }

        q->head  = result;
    } while (cur_index >= 0);

    //q->tail = q->head;
    //while (q->tail && q->tail->next)
    //    q->tail = q->tail->next;
}
