/* 
    Beyza Nur KAYA 150120077
    This program aims to calculate the multiplication of big numbers that cannot be showed in computer as an integer type.
*/

#include <stdio.h>
#include <stdlib.h>

// creating a node for doubly linked list - it has a number, next and previous pointers
typedef struct NumberNode
{
    int number;
    struct NumberNode *next;
    struct NumberNode *prev;

} NumberNode;

typedef NumberNode *NumberNodePtr;

// this inserts the numbers to the list while head is constant and tail is changing
void insert(NumberNodePtr *head, NumberNodePtr *tail, int number)
{
    // allocating memory
    NumberNodePtr newPtr = malloc(sizeof(NumberNode));

    if (newPtr != NULL)
    {
        // assigning values
        newPtr->number = number;
        newPtr->next = NULL;
        newPtr->prev = NULL;

        // if it is empty then both head and tail looks to the first node
        if ((*tail == NULL) && (*head == NULL))
        {
            *head = newPtr;
            *tail = newPtr;
        }
        // if it is not empty we should make the new node's previous to the tail, and tail's next new node 
        // at the end tail should point to new node
        else
        {
            newPtr->prev = *tail;
            (*tail)->next = newPtr;
            *tail = newPtr;
        }
    }
    else
    {
        printf("Not enough memory in insertion!");
    }
}

// This function is similar to insert but tail is constant and head is changing
void insertFromTail(NumberNodePtr *head, NumberNodePtr *tail, int number)
{
    // allocating memory
    NumberNodePtr newPtr = malloc(sizeof(NumberNode));

    if (newPtr != NULL)
    {
        // assigning values
        newPtr->number = number;
        newPtr->next = NULL;
        newPtr->prev = NULL;
        // if it is empty then both head and tail looks to the first node
        if ((*tail == NULL) && (*head == NULL))
        {
            *head = newPtr;
            *tail = newPtr;
        }
        // if it is not empty we should make the new node's next to the head, and head's prev new node 
        // at the end head should point to new node        
        else
        {
            newPtr->next = *head;
            (*head)->prev = newPtr;
            *head = newPtr;
        }
    }
    else
    {
        printf("Not enough memory in insertion!");
    }
}

// prints list using loop from the head to tail
void printList(NumberNodePtr head, NumberNodePtr tail)
{
    while (head != NULL)
    {
        printf("%d", head->number);
        head = head->next;
    }
    printf("\n");
}

// prints from the tail to head but does not show leading redundant 0's
void printListRev(NumberNodePtr head, NumberNodePtr tail)
{
    int count = 0;
    while (tail != NULL)
    {
        // if it is the first number after the leading zeros
        if (tail->number != 0 & count != 1)
        {
            count++;
        }
        // if we got rid of the redundant zeros
        if (count == 1)
            printf("%d", tail->number);
        tail = tail->prev;
    }
    printf("\n");
}

// this function multiplies a LL with 1 element and a LL with n elements
// I created this to reduce the time, it just simply multiplies with not much checks and statments
// I will use this to find the exponents of the base's like 1,6,6*6,6*36,6*216,...
void baseMultiply(NumberNodePtr *headOfMLPR, NumberNodePtr *tailOfMLPR, NumberNodePtr *headOfMCND, NumberNodePtr *tailOfMCND,
                  NumberNodePtr *headOfResult, NumberNodePtr *tailOfResult, int base)
{
    // making sure the result is NULL at first
    *headOfResult = NULL;
    *tailOfResult = NULL;
    // making pointers that point to the last digits of multiplier and multiplicand, that will be multiplied later
    NumberNodePtr currentMCND = *tailOfMCND;
    NumberNodePtr currentMLPR = *tailOfMLPR;
    int carry = 0;
    // if there's multiplier digit left
    while (currentMLPR != NULL)
    {
        // if there's multiplicand digit left
        while (currentMCND != NULL)
        {
            // the total will be the sum of these two digits multiplication and carry
            int total = currentMCND->number * currentMLPR->number + carry;
            // its modulo base is digit that will be added
            int digit = total % base;
            // its carry is the integer division between them
            carry = total / base;
            // then I insert the digit from the tail to get the result normal not reversed
            insertFromTail(headOfResult, tailOfResult, digit);
            // if the this multiplicand is the last and carry is not zero
            if (currentMCND->prev == NULL && carry != 0)
            // it inserts the carry to the result
                insertFromTail(headOfResult, tailOfResult, carry);
            // now shift multiplicand pointer to the left
            currentMCND = currentMCND->prev;
        }
        // clear the carry
        carry = 0;
        // shift the multiplier pointer to the left
        currentMLPR = currentMLPR->prev;
        // make multiplicand pointer point to same again
        currentMCND = *tailOfMCND;
    }
}

