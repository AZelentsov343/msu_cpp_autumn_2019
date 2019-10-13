import subprocess

def check(expr, res, num_of_test):
    out = subprocess.run(['./my_prog', expr], stdout=subprocess.PIPE)
    if out.stdout.decode('ascii') != res + '\n':
        print('error at test {0}, {1} != {2}, got {3}'.format(num_of_test, expr, res, out.stdout))
    else:
        print('Test {0} OK'.format(num_of_test))


check('2 + 2', '4', 1)
check('1 + 2 * 3', '7', 2)
check('1 + 5 * 3 / 2', '8', 3)
check('1', '1', 4)
check('', 'There are invalid symbols in input or input is incorrect', 5)
check('1 --', 'There are invalid symbols in input or input is incorrect', 6)
check('1 + -', 'There are invalid symbols in input or input is incorrect', 7)
check('12 2', 'There are invalid symbols in input or input is incorrect', 8)
check('42342342342342342344534', 'The absolute value is too great to be of type int', 9)
check('1 + -23423423423424234324', 'The absolute value is too great to be of type int', 10)
check('--1', '1', 11)
check('---1', '-1', 12)
check('1 + -1', '0', 13)
check('1 + --1', '2', 14)
check('1 - ---1', '2', 15)
check('1 *+4', '4', 16)
check('1/0', 'Do not divide by zero!', 17)
check('1 + 5*3/0', 'Do not divide by zero!', 18)