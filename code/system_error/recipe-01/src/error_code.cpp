// -*- C++ -*-
// HeXu's
// 2013 Jun

#include <cstring>
#include "error_code.hpp"

namespace mini_stl {

namespace {

class generic_error_category: public error_category {
public:
    generic_error_category() = default;

    const char* name() const throw() { return "generic"; }

    std::string message(int val) const;
};

class system_error_category: public error_category {
public:
    system_error_category() = default;

    const char* name() const throw() { return "system"; }

    std::string message(int val) const
    {
        // return generic_category().message(val);
        return strerror(val);
    }

    error_condition default_error_condition(int val) const throw();
};

std::string generic_error_category::message(int val) const
{
    // equivalent to call strerror, but strerror is not thread safe
    switch (val) {
    case 0:
        return "Success";
    case E2BIG:
        return "Argument list too long";
    case EACCES:
        return "Permission denied";
    case EADDRINUSE:
        return "Address already in use";
    case EADDRNOTAVAIL:
        return "Cannot assign requested address";
    case EAFNOSUPPORT:
        return "Address family not supported by protocol";
#if EAGAIN != EWOULDBLOCK
    case EAGAIN:
        return "Resource temporarily unavailable";
#endif
#if EALREADY != EBUSY    //  EALREADY and EBUSY are the same on QNX Neutrino
    case EALREADY:
        return "Operation already in progress";
#endif
    case EBADF:
        return "Bad file descriptor";
    case EBADMSG:
        return "Bad message";
    case EBUSY:
        return "Device or resource busy";
    case ECANCELED:
        return "Operation canceled";
    case ECHILD:
        return "No child processes";
    case ECONNABORTED:
        return "Software caused connection abort";
    case ECONNREFUSED:
        return "Connection refused";
    case ECONNRESET:
        return "Connection reset by peer";
    case EDEADLK:
        return "Resource deadlock avoided";
    case EDESTADDRREQ:
        return "Destination address required";
    case EDOM:
        return "Numerical argument out of domain";
    case EEXIST:
        return "File exists";
    case EFAULT:
        return "Bad address";
    case EFBIG:
        return "File too large";
    case EHOSTUNREACH:
        return "No route to host";
    case EIDRM:
        return "Identifier removed";
    case EILSEQ:
        return "Invalid or incomplete multibyte or wide character";
    case EINPROGRESS:
        return "Operation now in progress";
    case EINTR:
        return "Interrupted system call";
    case EINVAL:
        return "Invalid argument";
    case EIO:
        return "Input/output error";
    case EISCONN:
        return "Transport endpoint is already connected";
    case EISDIR:
        return "Is a directory";
    case ELOOP:
        return "Too many levels of symbolic links";
    case EMFILE:
        return "Too many open files";
    case EMLINK:
        return "Too many links";
    case EMSGSIZE:
        return "Message too long";
    case ENAMETOOLONG:
        return "File name too long";
    case ENETDOWN:
        return "Network is down";
    case ENETRESET:
        return "Network dropped connection on reset";
    case ENETUNREACH:
        return "Network is unreachable";
    case ENFILE:
        return "Too many open files in system";
    case ENOBUFS:
        return "No buffer space available";
    case ENODATA:
        return "No data available";
    case ENODEV:
        return "No such device";
    case ENOENT:
        return "No such file or directory";
    case ENOEXEC:
        return "Exec format error";
    case ENOLCK:
        return "No locks available";
    case ENOLINK:
        return "Link has been severed";
    case ENOMEM:
        return "Cannot allocate memory";
    case ENOMSG:
        return "No message of desired type";
    case ENOPROTOOPT:
        return "Protocol not available";
    case ENOSPC:
        return "No space left on device";
    case ENOSR:
        return "Out of streams resources";
    case ENOSTR:
        return "Device not a stream";
    case ENOSYS:
        return "Function not implemented";
    case ENOTCONN:
        return "Transport endpoint is not connected";
    case ENOTDIR:
        return "Not a directory";
#if ENOTEMPTY != EEXIST    // AIX treats ENOTEMPTY and EEXIST as the same value
    case ENOTEMPTY:
        return "Directory not empty";
#endif
#if ENOTRECOVERABLE != ECONNRESET    // the same on some Broadcom chips 
    case ENOTRECOVERABLE:
        return "State not recoverable";
#endif
    case ENOTSOCK:
        return "Socket operation on non-socket";
    case ENOTSUP:
        return "Operation not supported";
    case ENOTTY:
        return "Inappropriate ioctl for device";
    case ENXIO:
        return "No such device or address";
#if EOPNOTSUPP != ENOTSUP
    case EOPNOTSUPP:
        return "Operation not supported";
#endif
    case EOVERFLOW:
        return "Value too large for defined data type";
#if EOWNERDEAD != ECONNABORTED // the same on some Broadcom chips 
    case EOWNERDEAD:
        return "Owner died";
#endif
    case EPERM:
        return "Operation not permitted";
    case EPIPE:
        return "Broken pipe";
    case EPROTO:
        return "Protocol error";
    case EPROTONOSUPPORT:
        return "Protocol not supported";
    case EPROTOTYPE:
        return "Protocol wrong type for socket";
    case ERANGE:
        return "Numerical result out of range";
    case EROFS:
        return "Read-only file system";
    case ESPIPE:
        return "Illegal seek";
    case ESRCH:
        return "No such process";
    case ETIME:
        return "Timer expired";
    case ETIMEDOUT:
        return "Connection timed out";
    case ETXTBSY:
        return "Text file busy";
    case EWOULDBLOCK:
        return "Resource temporarily unavailable";
    case EXDEV:
        return "Invalid cross-device link";
    default:
        break;
    } 
    
    return std::string("Unknown error ")+std::to_string(val);
}

error_condition system_error_category::default_error_condition(int val) 
    const throw()
{
    return error_condition(val, system_category());
}

}   // namespace

const error_category& generic_category() throw()
{
    static const generic_error_category generic_category_;
    return generic_category_;
}

const error_category& system_category() throw()
{
    static const system_error_category system_category_;
    return system_category_;
}

}   // namespace mini_stl


