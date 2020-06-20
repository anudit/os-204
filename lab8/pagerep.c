
// Page Replacement Algorithms

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/queue.h>
#include "pagerep.h"


//Configuration variables

int num_frames = 10;           // Number of avaliable pages in page tables
int page_ref_upper_bound = 12; // Largest page reference
int max_page_calls = 1000;     // Max number of page refs to test

int debug = 0;                 // Debug bool, 1 shows verbose output
int printrefs = 0;             // Print refs bool, 1 shows output after each page ref


//Array of algorithm functions that can be enabled
Algorithm algos[7] = { {"OPTIMAL", &OPTIMAL, 0, NULL},
                       {"RANDOM", &RANDOM, 0, NULL},
                       {"FIFO", &FIFO, 0, NULL},
                       {"LRU", &LRU, 0, NULL} };


// Runtime variables
int counter = 0;           // "Time" as number of loops calling page_refs 0...num_refs (used as i in for loop)
int last_page_ref = -1;    // Last ref
size_t num_algos = 0;      // Number of algorithms in algos, calculated in init()
int *optimum_find_test;
int num_refs = 0;          // Number of page refs in page_refs list


// Run algorithm if given correct arguments, else terminate with error
int main ( int argc, char *argv[] )
{
        init();
        if ( !(argc >= 3 && argc <= 5) )
        { // argc should be 3-5 for correct execution
                print_help(argv[0]);
        }
        else
        {
                num_frames = atoi(argv[2]);
                if ( num_frames < 1 )
                {
                        num_frames = 1;
                        printf( "Number of page frames must be at least 1, setting to 1\n");
                }
                if ( argc > 3 )
                {
                        if(atoi(argv[3]) == 1 || atoi(argv[3]) == 0)
                        {
                                printrefs = atoi(argv[3]);
                        }
                        else
                        {
                                printf( "Printrefs must be 1 or 0, ignoring\n");
                        }
                }
                if ( argc > 4 )
                {
                        if(atoi(argv[4]) == 1 || atoi(argv[3]) == 0)
                        {
                                debug = atoi(argv[4]);
                        }
                        else
                        {
                                printf( "Debug must be 1 or 0, ignoring\n");
                        }
                }
                printf("argv[1][0] : %s\n", argv[1]);
                switch(argv[1][0])
                {
                case 'O':
                        algos[0].selected = 1;
                        break;
                case 'R':
                        algos[1].selected = 1;
                        break;
                case 'F':
                        algos[2].selected = 1;
                        break;
                case 'L':
                        algos[3].selected = 1;
                        break;
                default:
                        printf( "%s algorithm is invalid choice or not yet implemented\n", argv[1]);
                        print_help(argv[0]);
                        return 1;
                }
                event_loop();
        }
        cleanup();
        return 0;
}


int init()
{
        gen_page_refs();
        // Calculate number of algos
        num_algos = sizeof(algos)/sizeof(Algorithm);
        size_t i = 0;
        for (i = 0; i < num_algos; ++i)
        {
                algos[i].data = create_algo_data_store();
        }
        return 0;
}


// Generate all page refs to use in tests
void gen_page_refs()
{
        num_refs = 0;
        LIST_INIT(&page_refs);
        Page_Ref *page = gen_ref();
        LIST_INSERT_HEAD(&page_refs, page, pages);
        while(num_refs < max_page_calls)
        { // generate a page ref up too  max_page_calls and add to list
                LIST_INSERT_AFTER(page, gen_ref(), pages);
                page = page->pages.le_next;
                num_refs++;
        }
        // we need look-ahead for Optimal algorithm
        int all_found = 0;
        optimum_find_test = (int*)malloc(page_ref_upper_bound*sizeof(int));
        size_t i;
        for(i = 0; i < page_ref_upper_bound; ++i)
        { // generate new refs until one of each have been added to list
                optimum_find_test[i] = -1;
        }
        while(all_found == 0)
        { // generate new refs until one of each have been added to list
                LIST_INSERT_AFTER(page, gen_ref(), pages);
                page = page->pages.le_next;
                optimum_find_test[page->page_num] = 1;
                all_found = 1;
                for(i = 0; i < page_ref_upper_bound; ++i)
                { // see if we've got them all yet
                        if(optimum_find_test[i] == -1)
                        {
                                all_found = 0;
                                break;
                        }
                }
                num_refs++;
        }
        return;
}


