#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*********************************************************************
 *
 * These C functions use patterns and functionality often found in
 * operating system code. Your job is to implement them. Write some test
 * cases for each function ( stick them in a funciton called it xyzTests() or similar)
 * call your abcTests(), etc functions in main().
 *
 * Write your own tests for each function you are testing, then share/combine
 * tests with a classmate.  Try to come up with tests that will break people's code!
 * Easy tests don't catch many bugs!
 *
 * You may not use any global variables in your solution
 *
 *
 * You must compile in C mode (not C++).  If you compile from the commandline
 * run clang, not clang++. Submit your solution files on Canvas.
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 * 
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 * Ah, the joys of using bitwise operators!
 *
 * Hint: you may want to write helper functions for these two functions
 *
 *********************************************************************/

//unsigned long comp(const unsigned long a, const unsigned long b){
//    return b-a;
//}

unsigned long byte_sort (unsigned long arg)
{
    unsigned long a=arg>>14*4;
    unsigned long b=arg<<2*4>>14*4;
    unsigned long c=arg<<4*4>>14*4;
    unsigned long d=arg<<6*4>>14*4;
    unsigned long e=arg<<8*4>>14*4;
    unsigned long f=arg<<10*4>>14*4;
    unsigned long g=arg<<12*4>>14*4;
    unsigned long h=arg<<14*4>>14*4;
    
    unsigned long arr[8]={a,b,c,d,e,f,g,h};
//    qsort(arr,8,sizeof(unsigned long),comp);
    
    for(int i=0;i<7;i++){
        int minIndex=i;
        for(int j=i+1;j<8;j++){
            if(arr[j]<arr[minIndex])
                minIndex=j;
        }
        unsigned long temp=arr[i];
        arr[i]=arr[minIndex];
        arr[minIndex]=temp;
    }
    unsigned long output=0;
    for(int i=0;i<8;i++){
        output+= (arr[i]<< 8*i );
    }
  return output;
}

void byte_sort_test() {
    assert( byte_sort (0x0403deadbeef0201) == 0xefdebead04030201 );
    assert( byte_sort (0xffffffffffffffff) == 0xffffffffffffffff );
    assert( byte_sort (0x10b1bed25b0f5667) == 0xd2beb1675b56100f);
    assert( byte_sort (0xf1a5175525edfae6) == 0xfaf1ede6a5552517 );
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 * 
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

unsigned long nibble_sort (unsigned long arg)
{
    unsigned long a=arg>>15*4;
    unsigned long b=arg<<1*4>>15*4;
    unsigned long c=arg<<2*4>>15*4;
    unsigned long d=arg<<3*4>>15*4;
    unsigned long e=arg<<4*4>>15*4;
    unsigned long f=arg<<5*4>>15*4;
    unsigned long g=arg<<6*4>>15*4;
    unsigned long h=arg<<7*4>>15*4;
    unsigned long i=arg<<8*4>>15*4;
    unsigned long j=arg<<9*4>>15*4;
    unsigned long k=arg<<10*4>>15*4;
    unsigned long l=arg<<11*4>>15*4;
    unsigned long m=arg<<12*4>>15*4;
    unsigned long n=arg<<13*4>>15*4;
    unsigned long o=arg<<14*4>>15*4;
    unsigned long p=arg<<15*4>>15*4;
    
    unsigned long arr[16]={a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p};
    
    for(int i=0;i<15;i++){
        int minIndex=i;
        for(int j=i+1;j<16;j++){
            if(arr[j]<arr[minIndex])
                minIndex=j;
        }
        unsigned long temp=arr[i];
        arr[i]=arr[minIndex];
        arr[minIndex]=temp;
    }
    
    unsigned long output=0;
    for(int i=0;i<16;i++){
        output+= (arr[i]<<i*4 );
    }
  return output;
}

void nibble_sort_test() {
    assert( nibble_sort (0x0403deadbeef0201) == 0xfeeeddba43210000 );
    assert( nibble_sort (0xf1a5175525edfae6) == 0xffeedaa765555211 );
    assert( nibble_sort (0x48c5d6b4757ac233) == 0xdccba87765544332 );
    assert( nibble_sort (0x0b32c8efa93cab2e) == 0xfeeccbbaa9833220 );
    assert( nibble_sort (0x95cd4b2a52f236f5) == 0xffdcba9655543222 );
}

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 * Note, that since we're using C, not C++ you can't use new/delete!
 * The analog to delete is the free() function
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *
 *
 * Implement print_list and free_list which should do what you expect.
 * Printing or freeing a nullptr should do nothing
 * Note: free_list might be useful for error handling for name_list... 
 *********************************************************************/

struct elt {
  char val;
  struct elt *link;
};

void free_list(struct elt* head); /*so you can call free_list in name_list if you'd like*/
void print_list(struct elt* head);
struct elt *name_list (void)
{
    struct elt* head=(struct elt*)malloc(sizeof(struct elt));
    struct elt* temp=head;
    char* str="Yuhui";
    for(int i=0;i<4;i++){
        temp->val=str[i];
//        struct elt* next=(struct elt*)malloc(sizeof(struct elt));
//        temp->link=next;
//        temp=next;
        temp->link=(struct elt*)malloc(sizeof(struct elt));
        temp=temp->link;
    }
    
    temp->val = str[4];
    temp->link = NULL;
//    print_list(head);
//    free_list(head);
    
  return head;
}

void print_list(struct elt* head){
    if(head==NULL)
        return;
    for(struct elt* temp=head;temp!=NULL;temp=temp->link){
//        temp=malloc(sizeof(struct elt));
        printf("%c", temp->val);
    }
    printf("\n");
}

void free_list(struct elt* head){
    struct elt* temp;
    while(head!=NULL){
        temp=head;
        head=head->link;
        free(temp);
//        temp=NULL;
    }
//    return;
}


/*********************************************************************
 *
 * draw_me()
 *
 * this function creates a file called me.txt which contains an ASCII-art
 * picture of you (it does not need to be very big).
 * 
 * Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
 * don't use C++ iostreams
 *
 *
 *********************************************************************/

void draw_me (void)
{
    FILE * f=fopen("me.txt","w");
    
    char *p=
        "   *               MMM8&&&   \n"
        "                  MMMM88&&&&&\n"
        "                 MMMM88&&&&&&&\n"
        "     *           MMM88&&&&&&&&\n"
        "                 MMM88&&&&&&&&\n"
        "                  MMM88&&&&&&\n"
        "                    MMM8&&&\n"
        "          |\\___/|\n"
        "         =) ^Y^ (=\n"
        "          \\  ^  /\n"
        "           )=*=(\n"
        "          /     \\ \n"
        "          |     |\n"
        "         /| | | |\\ \n"
        "        /\\| | |_|/\\ \n"
        "  _/\\_//_// ___/\\_/\\_/\\_/\\_/\\_/\\_/\\_\n"
    ;
    
    fprintf(f, "%s", p);
    fclose(f);
}

int main(int argc, const char * argv[]) {
    
    byte_sort_test();
    nibble_sort_test();
    
    struct elt* head=name_list();
    print_list(head);
    free_list(head);
    
    draw_me();
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
