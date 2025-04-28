#ifndef SCORE_GUARD_HPP
#define SCORE_GUARD_HPP

#include <iomanip>

namespace vurvaa
{
    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();

    private:
        std::basic_ios<char>& s_;
        char fill_;
        std::streamsize width_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };
}

#endif
