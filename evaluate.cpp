#include <string>
#include <iostream>
#include <vector>

using namespace std;

bool is_numeric(string s);
bool convert_to_postfix(string expression, vector<string>& post_fix);
bool pop_and_return(vector<string>& post_fix, int& result);
bool evaluate(const char* expression, int& result);

/* A quick helper function, which only inspects first one or two chars to determine if string is numeric. We know from convert_to_postfix that our numeric string will contain consecutive ints.*/
bool is_numeric(string s)
{
    char c;
    if (s.size() > 0)
    {
        for (int i = 0; i < s.size(); i++)
        {
            c = s.at(i);
            if (c == '-' && s.size() >= 2) //Exceptio for negative nos ex "-45", if - is first char, "-.." then string must be of length 2 atleast
                continue;
            if (!(c >= 48 && c <= 57))
                return false;
        }
        return true;
    }
    else
        return false;
}

/* A recursive function, which help help to pop items out of post_fix vector*/
bool pop_and_return(vector<string>& post_fix, int& result)
{
    int op1, op2;

    if (post_fix.size() == 0)
        return false;

    string pop = post_fix.back();
    post_fix.pop_back();

    if (is_numeric(pop))
    {
        try {
            result = stoi(pop); //
        }
        catch (const std::invalid_argument& e) {
            return false;
        }
        return true;
    }
    else
    {
        if (pop == "+")
        {
            if (!pop_and_return(post_fix, op1) || !pop_and_return(post_fix, op2))
                return false;
            result = op2 + op1;
            return true;
        }
        else if (pop == "-")
        {
            if (!pop_and_return(post_fix, op1) || !pop_and_return(post_fix, op2))
                return false;
            result = op2 - op1;
            return true;
        }
        if (pop == "*")
        {
            if (!pop_and_return(post_fix, op1) || !pop_and_return(post_fix, op2))
                return false;
            result = op2 * op1;
            return true;
        }
        if (pop == "/")
        {
            if (!pop_and_return(post_fix, op1) || !pop_and_return(post_fix, op2))
                return false;
            if (op1 == 0) //Division by 0 exception
                return false;
            result = op2 / op1;
            return true;
        }
        else
        {
            return false;
        }
    }
}

/*
    Convert into post-fix expression which meets our requirements
    Format : Requirements -> action taken to meet those
    * Requirements - 1. Order of evaluation left to right -> We parse string left to right
    *                2. All operator are equal priority -> When an operator char is tried to push onto operator_stack, if an operator is present, pop it first. This ensures left to right associativity without any priority to * or / for example.
    *                3. () Nested evaluated first -> When an enclosing parenthsis is encountered, pop all from operator_stack onto operand_stack until opening parenthesis is found.
    */
bool convert_to_postfix(string expression, vector<string>& post_fix)
{
    vector<string> operator_stack;
    vector<string> operand_stack;
    string temp_operator, temp_string;
    char c;
    temp_string = "";

    for (int i = 0; i < expression.size(); i++)
    {
        c = expression.at(i);
        if (c >= 48 && c <= 57) //ASCI '0' -> 48 to '9' -> 57
        {
            temp_string += string(1, c);
            continue;
        }

        if (temp_string != "")
        {
            operand_stack.push_back(temp_string);
            temp_string = "";
        }

        if (c == ' ')
            continue;

        if (c == '+' || c == '-' || c == '/' || c == '*')
        {
            /*Handling of negative nos*/
            if (c == '-' && (i + 1 < expression.size()) && (expression.at(i + 1) >= 48 && expression.at(i + 1) <= 57))
            {
                // It is a negative number
                temp_string += string(1, c);
                if (operand_stack.size() == 0)
                    continue;
                c = '+';
            }
            else if (operator_stack.size() > 0 && (operator_stack.back() == "+" || operator_stack.back() == "-" || operator_stack.back() == "/" || operator_stack.back() == "*"))
            {
                temp_operator = operator_stack.back();
                operator_stack.pop_back();
                operand_stack.push_back(temp_operator);
            }
            operator_stack.push_back(string(1, c));

        }
        else if (c == '(')
        {
            operator_stack.push_back(string(1, c));
        }
        else if (c == ')')
        {
            if (operator_stack.size() == 0)
                return false;
            temp_operator = operator_stack.back();
            operator_stack.pop_back();
            while (temp_operator != "(")
            {
                operand_stack.push_back(temp_operator);
                if (operator_stack.size() == 0)
                    return false;
                temp_operator = operator_stack.back();
                operator_stack.pop_back();
            }
        }
        else {
            return false;
        }
    }

    if (temp_string != "")
    {
        operand_stack.push_back(temp_string);
        temp_string = "";
    }

    while (operator_stack.size() != 0)
    {
        operand_stack.push_back(operator_stack.back());
        operator_stack.pop_back();
    }

    post_fix = operand_stack;
    return true;
}

bool evaluate(const char* expression, int& result)
{
    vector<string> post_fix;

    if (!convert_to_postfix(string(expression), post_fix))
        return false;

    /* ------Debug------
    cout << "post-fix ";
    for (int i = 0; i < post_fix.size(); i++)
    {
        cout << post_fix[i] << " -> ";
    }
    */

    return pop_and_return(post_fix, result);
}


int main()
{

    string test_cases[] = { "45a",
        "-4 + ((20 / 4) * 2) / 3",
        "5+4)",
        "-1-2-3-4-5",
        " - 1",
        "-1",
        " - 4 - 5",
        "-4,-5"
        "1 + 3",
        "(1 + 3) * 2",
        "(1 + (12 * 2)",
        "5/0" };
    int result = 0;
    cout << "Program started..." << endl << endl;

    for (int i = 0; i < 11; i++)
    {
        cout << "Test " << i + 1 << endl << endl;
        cout << "INPUT - " << test_cases[i] << endl;
        if (evaluate(test_cases[i].c_str(), result))
            cout << "OUTPUT - " << result << endl << endl;
        else
            cout << "OUTPUT - " << "N/A" << endl << endl;
    }
    return 0;
}