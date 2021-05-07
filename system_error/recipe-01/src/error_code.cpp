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

	const char *name() const throw() { return "generic"; }

	std::string message(int val) const;
};

class system_error_category: public error_category {
public:
	system_error_category() = default;

	const char *name() const throw() { return "system"; }

	std::string message(int val) const
	{
		return generic_category().message(val);
	}

	error_condition default_error_condition(int val) const throw();
};

std::string generic_error_category::message(int val) const
{
	switch (val) {
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
#if EALREADY != EBUSY	//  EALREADY and EBUSY are the same on QNX Neutrino
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
#if ENOTEMPTY != EEXIST	// AIX treats ENOTEMPTY and EEXIST as the same value
	case ENOTEMPTY:
		return "Directory not empty";
#endif
#if ENOTRECOVERABLE != ECONNRESET	// the same on some Broadcom chips 
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
	
    char errbuf[256] = {0};
    strerror_r(val, errbuf, sizeof(errbuf));
    return std::string(errbuf);
}

error_condition system_error_category::default_error_condition(int val) 
	const throw()
{
	switch (val) {
	case 0: return make_error_condition(errc::success);
	// POSIX-like O/S -> posix_errno decode table  ---------------------------//
	case E2BIG: return make_error_condition(errc::argument_list_too_long);
	case EACCES: return make_error_condition(errc::permission_denied);
	case EADDRINUSE: return make_error_condition(errc::address_in_use);
	case EADDRNOTAVAIL: return make_error_condition(errc::address_not_available);
	case EAFNOSUPPORT: return make_error_condition(errc::address_family_not_supported);
	case EAGAIN: return make_error_condition(errc::resource_unavailable_try_again);
#if EALREADY != EBUSY  //  EALREADY and EBUSY are the same on QNX Neutrino
	case EALREADY: return make_error_condition(errc::connection_already_in_progress);
#endif
	case EBADF: return make_error_condition(errc::bad_file_descriptor);
	case EBADMSG: return make_error_condition(errc::bad_message);
	case EBUSY: return make_error_condition(errc::device_or_resource_busy);
	case ECANCELED: return make_error_condition(errc::operation_canceled);
	case ECHILD: return make_error_condition(errc::no_child_process);
	case ECONNABORTED: return make_error_condition(errc::connection_aborted);
	case ECONNREFUSED: return make_error_condition(errc::connection_refused);
	case ECONNRESET: return make_error_condition(errc::connection_reset);
	case EDEADLK: return make_error_condition(errc::resource_deadlock_would_occur);
	case EDESTADDRREQ: return make_error_condition(errc::destination_address_required);
	case EDOM: return make_error_condition(errc::argument_out_of_domain);
	case EEXIST: return make_error_condition(errc::file_exists);
	case EFAULT: return make_error_condition(errc::bad_address);
	case EFBIG: return make_error_condition(errc::file_too_large);
	case EHOSTUNREACH: return make_error_condition(errc::host_unreachable);
	case EIDRM: return make_error_condition(errc::identifier_removed);
	case EILSEQ: return make_error_condition(errc::illegal_byte_sequence);
	case EINPROGRESS: return make_error_condition(errc::operation_in_progress);
	case EINTR: return make_error_condition(errc::interrupted);
	case EINVAL: return make_error_condition(errc::invalid_argument);
	case EIO: return make_error_condition(errc::io_error);
	case EISCONN: return make_error_condition(errc::already_connected);
	case EISDIR: return make_error_condition(errc::is_a_directory);
	case ELOOP: return make_error_condition(errc::too_many_symbolic_link_levels);
	case EMFILE: return make_error_condition(errc::too_many_files_open);
	case EMLINK: return make_error_condition(errc::too_many_links);
	case EMSGSIZE: return make_error_condition(errc::message_size);
	case ENAMETOOLONG: return make_error_condition(errc::filename_too_long);
	case ENETDOWN: return make_error_condition(errc::network_down);
	case ENETRESET: return make_error_condition(errc::network_reset);
	case ENETUNREACH: return make_error_condition(errc::network_unreachable);
	case ENFILE: return make_error_condition(errc::too_many_files_open_in_system);
	case ENOBUFS: return make_error_condition(errc::no_buffer_space);
	case ENODATA: return make_error_condition(errc::no_message_available);
	case ENODEV: return make_error_condition(errc::no_such_device);
	case ENOENT: return make_error_condition(errc::no_such_file_or_directory);
	case ENOEXEC: return make_error_condition(errc::executable_format_error);
	case ENOLCK: return make_error_condition(errc::no_lock_available);
	case ENOLINK: return make_error_condition(errc::no_link);
	case ENOMEM: return make_error_condition(errc::not_enough_memory);
	case ENOMSG: return make_error_condition(errc::no_message);
	case ENOPROTOOPT: return make_error_condition(errc::no_protocol_option);
	case ENOSPC: return make_error_condition(errc::no_space_on_device);
	case ENOSR: return make_error_condition(errc::no_stream_resources);
	case ENOSTR: return make_error_condition(errc::not_a_stream);
	case ENOSYS: return make_error_condition(errc::function_not_supported);
	case ENOTCONN: return make_error_condition(errc::not_connected);
	case ENOTDIR: return make_error_condition(errc::not_a_directory);
#if ENOTEMPTY != EEXIST // AIX treats ENOTEMPTY and EEXIST as the same value
	case ENOTEMPTY: return make_error_condition(errc::directory_not_empty);
#endif // ENOTEMPTY != EEXIST
#if ENOTRECOVERABLE != ECONNRESET // the same on some Broadcom chips 
	case ENOTRECOVERABLE: return make_error_condition(errc::state_not_recoverable); 
#endif // ENOTRECOVERABLE != ECONNRESET 
	case ENOTSOCK: return make_error_condition(errc::not_a_socket);
	case ENOTSUP: return make_error_condition(errc::not_supported);
	case ENOTTY: return make_error_condition(errc::inappropriate_io_control_operation);
	case ENXIO: return make_error_condition(errc::no_such_device_or_address);
#if EOPNOTSUPP != ENOTSUP
	case EOPNOTSUPP: return make_error_condition(errc::operation_not_supported);
#endif // EOPNOTSUPP != ENOTSUP
	case EOVERFLOW: return make_error_condition(errc::value_too_large);
#if EOWNERDEAD != ECONNABORTED // the same on some Broadcom chips 
	case EOWNERDEAD: return make_error_condition(errc::owner_dead); 
#endif // EOWNERDEAD != ECONNABORTED 
	case EPERM: return make_error_condition(errc::operation_not_permitted);
	case EPIPE: return make_error_condition(errc::broken_pipe);
	case EPROTO: return make_error_condition(errc::protocol_error);
	case EPROTONOSUPPORT: return make_error_condition(errc::protocol_not_supported);
	case EPROTOTYPE: return make_error_condition(errc::wrong_protocol_type);
	case ERANGE: return make_error_condition(errc::result_out_of_range);
	case EROFS: return make_error_condition(errc::read_only_file_system);
	case ESPIPE: return make_error_condition(errc::invalid_seek);
	case ESRCH: return make_error_condition(errc::no_such_process);
	case ETIME: return make_error_condition(errc::stream_timeout);
	case ETIMEDOUT: return make_error_condition(errc::timed_out);
	case ETXTBSY: return make_error_condition(errc::text_file_busy);
#if EAGAIN != EWOULDBLOCK
	case EWOULDBLOCK: return make_error_condition(errc::operation_would_block);
#endif // EAGAIN != EWOULDBLOCK
	case EXDEV: return make_error_condition(errc::cross_device_link);
	default: return error_condition(val, system_category());
	}
}

}	// namespace

const error_category &generic_category() throw()
{
	static const generic_error_category generic_category_;
	return generic_category_;
}

const error_category &system_category() throw()
{
	static const system_error_category system_category_;
	return system_category_;
}

}	// namespace mini_stl