// Generate a random page ref within bounds
Page_Ref* gen_ref()
{
        Page_Ref *page = malloc(sizeof(Page_Ref));
        page->page_num = rand() % page_ref_upper_bound;
        return page;
}


//Creates an empty Algorithm_Data to init an Algorithm
Algorithm_Data *create_algo_data_store()
{
        Algorithm_Data *data = malloc(sizeof(Algorithm_Data));
        data->hits = 0;
        data->misses = 0;
        data->last_victim = NULL;
        /* Initialize Lists */
        LIST_INIT(&(data->page_table));
        LIST_INIT(&(data->victim_list));
        /* Insert at the page_table. */
        Frame *framep = create_empty_frame(0);
        LIST_INSERT_HEAD(&(data->page_table), framep, frames);
        /* Build the rest of the list. */
        size_t i = 0;
        for (i = 1; i < num_frames; ++i)
        {
                LIST_INSERT_AFTER(framep, create_empty_frame(i), frames);
                framep = framep->frames.le_next;
        }
        return data;
}


// Creates an empty Frame for page table list
Frame* create_empty_frame(int index)
{
        Frame *framep = malloc(sizeof(Frame));
        framep->index = index;
        framep->page = -1;
        time(&framep->time);
        framep->extra = 0;
        return framep;
}


// page all selected algorithms with input ref
int event_loop()
{
        counter = 0;
        while(counter < max_page_calls)
        {
                page(get_ref());
                ++counter;
        }
        size_t i = 0;
        for (i = 0; i < num_algos; i++)
        {
                if(algos[i].selected==1) {
                        print_summary(algos[i]);
                }
        }
        return 0;
}


// get a random ref
int get_ref()
{
        if (page_refs.lh_first != NULL)
        { // pop Page_Ref off page_refs
                int page_num = page_refs.lh_first->page_num;
                LIST_REMOVE(page_refs.lh_first, pages);
                return page_num;
        }
        else
        { // just in case
                return rand() % page_ref_upper_bound;
        }
}


// page all selected algorithms with input ref
int page(int page_ref)
{
        last_page_ref = page_ref;
        size_t i = 0;
        for (i = 0; i < num_algos; i++)
        {
                if(algos[i].selected==1) {
                        algos[i].algo(algos[i].data);
                        if(printrefs == 1)
                        {
                                print_stats(algos[i]);
                        }
                }
        }

        return 0;
}


// Add victim frame evicted from page table to list of victims
int add_victim(struct Frame_List *victim_list, struct Frame *frame)
{
        if(debug)
                printf("Victim index: %d, Page: %d\n", frame->index, frame->page);
        struct Frame *victim = malloc(sizeof(Frame));
        *victim = *frame;
        victim->index = 1;
        LIST_INSERT_HEAD(victim_list, victim, frames);
        return 0;
}


