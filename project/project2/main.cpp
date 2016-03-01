#include <vector>
#include <stack>
#include <string>
#include <iostream>
#include <map>
#include <unistd.h>
#include <algorithm>
#include <sys/wait.h>
#include <signal.h>
#include "Node.h"
#include "evaluator.h"

using namespace std;

const int READ = 0;
const int WRITE = 1;

const string MULT = "*";
const string DIV = "/";
const string ADD = "+";
const string SUB = "-";

map<const string, int> precedence;
vector<string> postfix;
stack<gvsu::Node<string>*> results;
vector<int> child_processes;
bool evaluate_immediately;

void sigint_handler(int num) {
    evaluate_immediately = true;
    kill(0, SIGUSR1);
}

void sigusr_one_handler(int num) {
    evaluate_immediately = true;
}

void init_precedence() {
    precedence[MULT] = 1;
    precedence[DIV] = 1;
    precedence[ADD] = 0;
    precedence[SUB] = 0;
}

bool is_operand(const string &token) {
    return token.length() >= 3;
}

bool is_operator(const string &token) {
    return token.size() == 1 && precedence.find(token) != precedence.end();
}

void get_tokens(const string &expr) {
    stack<string> operators;
    string current_token;
    for (unsigned int i = 0; i < expr.size(); ++i) {
        // If the next char is a space or we're at the end of the expression
        if (isspace(expr[i + 1]) || i == expr.size() - 1) {
            current_token += expr[i];
            if (is_operator(current_token)) {
                // If the stack contains higher precedence tokens,
                while (!operators.empty() && precedence.at(operators.top()) >= precedence.at(current_token)) {
                    postfix.push_back(operators.top());
                    operators.pop();
                }
                operators.push(current_token);
            } else {
                postfix.push_back(current_token);
            }
            current_token = "";
        } else if (!isspace(expr[i])) {
            current_token += expr[i];
        }
    }
    // Check for remainding operators
    while(!operators.empty()) {
        postfix.push_back(operators.top());
        operators.pop();
    }
}

float get_operation(const string &op, const float &lhs, const float &rhs) {
    if (op.compare(ADD) == 0) {
        return lhs + rhs;
    }
    if (op.compare(SUB) == 0) {
        return lhs - rhs;
    }
    if (op.compare(MULT) == 0) {
        return lhs * rhs;
    }
    if (op.compare(DIV) == 0) {
        return lhs / rhs;
    }
    return 0.0;
}

void clean_up_tree(gvsu::Node<string> *pos) {
    if (pos != nullptr){
        clean_up_tree(pos->left);
        clean_up_tree(pos->right);
        delete pos;
    }
    pos = nullptr;
}

void create_tree() {
    string current_token = "";
    gvsu::Node<string> *lhs;
    gvsu::Node<string> *rhs;

    for (unsigned int j = 0; j < postfix.size(); ++j) {
        current_token = postfix[j];
        if (is_operand(current_token)) {
            gvsu::Node<string> *operand = new gvsu::Node<string>();
            operand->data = current_token;
            operand->left = nullptr;
            operand->right = nullptr;
            results.push(operand);
        } else {
            rhs = results.top();
            results.pop();
            lhs = results.top();
            results.pop();
            gvsu::Node<string> *op = new gvsu::Node<string>();
            op->data = current_token;
            op->left = lhs;
            op->right = rhs;
            results.push(op);
        }
    }
}

float evaluate_tree(gvsu::Node<string> *pos) {
    float result;
    if (pos->left == NULL && pos->right == NULL) {
        result = stof(pos->data);
    } else {
        pid_t pid;
        int tube[2];
        if (pipe (tube) < 0) {
            perror ("Error creating pipe ");
            exit (1);
        }
        if ((pid = fork()) < 0) {
            perror("Fork failure");
            exit(1);
        } else if (pid == 0) { /* child process */
            float left_child_result;
            float right_child_result;
            cout << "pid# " << getppid() << " created process pid# " << getpid() << "\n" << endl;
            signal(SIGUSR1, sigusr_one_handler);
            child_processes.erase(child_processes.begin(), child_processes.end());
            left_child_result  = evaluate_tree(pos->left);
            right_child_result = evaluate_tree(pos->right);
            result = get_operation(pos->data, left_child_result, right_child_result);
            cout << "pid# " << getpid() << " evaluated: "
                << left_child_result << " "
                << pos->data << " "
                << right_child_result << " "
                << "= " << result << endl;
            close(tube[READ]);
            if (!evaluate_immediately) {
                pause();
            }
            cout << "pid# " << getpid()
                << " returns the value " << result << "\n" << endl;
            write(tube[WRITE], &result, sizeof(result));
            exit(1);
        } else { /* parent logic */
            int status;
            status = 0;
            child_processes.push_back(pid);
            cout << "pid# " << getpid() << " is now the parent of:" << endl;
            for (int child : child_processes) {
                cout << "- pid# " << child << endl;
                waitpid(child, &status, 0);
            }
            cout << endl;
            close(tube[WRITE]);
            read(tube[READ], &result, sizeof(result));
            child_processes.erase(child_processes.begin(), child_processes.end());
            return result;
        }
    }
    return result;
}

float evaluate(const string expr, bool immediate_result) {
    float root_result;

    evaluate_immediately = immediate_result;
    /* Parse through infix expression */
    get_tokens(expr);
    /* Create syntax tree from postfix expression */
    create_tree();
    /* Traverse infix tree */
    root_result = evaluate_tree(results.top());
    /* Clean up routines */
    clean_up_tree(results.top());
    postfix.erase(postfix.begin(), postfix.end());

    return root_result;
}

int main() {
    init_precedence();
    signal(SIGINT, sigint_handler);
    cout << "starting main process with pid# " << getpid() << endl;
    printf("Test 1 %f\n", evaluate("2.0 * 3.0 * 3.0", true));
    printf("Test 2 %f\n", evaluate("2.0 * 3.0 + 4.0 / 5.0", true));
    printf("Test 3 %f\n", evaluate("200.0 + 300.0", true));
    printf("Test 4 %f\n", evaluate("10.0 / 5.0", true));
    printf("Test 5 %f\n", evaluate("16.0 - 10.5", true));
    printf("Test 6 %f\n", evaluate("2.0 + 3.0 * 4.0 - 5.0 / 6.0", true));
    return 0;    
}