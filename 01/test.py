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
check('', 'error', 5)
check('1 --', 'error', 6)
check('1 + -', 'error', 7)
check('12 2', 'error', 8)
check('42342342342342342344534', 'error', 9)
check('1 + -23423423423424234324', 'error', 10)
check('--1', '1', 11)
check('---1', '-1', 12)
check('1 + -1', '0', 13)
check('1 + --1', '2', 14)
check('1 - ---1', '2', 15)
check('1 *4', '4', 16)
check('1/0', 'error', 17)
check('1 + 5*3/0', 'error', 18)
check('2', '2', 19)
check('-2', '-2', 20)
check('2 + 2', '4', 21)
check('2 + 2   ', '4', 22)
check('2 +- 2', '0', 23)
check('   2+-4', '-2', 24)
check('-    4- -4', '0', 25)
check('2-3*4+-5/2', '-12', 26)
check('2-3*4*2+1--2+-5/2', '-21', 27)
check('', 'error', 28)
check('2/0', 'error', 29)
check('2/', 'error', 30)
check('3 + a', 'error', 31)