#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H
#include <cmath>
#include <string>
#include <vector>
struct Monom {
  double koefficient;
  int degree;
  Monom(double koef = 0.0, int deg = 0) : koefficient(koef), degree(deg) {}
  int getX() const { return degree / 100; }
  int getY() const { return (degree / 10) % 10; }
  int getZ() const { return degree % 10; }
  bool isZero() const { return std::abs(koefficient) < 1e-10; }
  bool operator<(const Monom &other) const { return degree > other.degree; }
  bool operator==(const Monom &other) const { return degree == other.degree; }
};
struct Node {
  Monom data;
  Node *next;
  Node(const Monom &m = Monom(), Node *n = nullptr) : data(m), next(n) {}
};
class Polynomials {
private:
  Node *head;
  int count;
  void clear();
  void copyFrom(const Polynomials &other);
  void insertSorted(const Monom &other);
  void removeZeros();
public:
  Polynomials();
  Polynomials(const Polynomials &other);
  ~Polynomials();
  Polynomials &operator=(const Polynomials &other);
  Polynomials operator+(const Polynomials &other) const;
  Polynomials operator-(const Polynomials &other) const;
  Polynomials operator*(double constant) const;
  Polynomials operator*(const Polynomials &other) const;
  friend std::ostream &operator<<(std::ostream &os, const Polynomials &p);
  friend std::istream &operator>>(std::istream &is, Polynomials &p);
  bool isEmpty() const { return head->next == nullptr; }
  int size() const { return count; }
  bool parseFromString(const std::string &str);
  std::vector<Monom> getMonoms() const;
};
#endif
