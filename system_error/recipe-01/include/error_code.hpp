// -*- C++ -*-
// HeXu's
// 2013 May

#ifndef MINI_STL_ERROR_CODE_INC
#define MINI_STL_ERROR_CODE_INC

#include <string>
#include "cerrno.hpp"

namespace mini_stl {

template<typename T>
struct is_error_code_enum: public std::false_type {};

template<typename T>
struct is_error_condition_enum: public std::false_type {};

enum class errc {
    success = 0,
    address_family_not_supported = EAFNOSUPPORT,
    address_in_use = EADDRINUSE,
    address_not_available = EADDRNOTAVAIL,
    already_connected = EISCONN,
    argument_list_too_long = E2BIG,
    argument_out_of_domain = EDOM,
    bad_address = EFAULT,
    bad_file_descriptor = EBADF,
    bad_message = EBADMSG,
    broken_pipe = EPIPE,
    connection_aborted = ECONNABORTED,
    connection_already_in_progress = EALREADY,
    connection_refused = ECONNREFUSED,
    connection_reset = ECONNRESET,
    cross_device_link = EXDEV,
    destination_address_required = EDESTADDRREQ,
    device_or_resource_busy = EBUSY,
    directory_not_empty = ENOTEMPTY,
    executable_format_error = ENOEXEC,
    file_exists = EEXIST,
    file_too_large = EFBIG,
    filename_too_long = ENAMETOOLONG,
    function_not_supported = ENOSYS,
    host_unreachable = EHOSTUNREACH,
    identifier_removed = EIDRM,
    illegal_byte_sequence = EILSEQ,
    inappropriate_io_control_operation = ENOTTY,
    interrupted = EINTR,
    invalid_argument = EINVAL,
    invalid_seek = ESPIPE,
    io_error = EIO,
    is_a_directory = EISDIR,
    message_size = EMSGSIZE,
    network_down = ENETDOWN,
    network_reset = ENETRESET,
    network_unreachable = ENETUNREACH,
    no_buffer_space = ENOBUFS,
    no_child_process = ECHILD,
    no_link = ENOLINK,
    no_lock_available = ENOLCK,
    no_message_available = ENODATA,
    no_message = ENOMSG,
    no_protocol_option = ENOPROTOOPT,
    no_space_on_device = ENOSPC,
    no_stream_resources = ENOSR,
    no_such_device_or_address = ENXIO,
    no_such_device = ENODEV,
    no_such_file_or_directory = ENOENT,
    no_such_process = ESRCH,
    not_a_directory = ENOTDIR,
    not_a_socket = ENOTSOCK,
    not_a_stream = ENOSTR,
    not_connected = ENOTCONN,
    not_enough_memory = ENOMEM,
    not_supported = ENOTSUP,
    operation_canceled = ECANCELED,
    operation_in_progress = EINPROGRESS,
    operation_not_permitted = EPERM,
    operation_not_supported = EOPNOTSUPP,
    operation_would_block = EWOULDBLOCK,
    owner_dead = EOWNERDEAD,
    permission_denied = EACCES,
    protocol_error = EPROTO,
    protocol_not_supported = EPROTONOSUPPORT,
    read_only_file_system = EROFS,
    resource_deadlock_would_occur = EDEADLK,
    resource_unavailable_try_again = EAGAIN,
    result_out_of_range = ERANGE,
    state_not_recoverable = ENOTRECOVERABLE,
    stream_timeout = ETIME,
    text_file_busy = ETXTBSY,
    timed_out = ETIMEDOUT,
    too_many_files_open_in_system = ENFILE,
    too_many_files_open = EMFILE,
    too_many_links = EMLINK,
    too_many_symbolic_link_levels = ELOOP,
    value_too_large = EOVERFLOW,
    wrong_protocol_type = EPROTOTYPE
};

template<> struct is_error_condition_enum<errc>: public std::true_type {};

class error_category;

class error_code;

class error_condition;

/**
 * Return generic category
 * Returns a reference to the static object of the error_category type 
 * that has the following characteristics:
 * Its name member function returns a pointer to 
 * the character sequence "generic".
 */
const error_category& generic_category() noexcept;

/**
 * Return system category
 * Returns a reference to the static object of the error_category type 
 * that has the following characteristics:
 * Its name member function returns a pointer to 
 * the character sequence "system".
 */
const error_category& system_category() noexcept;

/**
 * Error category
 * This type serves as a base class for specific category types.
 */
class error_category {
public:
    /**
     * Construct error_category
     * The implicit default constructor constructs an object of this base type.
     */
protected:
    error_category() = default;

public:
    error_category(const error_category&) = delete;
    error_category& operator= (const error_category&) = delete;

