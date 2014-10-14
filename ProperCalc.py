#!/usr/bin/python2
import math

print "Calcbot v2.0 - by the numba1 bros"
print "/////////////////////////////////"

def add(number_1,number_2):
    number_1 = float(number_1)
    number_2 = float(number_2)
    return number_1+number_2


def sub(number_1,number_2):
    number_1 = float(number_1)
    number_2 = float(number_2)
    return number_1-number_2


def div(number_1,number_2):
    number_1 = float(number_1)
    number_2 = float(number_2)
    return number_1/number_2


def multi(number_1,number_2):
    number_1 = float(number_1)
    number_2 = float(number_2)
    return number_1*number_2

keepProgramRunning = True

while keepProgramRunning:
    pop = True
    while pop:
        print "***********************************************************************"
        command = raw_input("Do you want to add, subtract, multiply or divide?: ")
        print "***********************************************************************"
        print "   "
        if command == '+' or command == 'add' or command == '-' or command == 'subtract' or command == '/' or command == 'divide' or command == '*' or command == 'multiply':
            break 
        else:
            print 'Enter a valid command'
    pop_2 = True
    while pop_2:
        print "***********************************************************************"
        number_1 = raw_input("First number? ")
        number_2 = raw_input("Second number? ")
        print "***********************************************************************"
        print "  "
		
		
        if number_1.startswith('-') or number_2.startswith('-'):
            break
	"""	
	number_1 = int(number_1)
	number_2 = int(number_2)
        """
        if number_1.isdigit() == True and number_2.isdigit() == True:
            break
        else:
            print 'Enter valid numbers'

        
    if command == '+' or command == 'add':
        sum = add(number_1, number_2)
        print "Result:"
        print sum
    elif command == '-' or command == 'subtract':
        sum = sub(number_1, number_2)
        print "Result:"
        print sum
    elif command == '/' or command == 'divide':
        sum = div(number_1, number_2)
        print "Result:"
        print sum
    elif command == '*' or command == 'multiply':
        sum = multi(number_1, number_2)
        print "Result:"
        print sum
    else:
        print "Please enter a valid command or fuck off ya pretty cunt"

    
    try:
        logfile = open("result.txt", "w")
        try:
            sum = str(sum)
            logfile.write(sum)
        finally:
            logfile.close()
    except IOError:
        pass
    
