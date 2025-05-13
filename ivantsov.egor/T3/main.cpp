#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <tuple>          // std::tie

struct Point {
  int x = 0, y = 0;
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }
  bool operator< (const Point& o) const { return std::tie(x, y) < std::tie(o.x, o.y); }
};

struct Polygon {
  std::vector<Point> points;

  bool operator==(const Polygon& o) const { return points == o.points; }

  double area() const {
    if (points.size() < 3) return 0.0;
    long long sum = 0;
    for (size_t i = 0; i < points.size(); ++i) {
      const auto& p1 = points[i];
      const auto& p2 = points[(i + 1) % points.size()];
      sum += 1LL * p1.x * p2.y - 1LL * p2.x * p1.y;
    }
    return std::abs(sum) / 2.0;
  }
};

static bool isNumber(const std::string& s) {
  return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

static bool parsePoint(const std::string& tok, Point& p) {
  if (tok.size() < 5 || tok.front() != '(' || tok.back() != ')') return false;
  std::string inner = tok.substr(1, tok.size() - 2);
  std::replace(inner.begin(), inner.end(), ';', ' ');
  std::istringstream ss(inner);
  return static_cast<bool>(ss >> p.x >> p.y);
}

static bool parsePolygon(std::istringstream& iss, Polygon& poly) {
  size_t n;
  if (!(iss >> n) || n < 3) return false;
  poly.points.clear();
  std::string tok;
  for (size_t i = 0; i < n; ++i) {
    if (!(iss >> tok))            return false;
    Point p;
    if (!parsePoint(tok, p))      return false;
    poly.points.push_back(p);
  }
  return true;
}

static void cmdArea(const std::vector<Polygon>& v, const std::string& param) {
  if (v.empty()) { std::cout << "<INVALID COMMAND>\n"; return; }

  double res = 0.0;
  if (param == "EVEN" || param == "ODD") {
    bool even = param == "EVEN";
    for (const auto& p : v)
      if ((p.points.size() % 2 == 0) == even) res += p.area();
  }
  else if (param == "MEAN") {
    for (const auto& p : v) res += p.area();
    res /= v.size();
  }
  else if (isNumber(param)) {
    size_t N = std::stoul(param);
    if (N < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
    for (const auto& p : v)
      if (p.points.size() == N) res += p.area();
  }
  else {
    std::cout << "<INVALID COMMAND>\n"; return;
  }
  std::cout << std::fixed << std::setprecision(1) << res << '\n';
}

static void cmdCount(const std::vector<Polygon>& v, const std::string& param) {
  size_t cnt = 0;
  if (param == "EVEN" || param == "ODD") {
    bool even = param == "EVEN";
    cnt = std::count_if(v.begin(), v.end(),
      [even](const Polygon& p) { return (p.points.size() % 2 == 0) == even; });
  }
  else if (isNumber(param)) {
    size_t N = std::stoul(param);
    if (N < 3) { std::cout << "<INVALID COMMAND>\n"; return; }
    cnt = std::count_if(v.begin(), v.end(),
      [N](const Polygon& p) { return p.points.size() == N; });
  }
  else {
    std::cout << "<INVALID COMMAND>\n"; return;
  }
  std::cout << cnt << '\n';
}

static void cmdMax(const std::vector<Polygon>& v, const std::string& what) {
  if (v.empty()) { std::cout << "<INVALID COMMAND>\n"; return; }

  if (what == "AREA") {
    double best = 0.0;
    for (const auto& p : v) best = std::max(best, p.area());
    std::cout << std::fixed << std::setprecision(1) << best << '\n';
  }
  else if (what == "VERTEXES") {
    size_t best = 0;
    for (const auto& p : v) best = std::max(best, p.points.size());
    std::cout << best << '\n';
  }
  else
    std::cout << "<INVALID COMMAND>\n";
}

static void processCommand(const std::vector<Polygon>& v, const std::string& line) {
  std::istringstream iss(line);
  std::string w1, w2;
  iss >> w1 >> w2;

  if (w1 == "AREA")         cmdArea(v, w2);
  else if (w1 == "COUNT")   cmdCount(v, w2);
  else if (w1 == "MAX")     cmdMax(v, w2);
  else                      std::cout << "<INVALID COMMAND>\n";
}

int main(int argc, char* argv[]) {
  if (argc < 2) { std::cerr << "Error: filename not provided\n"; return EXIT_FAILURE; }

  std::ifstream fin(argv[1]);
  if (!fin) { std::cerr << "Error: cannot open file\n";   return EXIT_FAILURE; }

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
