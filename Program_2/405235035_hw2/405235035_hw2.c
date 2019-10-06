#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STRING_SIZE 128

typedef enum
{
    lparen,     // 0
    rparen,     // 1
    positive,   // 2
    negative,   // 3
    plus,       // 4
    minus,      // 5
    times,      // 6
    divide,     // 7
    eos,        // 8
    operand     // 9
} precedence;

//  means "lparen,rparen,positive,negative,plus,minus,times,divide,eos,operand"
int isp[] = {0, 19, 15, 15, 12, 12, 13, 13, 0, 0};
int icp[] = {20, 19, 15, 15, 12, 12, 13, 13, 0, 0};

// Store the infix into a postfix node
struct node
{
    precedence operator;
    struct node *next;
};
typedef struct node Node;

//The node that stores the postfix calculation process
struct eval_node
{
    double number;
    struct eval_node *next;
};
typedef struct eval_node eval_Node;

//Head of two linked lists
Node *head = NULL;
eval_Node *eval_head = NULL;

//Store the infix string
char expr[MAX_STRING_SIZE] = {0};

char symbol;        //Store the operator read from the infix string
precedence token;   //Store the value of the operator's enum
int n = 0;          //Infix string index
char temp[10];      //Store double digits or more

//Store the postfix string
char equation[MAX_STRING_SIZE] = {0};

//a function that reads a value or operator from an infix
precedence getToken(void)
{
    symbol = expr[n];
    n++;

    switch (symbol)
    {
        case '(':
            return lparen;
        case ')':
            return rparen;
        case '+':
        {
            //If there is an operator in front of the operator, then this operator is positive
            if (n == 1 || expr[n - 2] == '+' || expr[n - 2] == '-' || expr[n - 2] == '*' || expr[n - 2] == '/' || expr[n - 2] == '(')
                return positive;
            else
                return plus;
        }
        case '-':
        {
            //If there is an operator in front of the operator, then this operator is negative
            if (n == 1 || expr[n - 2] == '+' || expr[n - 2] == '-' || expr[n - 2] == '*' || expr[n - 2] == '/' || expr[n - 2] == '(')
                return negative;
            else
                return minus;
        }
        case '/':
            return divide;
        case '*':
            return times;
        case '\0':
            return eos;
        case '\n':
            return eos;
        default:
        {
            int i = 0;
            temp[i] = symbol;
            i++;
            //Determine if this number is greater than two digits
            while (expr[n] >= '0' && expr[n] <= '9')
            {
                temp[i] = expr[n];
                i++;
                n++;
            }
            temp[i] = '\0';
            return operand;
        }
    }
}

//Stack infix tokens in a linked list
void push(precedence token)
{
    Node *new = (Node *)malloc(sizeof(Node));
    new->operator= token;
    new->next = head;
    head = new;
    return;
}

//Take the infix token from the linked list stack
precedence pop(void)
{
    precedence temp = head->operator;
    Node *current = head;
    head = head->next;
    free(current);
    return temp;
}

//Output the operator stored in enum mode
void printToken(precedence token)
{
    if (token == lparen)
        strcat(equation, "( ");
    else if (token == rparen)
        strcat(equation, ") ");
    else if (token == positive)
        strcat(equation, "p ");
    else if (token == negative)
        strcat(equation, "n ");
    else if (token == plus)
        strcat(equation, "+ ");
    else if (token == minus)
        strcat(equation, "- ");
    else if (token == divide)
        strcat(equation, "/ ");
    else if (token == times)
        strcat(equation, "* ");
    else
        return;

    return;
}

//Convert infix to postfix
void postfix(void)
{
    for (token = getToken(); token != eos; token = getToken())
    {
        if (token == operand)
        {
            strcat(equation, temp);
            strcat(equation, " ");
            //Determine if there is a sign in front of the number
            while (head->operator== positive || head->operator== negative)
                printToken(pop());
        }

        else if (token == rparen)
        {
            while (head->operator!= lparen)
                printToken(pop());
            pop();
        }

        //The sign is placed directly in the stack
        else if (token == positive || token == negative)
            push(token);

        else
        {
            //Determine whether the operator in the stack should be output with the priority value.
            while (isp[head->operator] >= icp[token])
                printToken(pop());
            push(token);
        }
    }

    //After the infix string is completely processed, the operator in the stack is output.
    for (token = pop(); token != eos; token = pop())
        printToken(token);

    return;
}

//Stack postfix tokens in a linked list
void eval_push(double input)
{
    eval_Node *new = (eval_Node *)malloc(sizeof(eval_Node));
    new->number = input;
    new->next = eval_head;
    eval_head = new;
    return;
}

//Take the postfix token from the linked list stack
double eval_pop(void)
{
    double temp = eval_head->number;
    eval_Node *current = eval_head;
    eval_head = eval_head->next;
    free(current);
    return temp;
}

//Calculate the value of postfix
double eval(void)
{
    double op1, op2;
    int i = 0, flag=0;
    double input;

    //Continuous calculation when postfix has not read the end
    while (equation[i] != '\0')
    {
        //Take out numbers greater than two digits and put them on the stack
        if (equation[i] >= '0' && equation[i] <= '9')
        {
            input = (double)atoi(equation + i);
            eval_push(input);
            flag++;
            while (equation[i] != ' ')
                i++;
        }
        //Calculate the values produced by various operators
        else
        {
            op2 = eval_pop();
            if (flag >= 2)
                op1 = eval_pop();

            if (equation[i] == 'n')
            {
                if (flag >= 2)
                    eval_push(op1);
                eval_push(-1 * op2);
            }
            else if (equation[i] == 'p')
            {
                if (flag >= 2)
                    eval_push(op1);
                eval_push(op2);
            }
            else if (equation[i] == '*')
            {
                eval_push(op1 * op2);
                flag--;
            }
            else if (equation[i] == '/')
            {
                eval_push(op1 / op2);
                flag--;
            }
            else if (equation[i] == '+')
            {
                eval_push(op1 + op2);
                flag--;
            }
            else if (equation[i] == '-')
            {
                eval_push(op1 - op2);
                flag--;
            }

            i++;
        }
        i++;
    }

    return (eval_pop());
}

int main()
{
    FILE *fp, *fd;
    fp = fopen("infix.txt", "r+");
    fd = fopen("postfix.txt", "w+");

    while (fgets(expr, 128, fp) != NULL)
    {
        //Give a header to the infix stack
        Node *top = (Node *)malloc(sizeof(Node));
        top->operator= eos;
        top->next = NULL;
        head = top;

        //Reset the string that stores postfix
        strcpy(equation, "\0");
        n = 0;

        postfix();
        equation[strlen(equation) - 1] = '\0';  //Remove the last blank

        printf("%s\t%lf\n", equation, eval());
        fprintf(fd, "%s\t%lf\n", equation, eval());
    }

    fclose(fp);
    fclose(fd);

    return 0;
}