    /**
     * Relational operators
     * These member functions compare whether two error_category objects 
     * are the same.
     */
    bool operator== (const error_category& rhs) const noexcept
    {
        return this == &rhs;
    }

    bool operator!= (const error_category& rhs) const noexcept
    {
        return this != &rhs;
    }

    bool operator< (const error_category& rhs) const noexcept
    {
        return std::less<const error_category*>()(this, &rhs);
    }

    /**
     * Destroy error_category
     * Destroys an object of this type.
     */
    virtual ~error_category() noexcept {}

    /**
     * Return category name
     * In derived classes, the function returns a C-string naming the category.
     */
    virtual const char* name() const noexcept = 0;

    /**
     * Default error condition
     * Returns the default error_condition object of this category that 
     * is associated with the error_code identified by a value of val.
     */
    virtual error_condition default_error_condition(int val) const noexcept;

    /**
     * Check error code equivalence
     * Checks whether, for the category, an error code is equivalent to 
     * an error condition.
     */
    virtual bool equivalent(int valcode, const error_condition& cond) 
        const noexcept;
    virtual bool equivalent(const error_code& code, int valcond) const noexcept;

    /**
     * Error message
     * In derived classes, the function returns a string object 
     * with a message describing the error condition denoted by val.
     */
    virtual std::string message(int val) const = 0;
};

/**
 * Error condition
 * Objects of this type hold a condition value associated with a category.
 */
class error_condition {
private:
    int value_;
    const error_category *category_;

public:
    /**
     * default constructor
     * Error condition with a value of 0 of the generic_category 
     * (associated with lack of error).
     */
    error_condition() noexcept: value_(0), category_(&generic_category()) {}

    /**
     * initialization constructor
     * Error condition with a value of val of the category specified by cat.
     */
    error_condition(int val, const error_category& cat) noexcept: 
        value_(val), category_(&cat) {}

    /**
     * construct from error condition enum type
     * Calls make_error_condition to construct an error code.
     * This constructor only participates in overload resolution 
     * if is_error_condition_enum<ErrorConditionEnum>::value is true. 
     * Which is the case when errc is used as the ErrorConditionEnum type.
     */
    template <class ErrorConditionEnum, typename = typename
        std::enable_if<is_error_condition_enum<ErrorConditionEnum>::value>::type>
    error_condition(ErrorConditionEnum e) noexcept
    {
        *this = make_error_condition(e);
    }

    /**
     * Assign error condition
     * Assigns the error_condition object a value of val associated with 
     * the error_category cat.
     */
    void assign(int val, const error_category& cat) noexcept
    {
        value_ = val;
        category_ = &cat;
    }

    /**
     * Assign error condition
     * Calls make_error_condition to construct an error condition from e, 
     * whose value is assigned to the error_condition object.
     */
    template <class ErrorConditionEnum, typename T = typename
        std::enable_if<is_error_condition_enum<ErrorConditionEnum>::value>::type>
    error_condition& operator= (ErrorConditionEnum e) noexcept
    {
        *this = make_error_condition(e);
        return *this;
    }