// OPTIMAL Page Replacement Algorithm
int OPTIMAL(Algorithm_Data *data)
{
        Frame *framep = data->page_table.lh_first,
              *victim = NULL;
        int fault = 0;
        /* Find target (hit), empty page index (miss), or victim to evict (miss) */
        while (framep != NULL && framep->page > -1 && framep->page != last_page_ref) {
                framep = framep->frames.le_next;
        }
        if(framep == NULL)
        { // It's a miss, find our victim
                size_t i,j;
                for(i = 0; i < page_ref_upper_bound; ++i)
                        optimum_find_test[i] = -1;
                Page_Ref *page = page_refs.lh_first;
                int all_found = 0;
                j = 0;
                //optimum_find_test = malloc(sizeof(int)*page_ref_upper_bound);
                while(all_found == 0)
                {
                        if(optimum_find_test[page->page_num] == -1)
                                optimum_find_test[page->page_num] = j++;
                        all_found = 1;
                        for(i = 0; i < page_ref_upper_bound; ++i)
                                if(optimum_find_test[i] == -1)
                                {
                                        all_found = 0;
                                        break;
                                }
                        page = page->pages.le_next;
                }
                framep = data->page_table.lh_first;
                while (framep != NULL) {
                        if(victim == NULL || optimum_find_test[framep->page] > optimum_find_test[victim->page])
                        { // No victim yet or page used further in future than victim
                                victim = framep;
                        }
                        framep = framep->frames.le_next;
                }
                if(debug) printf("Victim selected: %d, Page: %d\n", victim->index, victim->page);
                add_victim(&data->victim_list, victim);
                victim->page = last_page_ref;
                time(&victim->time);
                victim->extra = counter;
                fault = 1;
        }
        else if(framep->page == -1)
        { // Use free page table index
                framep->page = last_page_ref;
                time(&framep->time);
                framep->extra = counter;
                fault = 1;
        }
        else if(framep->page == last_page_ref)
        { // The page was found! Hit!
                time(&framep->time);
                framep->extra = counter;
        }
        if(debug)
        {
                printf("Page Ref: %d\n", last_page_ref);
                for (framep = data->page_table.lh_first; framep != NULL; framep = framep->frames.le_next)
                        printf("Slot: %d, Page: %d, Time used: %d\n", framep->index, framep->page, framep->extra);
        }
        if(fault == 1) data->misses++; else data->hits++;
        return fault;
}


//int RANDOM(Algorithm_Data *data)
int RANDOM(Algorithm_Data *data)
{
        struct Frame *framep = data->page_table.lh_first,
                     *victim = NULL;
        int rand_victim = rand() % num_frames;
        int fault = 0;
        /* Find target (hit), empty page index (miss), or victim to evict (miss) */
        while (framep != NULL && framep->page > -1 && framep->page != last_page_ref) {
                if(framep->index == rand_victim) // rand
                        victim = framep;
                framep = framep->frames.le_next;
        }
        if(framep == NULL)
        { // It's a miss, kill our victim
                if(debug) printf("Victim selected: %d, Page: %d\n", victim->index, victim->page);
                add_victim(&data->victim_list, victim);
                victim->page = last_page_ref;
                time(&victim->time);
                victim->extra = counter;
                fault = 1;
        }
        else if(framep->page == -1)
        { // Use free page table index
                framep->page = last_page_ref;
                time(&framep->time);
                framep->extra = counter;
                fault = 1;
        }
        else if(framep->page == last_page_ref)
        { // The page was found! Hit!
                time(&framep->time);
                framep->extra = counter;
        }
        if(debug)
        {
                printf("Page Ref: %d\n", last_page_ref);
                for (framep = data->page_table.lh_first; framep != NULL; framep = framep->frames.le_next)
                        printf("Slot: %d, Page: %d, Time used: %d\n", framep->index, framep->page, framep->extra);
        }
        if(fault == 1) data->misses++; else data->hits++;
        return fault;
}


//FIFO Page Replacement Algorithm
int FIFO(Algorithm_Data *data)
{
        struct Frame *framep = data->page_table.lh_first,
                     *victim = NULL;
        int fault = 0;
        /* Find target (hit), empty page index (miss), or victim to evict (miss) */
        while (framep != NULL && framep->page > -1 && framep->page != last_page_ref) {
                if(victim == NULL || framep->time > victim->time)
                { // No victim yet or frame older than victim
                        victim = framep;
                }
                framep = framep->frames.le_next;
        }
        /* Make a decision */
        if(framep == NULL)
        { // It's a miss, kill our victim
                if(debug) printf("Victim selected: %d, Page: %d\n", victim->index, victim->page);
                add_victim(&data->victim_list, victim);
                victim->page = last_page_ref;
                time(&victim->time);
                victim->extra = counter;
                fault = 1;
        }
        else if(framep->page == -1)
        { // Can use free page table index
                framep->page = last_page_ref;
                time(&framep->time);
                framep->extra = counter;
                fault = 1;
        }
        else if(framep->page == last_page_ref)
        { // The page was found! Hit!
                time(&framep->time);
                framep->extra = counter;
        }
        if(fault == 1) data->misses++; else data->hits++;
        return fault;
}



