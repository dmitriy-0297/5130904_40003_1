#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <tuple>        // ← для std::tie

struct Point {
  int x = 0, y = 0;

  bool operator==(const Point& other) const {
    return x == other.x && y == other.y;
  }
  bool operator<(const Point& other) const {
    return std::tie(x, y) < std::tie(other.x, other.y);
  }
};

struct Polygon {
  std::vector<Point> points;

  bool operator==(const Polygon& other) const {
    return points == other.points;
  }
  double area() const {
    if (points.size() < 3) return 0.0;
    long long sum = 0;
    for (size_t i = 0; i < points.size(); ++i) {
      const auto& p1 = points[i];
      const auto& p2 = points[(i + 1) % points.size()];
      sum += static_cast<long long>(p1.x) * p2.y -
        static_cast<long long>(p2.x) * p1.y;
    }
    return std::abs(sum) / 2.0;
  }
};

static bool isNumber(const std::string& str) {
  return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

static bool parseBool(const std::string& str) {
  return str == "true" || str == "1";
}

static bool parsePoint(const std::string& token, Point& point) {
  if (token.size() < 5 || token.front() != '(' || token.back() != ')') return false;
  std::string inner = token.substr(1, token.size() - 2);
  std::replace(inner.begin(), inner.end(), ';', ' ');
  std::istringstream ss(inner);
  return static_cast<bool>(ss >> point.x >> point.y);   // явное преобразование
}

static bool parsePolygon(std::istringstream& iss, Polygon& polygon) {
  size_t n;
  if (!(iss >> n) || n < 3) return false;
  polygon.points.clear();
  std::string tok;
  for (size_t i = 0; i < n; ++i) {
    if (!(iss >> tok)) return false;
    Point p;
    if (!parsePoint(tok, p)) return false;
    polygon.points.push_back(p);
  }
  return true;
}

static void handleArea(const std::vector<Polygon>& polys, const std::string& param) {
  double res = 0.0;
  if (param == "EVEN" || param == "ODD") {
    bool needEven = param == "EVEN";
    for (const auto& p : polys)
      if ((p.points.size() % 2 == 0) == needEven) res += p.area();
  }
  else if (param == "MEAN" && !polys.empty()) {
    for (const auto& p : polys) res += p.area();
    res /= polys.size();
  }
  std::cout << std::fixed << std::setprecision(1) << res << '\n';
}

static void handleCount(const std::vector<Polygon>& polys, const std::string& param) {
  size_t cnt = 0;
  if (param == "EVEN" || param == "ODD") {
    bool needEven = param == "EVEN";
    cnt = std::count_if(polys.begin(), polys.end(),
      [needEven](const Polygon& p) { return (p.points.size() % 2 == 0) == needEven; });
  }
  else if (isNumber(param)) {
    size_t v = std::stoul(param);
    cnt = std::count_if(polys.begin(), polys.end(),
      [v](const Polygon& p) { return p.points.size() == v; });
  }
  std::cout << cnt << '\n';
}

static void processCommand(const std::vector<Polygon>& polys, const std::string& line) {
  std::istringstream iss(line);
  std::string cmd, param;
  iss >> cmd >> param;
  if (cmd == "AREA" && !param.empty())
    handleArea(polys, param);
  else if (cmd == "COUNT" && !param.empty())
    handleCount(polys, param);
  else
    std::cout << "<INVALID COMMAND>\n";
}

int main(int argc, char* argv[]) {
  if (argc < 2) { std::cerr << "Error: filename not provided\n"; return EXIT_FAILURE; }

  std::ifstream fin(argv[1]);
  if (!fin) { std::cerr << "Error: cannot open file\n"; return EXIT_FAILURE; }

  std::vector<Polygon> polys;
  std::string line;
  while (std::getline(fin, line)) {
    std::istringstream iss(line);
    Polygon p;
    if (parsePolygon(iss, p)) polys.push_back(std::move(p));
  }

  while (std::getline(std::cin, line))
    processCommand(polys, line);

  return EXIT_SUCCESS;
}
