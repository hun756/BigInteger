#ifndef BIGINT_EXCEPT_HPP
#define BIGINT_EXCEPT_HPP

#include <string>
#include <stdexcept>

namespace BigInt
{
    namespace Except
    {
        class DivideZeroException : virtual public std::exception
        {
        public:
            DivideZeroException(std::string message)
                : msg(message)
            {
            }

            virtual const char *what() const throw()
            {
                return msg.c_str();
            }

        protected:
            std::string msg;
        };
    }

} // namespace BigInt

#endif /* end of include guard :  BIGINT_EXCEPT_HPP */