    /**
     * Clear error condition
     * Clears the value in the error_condition object so that 
     * it is set to a value of 0 of the generic_category (indicating no error).
     */
    void clear() noexcept
    {
        value_ = 0;
        category_ = &generic_category();
    }

    /**
     * Error condition value
     * Returns the value associated with the error_condition object.
     */
    int value() const noexcept { return value_; }

    /**
     * Get category
     * Returns a reference to the error category associated with 
     * the error_condition object.
     */
    const error_category& category() const noexcept { return *category_; }

    /**
     * Get message
     * Returns the message associated with the error condition.
     */
    std::string message() const { return category_->message(value_); }
    
    /**
     * Convert to bool
     * Returns whether the error condition has a numerical value other than 0.
     * If it is zero (which is generally used to represent no error), 
     * the function returns false, otherwise it returns true.
     */
    explicit operator bool() const 
    {
        return value_ != 0 ? true : false; 
    }
};

/**
 * error code
 * Objects of this type hold an error code.
 */
class error_code {
private:
    int value_;
    const error_category* category_;

public:
    /**
     * default constructor
     * Error code with a value of 0 of the system_category 
     * (generally indicating no error).
     */
    error_code() noexcept: value_(0), category_(&system_category()) {}

    /**
     * initialization constructor
     * Error code with a value of val of the category specified by cat.
     */
    error_code(int val, const error_category& cat) noexcept: 
        value_(val), category_(&cat) {}

    /**
     * construct from error code enum type
     * Calls make_error_code to construct an error code.
     * This constructor only participates in overload resolution 
     * if is_error_code_enum<ErrorCodeEnum>::value is true.
     */
    template <class ErrorCodeEnum, typename = typename
        std::enable_if<is_error_code_enum<ErrorCodeEnum>::value>::type>
    error_code(ErrorCodeEnum e) noexcept
    {
        *this = make_error_code(e);
    }

    /**
     * Assign error code
     * Assigns the error_code object a value of val associated with 
     * the error_category cat.
     */
    void assign(int val, const error_category& cat) noexcept 
    {
        value_ = val; 
        category_ = &cat;
    }

    /**
     * Assign error code
     * Calls make_error_code to construct an error code from e, 
     * whose value is assigned to the error_code object.
     */
    template <class ErrorCodeEnum, typename = typename
        std::enable_if<is_error_code_enum<ErrorCodeEnum>::value>::type>
    error_code& operator= (ErrorCodeEnum e) noexcept
    {
        *this = make_error_code(e);
        return *this;
    }

    /**
     * Clear error code
     * Clears the value in the error_code object so that it is set to 
     * a value of 0 of the system_category (indicating no error).
     */
    void clear() noexcept 
    {
        value_ = 0;
        category_ = &system_category();
    }

    /**
     * error value
     * Returns the error value associated with the error_code object.
     */
    int value() const noexcept { return value_; }

    /**
     * Get category
     * Returns a reference to the error category associated with 
     * the error_code object.
     */
    const error_category& category() const noexcept { return *category_; }

    /**
     * Default error condition
     * Returns the default error_condition object associated with 
     * the error_code object.
     */
    error_condition default_error_condition() const noexcept
    {
        return category_->default_error_condition(value_);
    }

    /**
     * Get message
     * Returns the message associated with the error code.
     */
    std::string message() const { return category_->message(value_); }

