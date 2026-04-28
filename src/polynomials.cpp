#include "../include/polynomial.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

Polynomials::Polynomials() {
  head = new Node();
  count = 0;
}
Polynomials::Polynomials(const Polynomials &other) {
  head = new Node();
  count = 0;
  copyFrom(other);
}
Polynomials::~Polynomials() {
  clear();
  delete head;
}
void Polynomials::clear() {
  Node *current = head->next;
  while (current != nullptr) {
    Node *temp = current;
    current = current->next;
    delete temp;
  }
  head->next = nullptr;
  count = 0;
}
void Polynomials::copyFrom(const Polynomials &other) {
  Node *current = other.head->next;
  Node *last = head;
  while (current != nullptr) {
    last->next = new Node(current->data);
    last = last->next;
    current = current->next;
    count++;
  }
  if (last != nullptr) {
    last->next = nullptr;
  }
}
Polynomials &Polynomials::operator=(const Polynomials &other) {
  if (this != &other) {
    clear();
    copyFrom(other);
  }
  return *this;
}
void Polynomials::insertSorted(const Monom &m) {
  if (m.isZero()) {
    return;
  }
  Node *prev = head;
  Node *current = head->next;
  while (current != nullptr && current->data.degree > m.degree) {
    prev = current;
    current = current->next;
  }
  if (current != nullptr && current->data.degree == m.degree) {
    current->data.koefficient += m.koefficient;
    if (std::abs(current->data.koefficient) < 1e-10) {
      prev->next = current->next;
      delete current;
      count--;
    }
  } else {
    prev->next = new Node(m, current);
    count++;
  }
}
void Polynomials::removeZeros() {
  Node *prev = head;
  Node *current = head->next;
  while (current != nullptr) {
    if (current->data.isZero()) {
      prev->next = current->next;
      delete current;
      current = prev->next;
      count--;
    } else {
      prev = current;
      current = current->next;
    }
  }
}
Polynomials Polynomials::operator+(const Polynomials &other) const {
  Polynomials result;
  Node *p1 = head->next;
  Node *p2 = other.head->next;
  while (p1 != nullptr && p2 != nullptr) {
    if (p1->data.degree > p2->data.degree) {
      result.insertSorted(p1->data);
      p1 = p1->next;
    } else if (p1->data.degree < p2->data.degree) {
      result.insertSorted(p2->data);
      p2 = p2->next;
    } else {
      Monom m = p1->data;
      m.koefficient += p2->data.koefficient;
      result.insertSorted(m);
      p1 = p1->next;
      p2 = p2->next;
    }
  }
  while (p1 != nullptr) {
    result.insertSorted(p1->data);
    p1 = p1->next;
  }
  while (p2 != nullptr) {
    result.insertSorted(p2->data);
    p2 = p2->next;
  }
  result.removeZeros();
  return result;
}
Polynomials Polynomials::operator-(const Polynomials &other) const {
  return *this + (other * (-1.0));
}
Polynomials Polynomials::operator*(double constant) const {
  if (std::abs(constant) < 1e-10) {
    return Polynomials();
  }
  Polynomials result;
  Node *current = head->next;
  while (current != nullptr) {
    Monom m = current->data;
    m.koefficient *= constant;
    result.insertSorted(m);
    current = current->next;
  }
  return result;
}
Polynomials Polynomials::operator*(const Polynomials &other) const {
  Polynomials result;
  Node *p1 = head->next;
  while (p1 != nullptr) {
    Node *p2 = other.head->next;
    while (p2 != nullptr) {
      int newX = p1->data.getX() + p2->data.getX();
      int newY = p1->data.getY() + p2->data.getY();
      int newZ = p1->data.getZ() + p2->data.getZ();
      if (newX > 9 || newY > 9 || newZ > 9) {
        throw std::runtime_error("Error: degree of the variable exceeds 9!");
      }
      int newDeg = newX * 100 + newY * 10 + newZ;
      double newKoef = p1->data.koefficient * p2->data.koefficient;
      result.insertSorted(Monom(newKoef, newDeg));
      p2 = p2->next;
    }
    p1 = p1->next;
  }
  result.removeZeros();
  return result;
}
bool Polynomials::parseFromString(const std::string &str) {
  clear();
  std::string s = str;
  s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
  if (s.empty()) {
    return false;
  }
  std::regex monom_regex(
      R"([+-]?(?:\d+(?:\.\d*)?|\.\d+)?(?:\*?x(?:\^?(\d))?)?(?:\*?y(?:\^?(\d))?)?(?:\*?z(?:\^?(\d))?)?)");
  auto begin = std::sregex_iterator(s.begin(), s.end(), monom_regex);
  auto end = std::sregex_iterator();
  for (auto it = begin; it != end; ++it) {
    std::string monom_str = it->str();
    if (monom_str.empty())
      continue;
    double sign = 1.0;
    if (monom_str[0] == '+') {
      monom_str = monom_str.substr(1);
    } else if (monom_str[0] == '-') {
      sign = -1.0;
      monom_str = monom_str.substr(1);
    }
    if (monom_str.empty())
      continue;
    double koeff = 1.0;
    size_t pos = 0;
    if (std::isdigit(monom_str[0]) || monom_str[0] == '.') {
      std::string num_str;
      while (pos < monom_str.size() &&
             (std::isdigit(monom_str[pos]) || monom_str[pos] == '.')) {
        num_str += monom_str[pos];
        pos++;
      }
      if (!num_str.empty()) {
        koeff = std::stod(num_str);
      }
    }
    koeff *= sign;
    if (pos < monom_str.size() && monom_str[pos] == '*') {
      pos++;
    }
    int x_deg = 0, y_deg = 0, z_deg = 0;
    if (pos < monom_str.size() && monom_str[pos] == 'x') {
      x_deg = 1;
      pos++;
      if (pos < monom_str.size() && monom_str[pos] == '^') {
        pos++;
        if (pos < monom_str.size() && std::isdigit(monom_str[pos])) {
          x_deg = monom_str[pos] - '0';
          pos++;
        }
      }
    }
    if (pos < monom_str.size() && monom_str[pos] == 'y') {
      y_deg = 1;
      pos++;
      if (pos < monom_str.size() && monom_str[pos] == '^') {
        pos++;
        if (pos < monom_str.size() && std::isdigit(monom_str[pos])) {
          y_deg = monom_str[pos] - '0';
          pos++;
        }
      }
    }
    if (pos < monom_str.size() && monom_str[pos] == 'z') {
      z_deg = 1;
      pos++;
      if (pos < monom_str.size() && monom_str[pos] == '^') {
        pos++;
        if (pos < monom_str.size() && std::isdigit(monom_str[pos])) {
          z_deg = monom_str[pos] - '0';
          pos++;
        }
      }
    }
    int degree = x_deg * 100 + y_deg * 10 + z_deg;
    insertSorted(Monom(koeff, degree));
  }
  removeZeros();
  return true;
}
std::ostream &operator<<(std::ostream &os, const Polynomials &p) {
  if (p.isEmpty()) {
    os << "0";
    return os;
  }
  Node *current = p.head->next;
  bool first = true;
  while (current != nullptr) {
    const Monom &m = current->data;
    if (first) {
      if (m.koefficient < 0) {
        os << "-";
      }
      first = false;
    } else {
      os << (m.koefficient > 0 ? "+" : "-");
    }
    double abs_koef = std::abs(m.koefficient);
    if (std::abs(abs_koef - 1.0) > 1e-10 || m.degree == 0) {
      os << std::fixed << std::setprecision(2) << abs_koef;
    }
    int x = m.getX();
    int y = m.getY();
    int z = m.getZ();
    if (x > 0) {
      os << "x";
      if (x > 1) {
        os << "^" << x;
      }
    }
    if (y > 0) {
      os << "y";
      if (y > 1) {
        os << "^" << y;
      }
    }
    if (z > 0) {
      os << "z";
      if (z > 1) {
        os << "^" << z;
      }
    }
    current = current->next;
  }
  return os;
}
std::istream &operator>>(std::istream &is, Polynomials &p) {
  std::string input;
  std::cout << "Enter polynomial in the format: 3x^2y + 8z^3xy" << std::endl;
  std::cout << "=Rules=" << std::endl;
  std::cout << "--- Coefficients are integer or double(by a dot) " << std::endl;
  std::cout << "--- Variables: x, y, z (order is just this) " << std::endl;
  std::cout << "--- Degrees: from 0 to 9 (specify through ^) " << std::endl;
  std::cout << "--->";
  std::getline(is, input);
  p.parseFromString(input);
  return is;
}
std::vector<Monom> Polynomials::getMonoms() const {
  std::vector<Monom> result;
  Node *current = head->next;
  while (current != nullptr) {
    result.push_back(current->data);
    current = current->next;
  }
  return result;
}