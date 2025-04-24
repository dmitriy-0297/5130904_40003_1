#include "data_struct.hpp"
#include "score_guard.hpp"

namespace
{
    struct DelimetrIO
    {
        char exp;
    };

    struct UnsignedLongLongIO
    {
        unsigned long long& ref;
    };

    struct ComplexIO
    {
        std::complex<double>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    std::istream& operator>>(std::istream& in, DelimetrIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char ch = 0;
        in >> ch;
        if (in && (ch != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        in >> DelimetrIO{'0'}
           >> std::oct >> dest.ref
           >> std::dec >> DelimetrIO{':'};
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        double real = 0.0;
        double image = 0.0;

        in >> DelimetrIO{'#'} >> DelimetrIO{'c'} >> DelimetrIO{'('} >> real;
        in >> image >> DelimetrIO{')'} >> DelimetrIO{':'};

        if (in)
        {
            dest.ref = std::complex<double>(real, image);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimetrIO{'"'}, dest.ref, '"');
    }
}

std::istream& vurvaa::operator>>(std::istream& in, vurvaa::DataStruct& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    vurvaa::DataStruct input{};
    {
        using sep = DelimetrIO;
        using ull = UnsignedLongLongIO;
        using cmp = ComplexIO;
        using str = StringIO;
        in >> sep{'('} >> sep{':'};
        for (int i = 0; i < 3; i++)
        {
            std::string temp = "";
            in >> temp;
            if (temp == "key1")
            {
                in >> ull{input.key1};
            }
            else if (temp == "key2")
            {
                in >> cmp{input.key2};
            }
            else if (temp == "key3")
            {
                in >> str{input.key3} >> sep{':'};
            }
        }
        in >> sep{')'};
    }
    if (in)
    {
        dest = input;
    }
    return in;
}

std::ostream& vurvaa::operator<<(std::ostream& out, const vurvaa::DataStruct& dest)
{
    std::ostream::sentry sentry(out);
    if (!sentry)
    {
        return out;
    }

    iofmtguard fmtguard(out);

    out << "(:key1 0" << std::oct << dest.key1 << std::dec;
    out << ":key2 #c(" << std::fixed << std::setprecision(1)
        << dest.key2.real() << ' ' << dest.key2.imag() << ')';
    out << ":key3 " << '"' << dest.key3 << '"' << ":)";

    return out;
}