    /**
     * Convert to bool
     * Returns whether the error code has a numerical value other than 0.
     * If it is zero (which is generally used to represent no error), 
     * the function returns false, otherwise it returns true.
     */
    explicit operator bool() const 
    {
        return value_ != 0 ? true : false;
    }
};

/**
 * Relational operators
 * Returns the result of the relational operation between lhs and rhs,
 * which both objects of type error_code: 
 * if they are of the same category, the function returns the result of 
 * applying the appropriate operator (==, !=, <) on their associated values.
 * otherwise, the function returns the result of 
 * applying the operator (==, !=, <) to their categories.
 */
inline 
bool operator== (const error_code& lhs, const error_code& rhs) noexcept
{
    return lhs.category() == rhs.category() && lhs.value() == rhs.value();
}

inline 
bool operator!= (const error_code& lhs, const error_code& rhs) noexcept
{
    return !(lhs == rhs);
}

inline 
bool operator< (const error_code& lhs, const error_code& rhs) noexcept
{
    return lhs.category() < rhs.category() || 
        (lhs.category() == rhs.category() && lhs.value() < rhs.value());
}

/**
 * Relational operators
 * Returns the result of the relational operation between lhs and rhs,
 * which both objects of type error_condition: 
 * if they are of the same category, the function returns the result of 
 * applying the appropriate operator (==, !=, <) on their associated values.
 * otherwise, the function returns the result of 
 * applying the operator (==, !=, <) to their categories.
 */
inline 
bool operator== (const error_condition& lhs, const error_condition& rhs) noexcept
{
    return lhs.category() == rhs.category() && lhs.value() == rhs.value();
}

inline 
bool operator!= (const error_condition& lhs, const error_condition& rhs) noexcept
{
    return !(lhs == rhs);
}

inline 
bool operator< (const error_condition& lhs, const error_condition& rhs) noexcept
{
    return lhs.category() < rhs.category() || 
        (lhs.category() == rhs.category() && lhs.value() < rhs.value());
}

/**
 * Relational operators
 * Returns the result of the relational operation between 
 * an error_code object and an error_condition object, 
 * calls to error_category::equivalent to determine 
 * whether they are equivalent.
 */
inline 
bool operator== (const error_code& code, const error_condition& cond) noexcept
{
    return code.category().equivalent(code.value(), cond) ||
        cond.category().equivalent(code, cond.value());
}

inline 
bool operator== (const error_condition& cond, const error_code& code) noexcept
{
    return cond.category().equivalent(code, cond.value()) ||
        code.category().equivalent(code.value(), cond);
}

inline 
bool operator!= (const error_code& lhs, const error_condition& rhs) noexcept
{
    return !(lhs == rhs);
}

inline 
bool operator!= (const error_condition& lhs, const error_code& rhs) noexcept
{
    return !(lhs == rhs);
}

inline 
error_condition error_category::default_error_condition(int val) const noexcept
{
    return error_condition(val, *this);
}

inline 
bool error_category::equivalent(int valcode, const error_condition& cond)
    const noexcept
{
    return default_error_condition(valcode) == cond;
}

inline 
bool error_category::equivalent(const error_code& code, int valcond)
    const noexcept
{
    return *this == code.category() && code.value() == valcond;
}

/**
 * Insert into ostream
 * Writes a textual representation of the error code.
 */
template <class charT, class traits>
inline 
std::basic_ostream<charT, traits>& operator<< (std::basic_ostream<charT, traits>& os, error_code ec)
{
    os << ec.category().name() << ":" << ec.value();
    return os;
}

/**
 * Make error code
 * Creates an error_code object (of the generic_category) 
 * from the errc enum value e.
 */
inline error_code make_error_code(errc e) noexcept
{
    return error_code(static_cast<int>(e), generic_category());
}

/**
 * Make error condition
 * Creates an error_condition object from the errc enum value e 
 * (of the generic_category).
 */
inline error_condition make_error_condition(errc e) noexcept
{
    return error_condition(static_cast<int>(e), generic_category());
}

}    // namespace mini_stl

namespace std {

template <> struct hash<mini_stl::error_code>
{
    size_t operator ()(const mini_stl::error_code& ec) const noexcept
    {
        return static_cast<size_t>(ec.value())
            + reinterpret_cast<size_t>(&ec.category());
    }
};

}    // namespace std

#endif // MINI_STL_ERROR_CODE_INC

