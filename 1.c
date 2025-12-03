#include <stdio.h>
double calculateRepayment(double loan, double interestRate, double installment, 
                          double extraPayment, int year, int totalYears)
{

    if (loan <= 0 || year > totalYears) {
        return 0;  
    }
    
    loan = loan + (loan * interestRate);

    double paymentThisYear = installment + extraPayment;
    loan = loan - paymentThisYear;
    
    if (loan < 0) loan = 0;   
    printf("Year %d: Remaining loan = %.2f\n", year, loan);

    return paymentThisYear + 
           calculateRepayment(loan, interestRate, installment, extraPayment, year + 1, totalYears);
}
int main() {
    double loan = 100000;        
    double interestRate = 0.05;  
    double installment = 30000;  
    double extraPayment = 0;    
    int years = 3;
    printf("Loan Repayment Schedule:\n");
    double totalPaid = calculateRepayment(loan, interestRate, installment, extraPayment, 1, years);
    printf("\nTotal repayment over %d years = %.2f\n", years, totalPaid);
    return 0;
}
