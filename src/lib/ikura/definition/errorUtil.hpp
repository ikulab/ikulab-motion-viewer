#include <string>

#define RT_ERR_MSG(msg) \
    runtime_error_message(msg, __FILE__, __func__, __LINE__)

#define THROW_RT_ERR(msg) \
	throw std::runtime_error(RT_ERR_MSG(msg))

const std::string runtime_error_message(
	std::string msg,
	std::string file,
	std::string func,
	int line) {

	std::string res;
	res += "Runtime Error\n";
	res += "\n";
	res += msg;
	res += "\n\nFile     : ";
	res += file;
	res += ":";
	res += std::to_string(line);
	res += "\nFunction : ";
	res += func;

	return res;
}
