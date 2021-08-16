#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <any>

class any_out {
private:
    struct streamer {
        virtual void print(std::ostream & o,std::any& a)=0;
        virtual streamer* clone()=0;
        virtual ~streamer() {}
    };

    template <typename T> struct streamer_imp : public streamer {
        virtual void print(std::ostream& o,std::any& a) {
            o << *std::any_cast<T>(&a);
        }

        virtual streamer* clone() {
            return new streamer_imp<T>();
        }
    };

    streamer* streamer_;
    std::any o_;

public:
    any_out() : streamer_(0) {}

    template <typename T> any_out(const T& value) :
        streamer_(new streamer_imp<T>),o_(value) {}

    any_out(const any_out& a)
        : streamer_(a.streamer_?a.streamer_->clone():0),o_(a.o_) {}

    template<typename T> any_out& operator=(const T& r) {
        any_out(r).swap(*this);
        return *this;
    }

    any_out& operator=(const any_out& r) {
        any_out(r).swap(*this);
        return *this;
    }

    ~any_out() {
        delete streamer_;
    }

    any_out& swap(any_out& r) {
        std::swap(streamer_, r.streamer_);
        std::swap(o_,r.o_);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& o,any_out& a) {
        if (a.streamer_) {
            a.streamer_->print(o,a.o_);
        }
        return o;
    }
};

int main() {
    std::vector<any_out> vec;

    any_out a(std::string("I do have operator<<"));

    vec.push_back(a);
    vec.push_back(112);
    vec.push_back(65.535);

    // 打印vector vec 中的所有东西
    std::cout << vec[0] << "\n";
    std::cout << vec[1] << "\n";
    std::cout << vec[2] << "\n";

    a=std::string("This is great!");
    std::cout << a;
    std::cout << "\n";
}
