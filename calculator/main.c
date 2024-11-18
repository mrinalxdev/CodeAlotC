#include <stdio.h>
#include "calculator.h"

int main(){
    double num1, num2, result;
    char operator;

    printf("Enter first number : ");
    scanf("%lf", &num1);

    printf("Enter an operator (+, -, *, /) : ");
    scanf(" %c", &operator);

    printf("Enter second number : ");
    scanf("%lf", &num2);

    switch (operator){
        case '+':
            result = add(num1, num2);
            break;
        case '-':
            result = subtract(num1, num2);
            break;
        case '*':
            result = multiply(num1, num2);
            break;
        
        case '/':
            if (num2 != 0){
                result = divide(num1, num2);
            } else {
                printf("Error : Division by zero is not allowed. \n");
                return 1;
            }
            break;
        default:
            printf("Error : Invalid operator . \n");
            return 1;
    }

    printf("Result : %.2lf\n", result);
    return 0;
}