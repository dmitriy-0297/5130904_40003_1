#include "DataStruct.h"
#include <sstream>
#include <cctype>
#include <algorithm>


namespace mynsp {
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::string readDigitsOrAlpha(std::istream& in, bool allowAlpha) {
        std::string str;
        char ch;
        while (in.get(ch)) {
            bool isDigit = std::isdigit(static_cast<unsigned char>(ch));
            bool isAlpha = allowAlpha && std::isalpha(static_cast<unsigned char>(ch));

            if (isDigit || isAlpha) {
                str += ch;
            } else {
                in.unget();
                break;
            }
        }
        return str;
    }

    std::istream& operator>>(std::istream& in, UllLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string str = readDigitsOrAlpha(in, true);

        if (str.size() < 4) { //суффикс и >0 цифр
            in.setstate(std::ios::failbit);
            return in;
        }

        std::string suffix = str.substr(str.size() - 3);
        for (char& c : suffix) {
            c = std::tolower(static_cast<unsigned char>(c));
        }
        if (suffix != "ull") {
            in.setstate(std::ios::failbit);
            return in;
        }

        str = str.substr(0, str.size() - 3);

        try {
            size_t pos = 0;
            dest.ref = std::stoull(str, &pos);
            if (pos != str.size()) {
                in.setstate(std::ios::failbit);
            }
        } catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, UllOctIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string str = readDigitsOrAlpha(in, false);

        try {
            size_t pos = 0;
            if (str.empty() || str[0] != '0') {
                in.setstate(std::ios::failbit);
                return in;
            }
            dest.ref = std::stoull(str, &pos, 8);
        } catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string data = "";
        if ((in >> StringIO{ data }) && (data != dest.exp)) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        DataStruct input;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        char c = '\0';
        while (in.get(c)) {
            if (c == '(') {
                break;
            }
        }
        if (!in) {
            in.setstate(std::ios::failbit);
            return in;
        }


        in >> std::ws; // пропускаем пробелы

        while (in >> c && c != ')') {
            if (c == ':') {
                std::string fieldName;
                char ch;
                while (in.get(ch)) {
                    if (ch == ' ' || ch == ':' || ch == ')') {
                        in.unget();
                        break;
                    }
                    fieldName += ch;
                }

                in >> std::ws;
                if (in.peek() == ')') { // после ':' больше нет полей
                    break;
                }

                if (fieldName.empty()) {
                    in.setstate(std::ios::failbit);
                    break;
                }

                if (!(in >> std::ws)) {
                    in.setstate(std::ios::failbit);
                    break;
                }

                if (fieldName == "key1") {
                    if (!(in >> UllLitIO{ input.key1 })) {
                        break;
                    }
                    hasKey1 = true;
                } else if (fieldName == "key2") {
                    if (!(in >> UllOctIO{ input.key2 })) {
                        break;
                    }
                    hasKey2 = true;
                } else if (fieldName == "key3") {
                    if (!(in >> StringIO{ input.key3 })) {
                        break;
                    }
                    hasKey3 = true;
                } else {
                    in.setstate(std::ios::failbit);
                    break;
                }
            }
        }

        if (in && hasKey1 && hasKey2 && hasKey3) {
            dest = input;
        } else {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) {
            return out;
        }

        iofmtguard fmtguard(out);
        out << "(:key1 " << src.key1 << "ull:key2 0" << std::oct
            << src.key2 << ":key3 \"" << src.key3 << "\":)";

        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags()) {}

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    bool compareDataStruct(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) {
            return a.key1 < b.key1;
        } else if (a.key2 != b.key2) {
            return a.key2 < b.key2;
        } else {
            return a.key3.length() < b.key3.length();
        }
    }
}
