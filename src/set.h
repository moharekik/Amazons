#ifndef _AMAZON_SET_H_
#define _AMAZON_SET_H_

struct set
{
    unsigned int *data;
    unsigned int size;
};

/**
 * @brief Create a set object and returns a pointer to it
 *
 * @param n
 * @return struct set*
 */
struct set *set_alloc(unsigned int n);

/**
 * @brief Free a set object
 *
 * @param S
 */
void set_free(struct set *S);

/**
 * @brief Remove an element from a set
 *
 * @param elem
 * @param S
 */
void set_remove(unsigned int elem, struct set *S);

/**
 * @brief Remove the last element of a set
 *
 * @param s
 * @return Last element : unsigned int
 */
unsigned int set_pop(struct set *s);

/**
 * @brief Add an element at the end of a set
 *
 * @param s
 * @param val
 */
void set_push(struct set *s, unsigned int val);

/**
 * @brief Print a set
 *
 * @param S
 */
void set_print(struct set *S);

// ===================== Tools on Arrays =====================

/**
 * @brief Check if an elemement is in an array
 *
 * @param elem
 * @param tab
 * @param l_tab
 * @return int
 */
int is_in_array(unsigned int elem, unsigned int *tab, unsigned int len_tab);

#endif // _AMAZON_SET_H_