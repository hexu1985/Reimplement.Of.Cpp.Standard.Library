#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
#include "unique_ptr.hpp"
 
// helper class for runtime polymorphism demo below
struct B {
  virtual void bar() { std::cout << "B::bar\n"; }
  virtual ~B() = default;
};
struct D : B
{
    D() { std::cout << "D::D\n";  }
    ~D() { std::cout << "D::~D\n";  }
    void bar() override { std::cout << "D::bar\n";  }
};
 
// a function consuming a unique_ptr can take it by value or by rvalue reference
Hx::unique_ptr<D> pass_through(Hx::unique_ptr<D> p)
{
    p->bar();
    return p;
}
 
// helper function for the custom deleter demo below
void close_file(std::FILE* fp) { std::fclose(fp); }
 
// unique_ptr-based linked list demo
struct List {
  struct Node {
    int data;
    Hx::unique_ptr<Node> next;
    Node(int data) : data{data}, next{nullptr} {}
  };
  List() : head{nullptr} {};
  // N.B. iterative destructor to avoid stack overflow on long lists
  ~List() { while(head) head = std::move(head->next); }
  // copy/move and other APIs skipped for simplicity
  void push(int data) {
    auto temp = Hx::make_unique<Node>(data);
    if(head) temp->next = std::move(head);
    head = std::move(temp);
  }
private:
  Hx::unique_ptr<Node> head;
};
 
int main()
{
  std::cout << "unique ownership semantics demo\n";
  {
      auto p = Hx::make_unique<D>(); // p is a unique_ptr that owns a D
      auto q = pass_through(std::move(p)); 
      assert(!p); // now p owns nothing and holds a null pointer
      q->bar();   // and q owns the D object
  } // ~D called here
 
  std::cout << "Runtime polymorphism demo\n";
  {
    Hx::unique_ptr<B> p = Hx::make_unique<D>(); // p is a unique_ptr that owns a D
                                                  // as a pointer to base
    p->bar(); // virtual dispatch
 
    std::vector<Hx::unique_ptr<B>> v;  // unique_ptr can be stored in a container
    v.push_back(Hx::make_unique<D>());
    v.push_back(std::move(p));
    v.emplace_back(new D);
    for(auto& p: v) p->bar(); // virtual dispatch
  } // ~D called 3 times
 
  std::cout << "Custom deleter demo\n";
  std::ofstream("demo.txt") << 'x'; // prepare the file to read
  {
      Hx::unique_ptr<std::FILE, decltype(&close_file)> fp(std::fopen("demo.txt", "r"),
                                                           &close_file);
      if(fp) // fopen could have failed; in which case fp holds a null pointer
        std::cout << (char)std::fgetc(fp.get()) << '\n';
  } // fclose() called here, but only if FILE* is not a null pointer
    // (that is, if fopen succeeded)
 
  std::cout << "Custom lambda-expression deleter demo\n";
  {
    Hx::unique_ptr<D, std::function<void(D*)>> p(new D, [](D* ptr)
        {
            std::cout << "destroying from a custom deleter...\n";
            delete ptr;
        });  // p owns D
    p->bar();
  } // the lambda above is called and D is destroyed
 
  std::cout << "Array form of unique_ptr demo\n";
  {
      Hx::unique_ptr<D[]> p{new D[3]};
  } // calls ~D 3 times
 
  std::cout << "Linked list demo\n";
  {
    List l;
    for(long n = 0; n != 1'000'000; ++n) l.push(n);
  } // destroys all 1 million nodes
}

/*
Output:

unique ownership semantics demo
D::D
D::bar
D::bar
D::~D
Runtime polymorphism demo
D::D
D::bar
D::D
D::D
D::bar
D::bar
D::bar
D::~D
D::~D
D::~D
Custom deleter demo
x
Custom lambda-expression deleter demo
D::D
D::bar
destroying from a custom deleter...
D::~D
Array form of unique_ptr demo
D::D
D::D
D::D
D::~D
D::~D
D::~D
*/
