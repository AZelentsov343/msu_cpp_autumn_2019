#include <iostream>
#include <cerrno>
#include <climits>
#include "calc.h"

using namespace std;


void normalize_str(char *s) {
    char *copy = s;
    bool even = true;
    int count = 0;
    bool deleting = false;
    while (*s) {
        if (*s == '-') {
            deleting = true;
            count++;
            even = !even;
        } else {
            if (deleting) {
                if (!even) {
                    *copy = '-';
                    copy++;
                }
                if (!isdigit(*s) && count > 1) {
                    cout << "There are invalid symbols in input or input is incorrect" << endl;
                    exit(1);
                }
            }
            even = true;
            *copy = *s;
            copy++;
            deleting = false;
            count = 0;
        }
        s++;
    }
    if (deleting) {
        if (!even) {
            *copy = '-';
            copy++;
        }
        if (!isdigit(*s) && count > 1) {
            cout << "There are invalid symbols in input or input is incorrect" << endl;
            exit(1);
        }
    }
    *copy = '\0';
}

int prior(int c) {
    if (c == '+' || c == '-') {
        return 1;
    } else if (c == '*' || c == '/') {
        return 2;
    } else {
        return 0;
    }
}

int count(int a, int op, int b) {
    switch (op) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b == 0) {
                cout << "Do not divide by zero!" << endl;
                exit(1);
            }
            return a / b;
        default:
            return 0;
    }
}

int is_aryphmetic(char a) {
    return (a == '+' || a == '-' || a == '*' || a == '/');
}

void rec_delete(struct List *l) {
    if (l) {
        if (l->prev) {
            rec_delete(l->prev);
        }
        delete l;
    }
}

int calc(char *s, struct List *prev) {
    if (prev == nullptr) {
        prev = new List;
    }
    errno = 0;
    int cur_num = strtol(s, &s, 10);
    if (errno == ERANGE || cur_num > INT_MAX || cur_num < INT_MIN) {
        cout << "The absolute value is too great to be of type int" << endl;
        rec_delete(prev);
        exit(1);
    } else if (errno == EINVAL) {
        cout << "There are invalid symbols in input or input is incorrect" << endl;
        rec_delete(prev);
        exit(1);
    }
    while (*s && isspace(*s)) {
        s++;
    }
    if (*s && !is_aryphmetic(*s)) {
        cout << "There are invalid symbols in input or input is incorrect" << endl;
        rec_delete(prev);
        exit(1);
    }
    if (*s == '\0') {
        int res;
        if (prev->prior == 0) {
            rec_delete(prev);
            return cur_num;
        } else if (prev->prior > prev->prev->prior) {
            prev->num = count(prev->num, prev->op, cur_num);
            if (prev->prev->prior != 0) {
                res = count(prev->prev->num, prev->prev->op, prev->num);
            } else {
                res = prev->num;
            }
            rec_delete(prev);
            return res;
        } else if (prev->prev->prior != 0) {
            prev->num = count(prev->prev->num, prev->prev->op, prev->num);
            res = count(prev->num, prev->op, cur_num);
            rec_delete(prev);
            return res;
        }
    }
    int cur_op = *s;
    s++;
    auto l = new List(cur_op, cur_num, prior(cur_op), prev);
    if (prev->prior < l->prior) {
        return calc(s, l);
    } else if (prev->prior >= l->prior) {
        l->num = count(prev->num, prev->op, l->num);
        l->prev = prev->prev;
        delete prev;
        return calc(s, l);
    }
    return 0;
}