//this function does multiply 2 linked lists and return the result by reference
void multiplyLists(NumberNodePtr *headOfMLPR, NumberNodePtr *tailOfMLPR, NumberNodePtr *headOfMCND, NumberNodePtr *tailOfMCND,
                   NumberNodePtr *headOfResult, NumberNodePtr *tailOfResult, int base)
{
    // to not get a NULL error we fill the result with zero for the first
    insert(headOfResult, tailOfResult, 0);
    // then create 4 pointer, 2 of them are for result, 1 of them is for the multiplicand's tail, the other is for the multiplier's tail
    NumberNodePtr currentMCND = *tailOfMCND;
    NumberNodePtr currentMLPR = *tailOfMLPR;
    NumberNodePtr currentResultHead = *headOfResult;
    NumberNodePtr currentResultTail = *tailOfResult;

    int carry = 0;
    // while there is a multiplier to be multiplied
    while (currentMLPR != NULL)
    {
        // then create 2 pointer that points the position of the basic operation 
        NumberNodePtr currentResultHeadForLoop = currentResultHead;
        NumberNodePtr currentResultTailForLoop = currentResultTail;
        // while there is a multiplicand to be multiplied 
        while (currentMCND != NULL)
        {
            // calculate the total by multiplying the multiplier digit with multiplicand digit plus the carries we put before
            int total = (currentMLPR->number) * (currentMCND->number) + (currentResultHeadForLoop)->number;
            // its modulo base is digit that will be added
            int digit = total % base;
            // its carry is the integer division between them
            carry = total / base;
            // making the digit put into its position in the result
            currentResultHeadForLoop->number = digit;

            // not to get a NULL error we fill it with 0
            if (currentResultHeadForLoop->next == NULL)
            {
                insert(headOfResult, tailOfResult, 0);
            }
            // then put the carry to the next number
            currentResultHeadForLoop->next->number = currentResultHeadForLoop->next->number + carry;
            // and change the result's position
            currentResultHeadForLoop = currentResultHeadForLoop->next;
            // shift multiplicand pointer to the left
            currentMCND = currentMCND->prev;
        }
        // then since the multiplication, we shift our result after doing 1 multipler digit * multiplican
        // likewise we shift the result too
        currentResultHead = currentResultHead->next;
        // making carry clear again
        carry = 0;
        // shift the multiplier pointer to the left
        currentMLPR = currentMLPR->prev;
        // make multiplicand pointer point to same again
        currentMCND = *tailOfMCND;
    }
}
// this function adds two lists into a one list
void addTwoLists(NumberNodePtr *headOfAddend1, NumberNodePtr *tailOfAddend1, NumberNodePtr *headOfAddend2, NumberNodePtr *tailOfAddend2, NumberNodePtr *headOfResult, NumberNodePtr *tailOfResult, int base)
{
    // making sure there is nothing in result
    *headOfResult = NULL;
    *tailOfResult = NULL;
    // making 2 pointers that points to the head of the two that will be added, since they are from a multiplication their head is actually their tail
    NumberNodePtr currentPos = *headOfAddend1;
    NumberNodePtr currentPos2 = *headOfAddend2;
    // making a sum and a carry
    int sum;
    int carry = 0;
    // while there is a digit in the addend1
    while (currentPos != NULL)
    {
        // if there is a digit in the addend2
        if (currentPos2 != NULL)
        {
            // then add them with the carry
            sum = currentPos->number + currentPos2->number + carry;
        }
        // if there is not a digit in the addend
        else
        {
            // then it is the addend1 with carry
            sum = currentPos->number + carry;
        }
        // the the digit which would be in result will be the sum's modulo base
        int digit = sum % base;
        // carry will be their integer division
        carry = sum / base;
        // then i put the digit i've found to the result list
        insert(headOfResult, tailOfResult, digit);
        // if addend1 is ending
        if (currentPos->next == NULL && carry != 0)
        {
            // then insert the carry too
            insert(headOfResult, tailOfResult, carry);
        }
        // make them point to the new number so that they can loop
        currentPos = currentPos->next;
        // careful about the addend2 if it is null
        if (currentPos2 != NULL)
            currentPos2 = currentPos2->next;
    }
}

