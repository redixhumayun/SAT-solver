#include <iostream>
#include <map>
#include <set>
#include <vector>

typedef std::set<char> Clause;
typedef std::vector<Clause> Formula;
typedef std::map<char, bool> TruthTable;

void simplify(Formula &formula, TruthTable &truth_table) {
  /**
   * Iterate over the formula and for each clause, do the following rules:
   * 1. If the value for a variable is T and the clause contains the positive
   * version, remove the clause. It is satisfied. (Because T=T)
   * 2. If the value for a variable is F and the clause contains the negative
   * version, remove the clause. It is satisfied. (Because ~F=T)
   * 3. If the value for a variable is T and the clause contains the negative
   * version, remove the literal from the clause.
   * 4. If the value for a variable is F and the clause contains the positive
   * version, remove the literal from the clause.
   */
  for (const auto &[literal, value] : truth_table) {
    char positive_literal = toupper(literal);
    char negative_literal = tolower(literal);

    for (auto it = formula.begin(); it != formula.end();) {
      //  clauses 1 and 2
      if ((value && it->count(positive_literal)) ||
          (!value && it->count(negative_literal))) {
        it = formula.erase(it);
      } else {
        if (value) {
          //  clause 3
          it->erase(negative_literal);
        } else {
          //  clause 4
          it->erase(positive_literal);
        }
        ++it;
      }
    }
  }
}

bool is_contradictory(Formula &formula) {
  //  check if any clause of the formula has become empty.
  //  if it has, the formula cannot be satisfied
  for (const auto &clause : formula) {
    if (clause.empty()) {
      return true;
    }
  }
  return false;
}

bool DPLL(Formula formula, TruthTable &truth_table) {
  simplify(formula, truth_table);

  if (formula.empty()) return true;
  if (is_contradictory(formula)) return false;

  //  pick the first variable to modify
  char var = *formula[0].begin();
  char neg_var = isupper(var) ? tolower(var) : toupper(var);

  TruthTable truth_table_true = truth_table;
  truth_table_true[var] = true;
  truth_table_true[neg_var] = false;

  //  this is the first branch
  if (DPLL(formula, truth_table_true)) {
    truth_table = truth_table_true;
    return true;
  }

  //  this is the second branch because the first branch didn't satisfy
  TruthTable truth_table_false = truth_table;
  truth_table_false[var] = false;
  truth_table_false[neg_var] = true;

  if (DPLL(formula, truth_table_false)) {
    truth_table = truth_table_false;
    return true;
  }

  return false;
}

int main() {
  Formula formula = {{'A', 'b'}, {'a', 'B'}, {'a', 'b'}};
  TruthTable truth_table;
  bool result = DPLL(formula, truth_table);
  std::cout << "The result is: " << result << std::endl;
  for (const auto &truth_table_pair : truth_table) {
    std::cout << truth_table_pair.first << " = " << truth_table_pair.second
              << std::endl;
  }
  return 0;
}