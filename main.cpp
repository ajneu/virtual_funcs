#include <iostream>
#include <iomanip>
#include <string>
#include <initializer_list>

constexpr size_t sp_1 = 21;
constexpr size_t sp_2 = 15;
constexpr size_t sp_3 = 30;

class Printer {
public:
  Printer(const std::string &name, std::string::size_type sp1, std::string::size_type sp2)
    : class_name{name}, space1{sp1}, space2{sp2}
  {
  }
  std::ostream &print_constructor(std::ostream &os)      {
    os << std::setw(space1) << std::left;
    constructor(os) << std::setw(space2) << std::left << "constructor";
    return os;
  }
  std::ostream &print_copy_constructor(std::ostream &os) {
    os << std::setw(space1) << std::left;
    copy_constructor(os) << std::setw(space2) << std::left << "copy_constructor";
    return os;
  }
  std::ostream &print_destructor(std::ostream &os) {
    os << std::setw(space1) << std::left;
    destructor(os) << std::setw(space2) << std::left << "destructor";
    return os;
  }
  
  std::ostream &print_member_call(std::ostream &os, const std::string &member_name, std::initializer_list<const std::string> args) {
    os << std::setw(space1) << std::left;
    member_call(os, member_name, args) << std::setw(space2) << std::left << "member_call";
    return os;
  }

  const std::string &get_name() { return class_name; }
  
private:
  const std::string            class_name;
  const std::string::size_type space1;
  const std::string::size_type space2;

  std::ostream &constructor(std::ostream &os)
  { os << class_name + "()"; return os; }
  
  std::ostream &copy_constructor(std::ostream &os)
  { os << class_name + "(const " + class_name + " &)"; return os; }
  
  std::ostream &destructor(std::ostream &os)
  { os << '~' + class_name + "()"; return os;}
  
  std::ostream &member_call(std::ostream &os, const std::string &member_name, const std::initializer_list<const std::string> &args);
};

std::ostream &Printer::member_call(std::ostream &os, const std::string &member_name, const std::initializer_list<const std::string> &args)
{
  std::string tmp{class_name + "::" + member_name + '('};
  const auto end = args.end();
  auto it = args.begin();
  if (it != end) {
    tmp += *it;
    while (++it != end) {
      tmp += ", ";
      tmp +=*it;
    }
  }
  tmp += ')';
  os << tmp;
  return os;
}



class Base {
public:
  Base(const std::string &class_name = "Base")
    : print{class_name, sp_1, sp_2}
  {
    print.print_constructor(std::cout);
    virt();
  }
  Base(const Base &rhs)
    : print{rhs.print}
  {
    print.print_copy_constructor(std::cout);
    virt();
  }

  Base &operator=(const Base &rhs) {
  }

  virtual ~Base() {
    print.print_destructor(std::cout);
    virt();
  }

  void call(const std::string &msg = "") {
    print.print_member_call(std::cout, "call", {});
    virt(msg);
  }

private:
  Printer print;

  virtual void virt(const std::string &str="") { std::cout << std::setw(sp_3) << std::left << str << "Calling virt calls:  " << print.get_name() << "::virt" << std::endl; }
};


template <typename Base1>
class Deriv : public Base1 {
public:
  template <typename... Args>
  Deriv(const std::string &class_name = "Deriv", Args&&... args)
    : Base1{std::forward<Args>(args)...}, print{class_name, sp_1, sp_2}
  {
    print.print_constructor(std::cout);
    virt();
  }

  Deriv(const std::string &class_name = "Deriv")
    : print{class_name, sp_1, sp_2}
  {
    print.print_constructor(std::cout);
    virt();
  }

  Deriv(const Deriv &rhs)
    : Base1{rhs}, print{rhs.print}
  {
    print.print_copy_constructor(std::cout);
    virt();
  }
  Deriv &operator=(const Deriv &rhs) {
  }
  virtual ~Deriv() {
    print.print_destructor(std::cout);
    virt();
    Base1::call(std::string("called from ~") + print.get_name() + "()");
  }

private:
  Printer print;
  virtual void virt(const std::string &str="") { std::cout << std::setw(sp_3) << std::left << str << "Calling virt calls:  " << print.get_name() << "::virt" << std::endl; }
};

int main()
{
  Deriv<Deriv<Base>> d1{"Deriv_inner", "Deriv_outer", "Base"};
  return 0;
}