// this function converts the numbers from their base to decimal, instead of calling
// this function 3 times and calculating the exponents again and again
// i convert them all together
void baseConversions(NumberNodePtr *headOfMLPR, NumberNodePtr *tailOfMLPR, NumberNodePtr *headOfMCND, NumberNodePtr *tailOfMCND,
                   NumberNodePtr *headOfResult, NumberNodePtr *tailOfResult, int base)
{
    // making some pointers so that we can use them later to find base's exponents
    NumberNodePtr basePowHead = NULL;
    NumberNodePtr basePowTail = NULL;
    NumberNodePtr basePowHeadTotal = NULL;
    NumberNodePtr basePowTailTotal = NULL;
    NumberNodePtr basePowHeadResult = NULL;
    NumberNodePtr basePowTailResult = NULL;

    // making some pointers so that we can use them later to sum up two lists
    NumberNodePtr resAddedTail1 = NULL;
    NumberNodePtr resAddedHead1 = NULL;
    NumberNodePtr resTotalTail1 = NULL;
    NumberNodePtr resTotalHead1 = NULL;

    // making some pointers so that we can use them later to sum up two lists
    NumberNodePtr resAddedTail2 = NULL;
    NumberNodePtr resAddedHead2 = NULL;
    NumberNodePtr resTotalTail2 = NULL;
    NumberNodePtr resTotalHead2 = NULL;

    // making some pointers so that we can use them later to sum up two lists
    NumberNodePtr resAddedTail3 = NULL;
    NumberNodePtr resAddedHead3 = NULL;
    NumberNodePtr resTotalTail3 = NULL;
    NumberNodePtr resTotalHead3 = NULL;

    // making some pointers for some cases for base's exponent
    NumberNodePtr baseZeroHead = NULL;
    NumberNodePtr baseZeroTail = NULL;
    NumberNodePtr baseOneHead = NULL;
    NumberNodePtr baseOneTail = NULL;

    // by doing these two we say these two LL will have base inside
    insert(&basePowHead, &basePowTail, base);
    insert(&basePowHeadTotal, &basePowTailTotal, base);
    // this has the base to the power 0 case = 1
    insert(&baseZeroHead, &baseZeroTail, 1);
    // this has the base to the power 1 case = base
    insert(&baseOneHead, &baseOneTail, base);
    // counting the exponent so that we can use the special cases
    int count = 0;

    // while the given LLs is not all null
    while ( (*headOfMCND) != NULL || (*headOfMLPR) != NULL || (*headOfResult) != NULL)
    {
        // then we find the base^0 = 1 and assign it to the total base variable
        if (count == 0)
        {
            basePowHeadTotal = baseZeroHead;
            basePowTailTotal = baseZeroTail;
        }
        // then we find the base^1 = base and assign it to the total base variable
        else if (count == 1)
        {
            basePowHeadTotal = baseOneHead;
            basePowTailTotal = baseOneTail;
        }
        // then we find the base^n and assign its results to the total base variable
        else
        {
            baseMultiply(&basePowHead, &basePowTail, &basePowHeadTotal, &basePowTailTotal, &basePowHeadResult, &basePowTailResult, 10);
            basePowHeadTotal = basePowHeadResult;
            basePowTailTotal = basePowTailResult;
        }
        // if the 1st LL is not null
        if (*headOfMCND != NULL)
        {
            // create 2 pointer to get the digit
            NumberNodePtr tempTail = NULL;
            NumberNodePtr tempHead = NULL;
            // if it is MCND or MLPR - we should start with its tail
            insert(&tempHead, &tempTail, (*tailOfMCND)->number);
            // making a temp variable to hold the digit*base^n
            NumberNodePtr resTail = NULL;
            NumberNodePtr resHead = NULL;
            // finding the digit*base^n
            multiplyLists(&tempHead, &tempTail, &basePowHeadTotal, &basePowTailTotal, &resHead, &resTail, 10);
            // adding the digit*base^n to the old results
            addTwoLists(&resHead, &resTail, &resTotalHead1, &resTotalTail1, &resAddedHead1, &resAddedTail1, 10);
            // making total result have the total so far
            resTotalHead1 = resAddedHead1;
            resTotalTail1 = resAddedTail1;
            // making these two change their position
            *headOfMCND = (*headOfMCND)->next;
            *tailOfMCND = (*tailOfMCND)->prev;
            // then if it is for base^0 (count = 0) then make count 1, base^1(count = 1) make count 2 - which will be base^n
            if (count < 2)
                count++;
        }
        // if the 2nd LL is not null
        if (*headOfMLPR != NULL)
        {
            // create 2 pointer to get the digit
            NumberNodePtr tempTail = NULL;
            NumberNodePtr tempHead = NULL;
            // if it is MCND or MLPR - we should start with its tail
            insert(&tempHead, &tempTail, (*tailOfMLPR)->number);
            // making a temp variable to hold the digit*base^n
            NumberNodePtr resTail = NULL;
            NumberNodePtr resHead = NULL;
            // finding the digit*base^n
            multiplyLists(&tempHead, &tempTail, &basePowHeadTotal, &basePowTailTotal, &resHead, &resTail, 10);
            // adding the digit*base^n to the old results
            addTwoLists(&resHead, &resTail, &resTotalHead2, &resTotalTail2, &resAddedHead2, &resAddedTail2, 10);
            // making total result have the total so far
            resTotalHead2 = resAddedHead2;
            resTotalTail2 = resAddedTail2;
            // making these two change their position
            *headOfMLPR = (*headOfMLPR)->next; 
            *tailOfMLPR = (*tailOfMLPR)->prev;
            // then if it is for base^0 (count = 0) then make count 1, base^1(count = 1) make count 2 - which will be base^n
            if (count < 2)
                count++;
        }
        // if the 3rd LL is not null
        if (*headOfResult != NULL)
        {
            // create 2 pointer to get the digit
            NumberNodePtr tempTail = NULL;
            NumberNodePtr tempHead = NULL;
            // if it is result - we should start with its head since it is reversed it is actually its tail
            insert(&tempHead, &tempTail, (*headOfResult)->number);
            // making a temp variable to hold the digit*base^n
            NumberNodePtr resTail = NULL;
            NumberNodePtr resHead = NULL;
            // finding the digit*base^n
            multiplyLists(&tempHead, &tempTail, &basePowHeadTotal, &basePowTailTotal, &resHead, &resTail, 10);
            // adding the digit*base^n to the old results
            addTwoLists(&resHead, &resTail, &resTotalHead3, &resTotalTail3, &resAddedHead3, &resAddedTail3, 10);
            // making total result have the total so far
            resTotalHead3 = resAddedHead3;
            resTotalTail3 = resAddedTail3;
            // making these two change their position
            *headOfResult = (*headOfResult)->next;
            *tailOfResult = (*tailOfResult)->prev;
            // then if it is for base^0 (count = 0) then make count 1, base^1(count = 1) make count 2 - which will be base^n
            if (count < 2)
                count++;
        }

    }
        printListRev(resTotalHead1, resTotalTail1);
        printListRev(resTotalHead2, resTotalTail2);
        printListRev(resTotalHead3, resTotalTail3);
}

