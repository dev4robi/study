////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 1) 종류 : 자료구조/스택
// 2) 문제 : 괄호('()[]{}')의 짝이 올바르게 맞는지 "YES", "NO"로 결과를 반환해주는 코드 작성.
// 3) URL  : https://www.hackerrank.com/challenges/balanced-brackets/problem
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stack>
#include <list>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Complete the isBalanced function below.
bool isClosedBrackets(char bracket) {
    switch (bracket) {
        case ')': case ']': case '}': return true;
    }
    
    return false;
}

char getMatchedBrackets(char rightBracket) {
    switch (rightBracket) {
        case ')': return '(';
        case ']': return '[';
        case '}': return '{';
    }

    return '?';
}

string isBalanced(string s) {
    const string::size_type szStr = s.length();

    if ( szStr % 2 != 0 ) {        
        return string("NO");
    }

    // stack's default ctor container is 'deque' but, deque has timeout problem because of 'realloc'.
    // So, I choose the container that 'list' to solve this timeout problem.
    stack<char, list<char>> stk;

    for ( string::iterator iter = s.begin(); iter != s.end(); ++iter ) {
        char bracket = *iter;

        if ( !isClosedBrackets(bracket) ) {
            stk.push(bracket);
        }
        else {
            char openedBracket = getMatchedBrackets(bracket);

            if ( stk.top() != openedBracket ) {
                return string("NO");
            }

            stk.pop();
        }
    }

    if ( !stk.empty() ) {
        return string("NO");
    }

    return string("YES");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    int t;
    cin >> t;
    cin.ignore(1, '\n');

    for (int t_itr = 0; t_itr < t; t_itr++) {
        string s;
        getline(cin, s);

        string result = isBalanced(s);

        cout << result << "\n";
    }

    return 0;
}
