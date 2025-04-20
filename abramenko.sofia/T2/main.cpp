#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <iomanip>

//(:key1 89ull:key2 0b1000101:key3 "Data":)

namespace nspace
{
	struct DataStruct {
		unsigned long long key1 = 0;
		unsigned long long key2 = 0;
		std::string key3;
	};

	struct Delimiter {
		std::string exp;
	};

	struct UllLit {
		unsigned long long& ref;
	};

	struct UllBin {
		unsigned long long& ref;
	};

	struct String {
		std::string& ref;
	};

	struct Label {
		std::string exp;
	};

	std::istream& operator>>(std::istream& in, const Delimiter& dest);
	std::istream& operator>>(std::istream& in, UllLit&& dest);
	std::istream& operator>>(std::istream& in, UllBin&& dest);
	std::istream& operator>>(std::istream& in, String&& dest);
	std::istream& operator>>(std::istream& in, Label&& dest);
	std::istream& operator>>(std::istream& in, DataStruct& dest);
	std::ostream& operator<<(std::ostream& out, const DataStruct& dest);

	bool different(const DataStruct& a, const DataStruct& b) {
		if (a.key1 != b.key1) return a.key1 < b.key1;
		if (a.key2 != b.key2) return a.key2 < b.key2;
		return a.key3.size() < b.key3.size();
	}
}

int main()
{
	//(:key1 89ull:key2 0b1000101:key3 "Data":)

	using nspace::DataStruct;

	std::vector<DataStruct> data;

	std::istringstream iss("(:key1 89ull:key2 0b1000101:key3 \"Data\":)(:key2 0b110:key1 42ull:key3 \"Another Data\":)");

	std::copy(
		std::istream_iterator<DataStruct>(iss),
		std::istream_iterator<DataStruct>(),
		std::back_inserter(data)
	);

	std::sort(data.begin(), data.end(), nspace::different);

	std::cout << "Data:\n";

	std::copy(
		data.begin(),
		data.end(),
		std::ostream_iterator<DataStruct>(std::cout, "\n")
	);

	return 0;
}

namespace nspace
{
	std::istream& operator>>(std::istream& in, const Delimiter& dest) {
		std::istream::sentry sentry(in);
		if (!sentry) {
			return in;
		}
		std::string input;
		in >> input;
		if (in && (input != dest.exp)) {
			in.setstate(std::ios::failbit);
		}
		return in;
	}

	std::istream& operator>>(std::istream& in, UllLit&& dest) {
		std::istream::sentry sentry(in);
		if (!sentry)
		{
			return in;
		}
		return in >> dest.ref >> Delimiter{ "ull" };
	}

	std::istream& operator>>(std::istream& in, UllBin&& dest) {
		std::istream::sentry sentry(in);
		if (!sentry) {
			return in;
		}

		Delimiter delimiter{ "0b" };

		if (!(in >> delimiter)) {
			return in;
		}

		std::string binaryStr;
		in >> binaryStr;

		try {
			dest.ref = std::stoull(binaryStr, nullptr, 2);
		}
		catch (const std::invalid_argument&) {
			in.setstate(std::ios::failbit);
		}
		catch (const std::out_of_range&) {
			in.setstate(std::ios::failbit);
		}

		return in;
	}

	std::istream& operator>>(std::istream& in, String&& dest) {
		std::istream::sentry sentry(in);
		if (!sentry) {
			return in;
		}

		Delimiter delimiter{ "\"" };

		if (!(in >> delimiter)) {
			return in;
		}

		std::getline(in, dest.ref, '"');

		return in;
	}

	std::istream& operator>>(std::istream& in, Label&& dest) {
		std::istream::sentry sentry(in);
		if (!sentry)
		{
			return in;
		}
		std::string data = "";
		if ((in >> String{ data }) && (data != dest.exp))
		{
			in.setstate(std::ios::failbit);
		}
		return in;
	}

	std::istream& operator>>(std::istream& in, DataStruct& dest) {
		std::istream::sentry sentry(in);
		if (!sentry) {
			return in;
		}

		DataStruct temp;
		char c;
		in >> Delimiter{ "(:" };

		std::string key;
		while (in >> key && key != ":)") {
			if (key == "key1") {
				in >> UllLit{ temp.key1 } >> Delimiter{ ":" };
			}
			else if (key == "key2") {
				in >> UllBin{ temp.key2 } >> Delimiter{ ":" };
			}
			else if (key == "key3") {
				in >> String{ temp.key3 } >> Delimiter{ ":" };
			}
			else {
				in.setstate(std::ios::failbit);
				return in;
			}
		}

		if (in) {
			dest = temp;
		}
		return in;
	}

	std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
		std::ostream::sentry sentry(out);
		if (!sentry)
		{
			return out;
		}

		//(:key1 89ull:key2 0b1000101:key3 "Data":)

		out << "(:";
		out << "key1 " << src.key1 << "ull";
		out << ":key2 " << "0b" << src.key2;
		out << ":key3 " << src.key3;
		out << ":)";
		return out;
	}
}