// main function takes arguments
int main(int argc, char *argv[])
{
    // creating file 
    FILE *text;
    //checking the argc and reading the file or returning the program according to that
    if (argc == 2)
    {
        text = fopen(argv[1], "r");
    }
    else if (argc < 2)
    {
        printf("You entered less arguments than expected, try to write one txt file while running!");
        printf("%d", argc);
        return -1;
    }
    else
    {
        printf("You entered too much arguments, try to write one txt file while running!");
        printf("%d", argc);
        return -1;
    }
    // creating LLs ptrs for MCND MLPR and Result
    NumberNodePtr headOfMultiplicand = NULL;
    NumberNodePtr tailOfMultiplicand = NULL;
    NumberNodePtr headOfMultiplier = NULL;
    NumberNodePtr tailOfMultiplier = NULL;
    NumberNodePtr headOfResult = NULL;
    NumberNodePtr tailOfResult = NULL;
    // initially base is 0
    int base = 0;
    int count = 0;
    int baseDigitNumber = 0;

    // creating char 
    char numberAsChar;
    char prevChar;
    // till it is eof
    while ((numberAsChar = getc(text)) != EOF)
    {
        // if it is not a new line and the first line (count 0)
        if ((numberAsChar != '\n') && (count == 0))
        {
            //add them all to the MCND
            insert(&headOfMultiplicand, &tailOfMultiplicand, (int)(numberAsChar - 48));
        }
        // if it is not a new line and the second line (count 1)
        else if ((numberAsChar != '\n') && (count == 1))
        {
            insert(&headOfMultiplier, &tailOfMultiplier, (int)(numberAsChar - 48));
        }
        // if it is not a new line and the third line (count 2)
        else if ((numberAsChar != '\n') && (count == 2))
        {
            // for base 10 case
            if (numberAsChar == '1')
            {
                prevChar = (int)(numberAsChar - 48);
            }
            else if (numberAsChar == '0' && prevChar == 1)
            {
                base = 10;
            }
            // for base n case
            else
            {
                if(baseDigitNumber == 0)
                {
                    base = (int)(numberAsChar - 48);
                    baseDigitNumber++;
                }
                else
                    base = 0;
            }
        }
        // when it is a new line increment count
        else
        {
            count++;
        }
    }
    //print the MCND and MLPR
    printList(headOfMultiplicand, tailOfMultiplicand);
    printList(headOfMultiplier, tailOfMultiplier);
    //check the base if they can be multiplied or not, if it cannot be multiplied return the program w/ a message
    if(base < 11 && base > 1)
    multiplyLists(&headOfMultiplier, &tailOfMultiplier, &headOfMultiplicand, &tailOfMultiplicand, &headOfResult, &tailOfResult, base);
    else
    {
        printf("The base should be between 2 and 10, so the multiplication cannot be executed!\n");
        return -1;
    }
    //print the result
    printListRev(headOfResult, tailOfResult);
    // if it is not base 10 then convert them 
    if (base != 10)
    {
        baseConversions(&headOfMultiplier, &tailOfMultiplier, &headOfMultiplicand, &tailOfMultiplicand, &headOfResult, &tailOfResult, base);
    }
    // if it is base 10, then for output rules print them again with no conversion
    else if(base == 10)
    {
        printList(headOfMultiplicand, tailOfMultiplicand);
        printList(headOfMultiplier, tailOfMultiplier);
        printListRev(headOfResult, tailOfResult);
    }
    //close the text
    fclose(text);
}