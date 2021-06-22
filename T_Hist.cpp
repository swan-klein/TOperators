#include <algorithm>
#include <iostream>
#include <vector>
#include <array>
#include <sstream>
#include <bitset>
#include <iterator>
#include <stdint.h>
#include "Z2.hpp"
#include "SO6.hpp"
#include "T_Hist.hpp"

struct T_Hist::Node
{
    SO6 so6;
    Node *next[15];
    Node *prev;
};

SO6 T_Hist::tsv[15] = {SO6::tMatrix(0, 1, 0), SO6::tMatrix(0, 2, 1), SO6::tMatrix(0, 3, 2), SO6::tMatrix(0, 4, 3), SO6::tMatrix(0, 5, 4), SO6::tMatrix(1, 2, 5), SO6::tMatrix(1, 3, 6), SO6::tMatrix(1, 4, 7), SO6::tMatrix(1, 5, 8), SO6::tMatrix(2, 3, 9), SO6::tMatrix(2, 4, 10), SO6::tMatrix(2, 5, 11), SO6::tMatrix(3, 4, 12), SO6::tMatrix(3, 5, 13), SO6::tMatrix(4, 5, 14)};
T_Hist::Node *T_Hist::head = NULL;

T_Hist::T_Hist()
{
    hist.clear();
}

T_Hist::T_Hist(std::vector<int8_t> &new_hist)
{
    hist = new_hist;
}

void T_Hist::initHead()
{
    T_Hist::head = new Node;
    T_Hist::head->so6 = SO6::identity();
}

void T_Hist::tableInsert(Node *t, Node *p, int8_t depth)
{
    if (depth)
    {
        for (int8_t i = 0; i < 15; i++)
        {
            Node *node = new Node;
            t->next[i] = node;
            node->prev = p;
            node->so6 = T_Hist::tsv[i] * t->so6;
            tableInsert(node, t, depth - 1);
        }
    }
}

void T_Hist::tableDelete(Node *t, Node *p)
{
    if (t)
    {
        for (int8_t i = 0; i < 15; i++)
        {
            tableDelete(t->next[i], t);
        }
        delete t;
    }
}

SO6 T_Hist::tableLookup(std::vector<int8_t> index)
{
    Node *node = T_Hist::head;
    for (int8_t i : index)
    {
        node = node->next[i];
    }
    return node->so6;
}

SO6 T_Hist::reconstruct()
{
    std::vector<int8_t> left(hist.begin(), hist.begin() + hist.size() / 2);
    std::vector<int8_t> right(hist.begin() + hist.size() / 2, hist.end());
    SO6 ret = tableLookup(left) * tableLookup(right);
    ret.reduced_rep();
    return ret;
}

T_Hist T_Hist::operator*(T_Hist &other)
{
    std::vector<int8_t> history;
    for (int8_t i : hist)
        history.push_back(i);
    for (int8_t i : other.hist)
        history.push_back(i);
    return T_Hist(history);
}

bool T_Hist::operator==(T_Hist &other)
{
    return this->reconstruct() == other.reconstruct();
}

bool T_Hist::operator<(T_Hist &other)
{
    return this->reconstruct() < other.reconstruct();
}

bool T_Hist::operator<(const T_Hist &other) const
{
    T_Hist t = *this;
    T_Hist o = other;
    return t.reconstruct() < o.reconstruct();
}

std::ostream &operator<<(std::ostream &os, const T_Hist &h)
{
    for (int8_t i : h.hist)
    {
        os << std::hex << +i;
    }
    return os;
}