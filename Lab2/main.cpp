#include <iostream>
#include "Six.h"

void testBasicOperations() {
    std::cout << "=== Testing basic operations ===" << std::endl;

    Six num1("12345");
    Six num2(5, 3);
    Six num3;
    
    std::cout << "num1: " << num1.toString() << std::endl;
    std::cout << "num2: " << num2.toString() << std::endl;
    std::cout << "num3: " << num3.toString() << std::endl;
    
    Six num4 = num1.copy();
    std::cout << "Copy num1: " << num4.toString() << std::endl;
}

void testArithmetic() {
    std::cout << "\n=== Arithmetic testing ===" << std::endl;
    
    Six a("12345");
    Six b("54321");
    
    std::cout << "a: " << a.toString() << std::endl;
    std::cout << "b: " << b.toString() << std::endl;
    
    Six sum = a.add(b);
    std::cout << "a + b = " << sum.toString() << " (the sixfold)" << std::endl;

    Six diff = b.subtract(a);
    std::cout << "b - a = " << diff.toString() << " (the sixfold)" << std::endl;

    Six zero;
    Six sumWithZero = a.add(zero);
    std::cout << "a + 0 = " << sumWithZero.toString() << std::endl;
}

void testComparison() {
    std::cout << "\n=== Comparison testing ===" << std::endl;
    
    Six x("12345");
    Six y("54321");
    Six z("12345");
    
    std::cout << "x: " << x.toString() << std::endl;
    std::cout << "y: " << y.toString() << std::endl;
    std::cout << "z: " << z.toString() << std::endl;
    
    std::cout << "x equals y: " << (x.equals(y) ? "true" : "false") << std::endl;
    std::cout << "x equals z: " << (x.equals(z) ? "true" : "false") << std::endl;
    std::cout << "x greater than y: " << (x.greaterThan(y) ? "true" : "false") << std::endl;
    std::cout << "x less than y: " << (x.lessThan(y) ? "true" : "false") << std::endl;
    std::cout << "y greater than x: " << (y.greaterThan(x) ? "true" : "false") << std::endl;
}

void testEdgeCases() {
    std::cout << "\n=== Testing boundary cases ===" << std::endl;
    
    try {
        Six zero;
        std::cout << "Zero: " << zero.toString() << std::endl;

        Six withZeros("000123");
        std::cout << "The number '000123' after removing the zeros: " << withZeros.toString() << std::endl;
        
        Six maxDigit("55555");
        std::cout << "A number with maximum digits: " << maxDigit.toString() << std::endl;
        
        Six singleDigit("3");
        std::cout << "Single digit number: " << singleDigit.toString() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void testErrorHandling() {
    std::cout << "\n=== Error handling testing ===" << std::endl;

    try {
        Six invalid1("123456");  
        std::cout << "ERROR: There should have been an exception!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "The error was caught correctly: " << e.what() << std::endl;
    }

    try {
        Six invalid2(""); 
        std::cout << "ERROR: There should have been an exception!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "The error was caught correctly: " << e.what() << std::endl;
    }

    try {
        Six small("10");
        Six large("20");
        Six result = small.subtract(large); 
        std::cout << "ERROR: There should have been an exception!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "The error was caught correctly: " << e.what() << std::endl;
    }

    try {
        Six invalid3(0, 0); 
        std::cout << "ERROR: There should have been an exception!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "The error was caught correctly: " << e.what() << std::endl;
    }
}

void testComplexOperations() {
    std::cout << "\n=== Testing complex operations ===" << std::endl;
    
    Six num1("12345");
    Six num2("11111");
    Six num3("22222");
    
    std::cout << "num1: " << num1.toString() << std::endl;
    std::cout << "num2: " << num2.toString() << std::endl;
    std::cout << "num3: " << num3.toString() << std::endl;
    
    Six temp = num1.add(num2);
    Six result = temp.subtract(num3);
    
    std::cout << "(num1 + num2) - num3 = " << result.toString() << std::endl;

    Six original("55555");
    Six modified = original.add(Six("1"));
    
    std::cout << "The original after the operation: " << original.toString() << " (not changed)" << std::endl;
    std::cout << "The result of the operation: " << modified.toString() << std::endl;
}

int main() {
    std::cout << "A program for working with hexadecimal numbers" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    testBasicOperations();
    testArithmetic();
    testComparison();
    testEdgeCases();
    testErrorHandling();
    testComplexOperations();
    
    std::cout << "\nAll tests have been completed successfully!" << std::endl;
    return 0;
}