// LRU Page Replacement Algorithm
int LRU(Algorithm_Data *data)
{
        struct Frame *framep = data->page_table.lh_first,
                     *victim = NULL;
        int fault = 0;
        /* Find target (hit), empty page index (miss), or victim to evict (miss) */
        while (framep != NULL && framep->page > -1 && framep->page != last_page_ref) {
                if(victim == NULL || framep->time < victim->time)
                        victim = framep; // No victim yet or frame older than victim
                framep = framep->frames.le_next;
        }
        /* Make a decision */
        if(framep == NULL)
        { // It's a miss, kill our victim
                if(debug) printf("Victim selected: %d, Page: %d\n", victim->index, victim->page);
                add_victim(&data->victim_list, victim);
                victim->page = last_page_ref;
                time(&victim->time);
                victim->extra = counter;
                fault = 1;
        }
        else if(framep->page == -1)
        { // Can use free page table index
                framep->page = last_page_ref;
                time(&framep->time);
                framep->extra = counter;
                fault = 1;
        }
        else if(framep->page == last_page_ref)
        { // The page was found! Hit!
                time(&framep->time);
                framep->extra = counter;
        }
        if(fault == 1) data->misses++; else data->hits++;
        return fault;
}

// Function to print results after algo is run
int print_help(const char *binary)
{
        printf( "usage: %s algorithm num_frames show_process debug\n", binary);
        printf( "   algorithm    - page algorithm to use {LRU}\n");
        printf( "   num_frames   - number of page frames {int > 0}\n");
        printf( "   show_process - print page table after each ref is processed {1 or 0}\n");
        printf( "   debug        - verbose debugging output {1 or 0}\n");
        return 0;
}


// Function to print results after algo is run
int print_stats(Algorithm algo)
{
        print_summary(algo);
        print_list(algo.data->page_table.lh_first, "Frame #", "Page Ref");
        return 0;
}


// Function to print summary report of an Algorithm
int print_summary(Algorithm algo)
{
        printf("%s Algorithm\n", algo.label);
        printf("Frames in Mem: %d, ", num_frames);
        printf("Hits: %d, ", algo.data->hits);
        printf("Misses: %d, ", algo.data->misses);
        printf("Hit Ratio: %f\n", (double)algo.data->hits/(double)(algo.data->hits+algo.data->misses));
        return 0;
}


// Print list
int print_list(struct Frame *head, const char* index_label, const char* value_label)
{
        int colsize = 9, labelsize;
        struct Frame *framep;
        // Determine lanbel col size from text
        if (strlen(value_label) > strlen(index_label))
                labelsize = strlen(value_label) + 1;
        else
                labelsize = strlen(index_label) + 1;
        /* Forward traversal. */
        printf("%-*s: ", labelsize, index_label);
        for (framep = head; framep != NULL; framep = framep->frames.le_next)
        {
                printf("%*d", colsize, framep->index);
        }
        printf("\n%-*s: ", labelsize, value_label);
        for (framep = head; framep != NULL; framep = framep->frames.le_next)
        {
                if(framep->page == -1)
                        printf("%*s", colsize, "_");
                else
                        printf("%*d", colsize, framep->page);
        }
        printf("\n%-*s: ", labelsize, "Extra");
        for (framep = head; framep != NULL; framep = framep->frames.le_next)
        {
                printf("%*d", colsize, framep->extra);
        }
        printf("\n%-*s: ", labelsize, "Time");
        for (framep = head; framep != NULL; framep = framep->frames.le_next)
        {
                printf("%*d ", colsize, (int32_t) (framep->time%200000000));
        }
        printf("\n\n");
        return 0;
}


// Clean up memory
int cleanup()
{
        size_t i = 0;
        for (i = 0; i < num_algos; i++)
        {
                /* Clean up memory, delete the list */
                while (algos[i].data->page_table.lh_first != NULL)
                {
                        LIST_REMOVE(algos[i].data->page_table.lh_first, frames);
                }
                while (algos[i].data->victim_list.lh_first != NULL)
                {
                        LIST_REMOVE(algos[i].data->victim_list.lh_first, frames);
                }
        }
        return 0;
}
