//
// Created by Josiah Campbell on 2/22/16.
//
#include <string>
#include "Node.h"

#ifndef PROJECT2_EVALUATOR_H
#define PROJECT2_EVALUATOR_H

using namespace std;

void sigint_handler(int sig);

void sigusr_one_handler(int num);

void init_precedence();

bool is_operand(const string &token);

bool is_operator(const string &token);

void get_tokens(const string &expr);

float get_operation(const string &op, const float &lhs, const float &rhs);

void create_tree();

float evaluate(const string expr, bool immediate_result);

template<typename Z>
float evaluate_tree(gvsu::Node<Z> *pos);
template<typename Z>
void clean_up_tree(gvsu::Node<Z> *pos);

#endif //PROJECT2_EVALUATOR_H
