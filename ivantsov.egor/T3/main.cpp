#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <tuple>   // std::tie

struct Point {
  int x = 0, y = 0;
  bool operator==(const Point& o) const { return x == o.x && y == o.y; }
  bool operator<(const Point& o)  const { return std::tie(x, y) < std::tie(o.x, o.y); }
};

using Segment = std::pair<Point, Point>;

static long long cross(const Point& a, const Point& b, const Point& c) {
  return 1LL * (b.x - a.x) * (c.y - a.y) - 1LL * (b.y - a.y) * (c.x - a.x);
}
static bool onSegment(const Point& a, const Point& b, const Point& p) {
  return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
    std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y) &&
    cross(a, b, p) == 0;
}
static bool segInter(const Segment& s1, const Segment& s2) {
  const auto& a = s1.first;  const auto& b = s1.second;
  const auto& c = s2.first;  const auto& d = s2.second;

  long long d1 = cross(a, b, c);
  long long d2 = cross(a, b, d);
  long long d3 = cross(c, d, a);
  long long d4 = cross(c, d, b);

  if (d1 == 0 && onSegment(a, b, c)) return true;
  if (d2 == 0 && onSegment(a, b, d)) return true;
  if (d3 == 0 && onSegment(c, d, a)) return true;
  if (d4 == 0 && onSegment(c, d, b)) return true;

  return ((d1 > 0) ^ (d2 > 0)) && ((d3 > 0) ^ (d4 > 0));
}

struct Polygon {
  std::vector<Point> pts;

  bool operator==(const Polygon& o) const { return pts == o.pts; }

  bool simple() const {
    size_t n = pts.size();
    for (size_t i = 0; i < n; ++i) {
      Segment s1{ pts[i], pts[(i + 1) % n] };
      for (size_t j = i + 1; j < n; ++j) {
        if (j == i || (j + 1) % n == i) continue;
        if (i == 0 && j + 1 == n) continue;
        Segment s2{ pts[j], pts[(j + 1) % n] };
        if (segInter(s1, s2)) return false;
      }
    }
    return true;
  }

  double area() const {
    if (pts.size() < 3) return 0.0;
    long long s = 0;
    for (size_t i = 0; i < pts.size(); ++i) {
      const auto& p1 = pts[i];
      const auto& p2 = pts[(i + 1) % pts.size()];
      s += 1LL * p1.x * p2.y - 1LL * p2.x * p1.y;
    }
    return std::abs(s) / 2.0;
  }
};

static bool isNumber(const std::string& s)
{
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
  poly.pts.clear();
  std::string tok;
  for (size_t i = 0;i < n;++i) {
    if (!(iss >> tok)) return false;
    Point pt;
    if (!parsePoint(tok, pt)) return false;
    poly.pts.push_back(pt);
  }
  return poly.simple();
}

static void cmdArea(const std::vector<Polygon>& v, const std::string& p) {
  double res = 0.0;
  if (p == "EVEN" || p == "ODD") {
    bool even = p == "EVEN";
    for (auto& pl : v) if ((pl.pts.size() % 2 == 0) == even) res += pl.area();
    std::cout << std::fixed << std::setprecision(1) << res << '\n';
  }
  else if (p == "MEAN") {
    if (v.empty()) { std::cout << "<INVALID COMMAND>\n";return; }
    for (auto& pl : v)res += pl.area();
    res /= v.size();
    std::cout << std::fixed << std::setprecision(1) << res << '\n';
  }
  else if (isNumber(p)) {
    size_t N = std::stoul(p);
    if (N < 3) { std::cout << "<INVALID COMMAND>\n";return; }
    for (auto& pl : v) if (pl.pts.size() == N) res += pl.area();
    std::cout << std::fixed << std::setprecision(1) << res << '\n';
  }
  else std::cout << "<INVALID COMMAND>\n";
}

static void cmdCount(const std::vector<Polygon>& v, const std::string& p) {
  size_t cnt = 0;
  if (p == "EVEN" || p == "ODD") {
    bool even = p == "EVEN";
    cnt = std::count_if(v.begin(), v.end(),
      [even](const Polygon& pl) {return (pl.pts.size() % 2 == 0) == even;});
  }
  else if (isNumber(p)) {
    size_t N = std::stoul(p);
    if (N < 3) { std::cout << "<INVALID COMMAND>\n";return; }
    cnt = std::count_if(v.begin(), v.end(),
      [N](const Polygon& pl) {return pl.pts.size() == N;});
  }
  else { std::cout << "<INVALID COMMAND>\n";return; }
  std::cout << cnt << '\n';
}

static void cmdMax(const std::vector<Polygon>& v, const std::string& w) {
  if (v.empty()) { std::cout << "<INVALID COMMAND>\n";return; }
  if (w == "AREA") {
    double best = 0.0; for (auto& pl : v) best = std::max(best, pl.area());
    std::cout << std::fixed << std::setprecision(1) << best << '\n';
  }
  else if (w == "VERTEXES") {
    size_t best = 0; for (auto& pl : v) best = std::max(best, pl.pts.size());
    std::cout << best << '\n';
  }
  else std::cout << "<INVALID COMMAND>\n";
}

static void processCmd(const std::vector<Polygon>& v, const std::string& line) {
  std::istringstream iss(line);
  std::string w1, w2; iss >> w1 >> w2;
  if (w1 == "AREA")  cmdArea(v, w2);
  else if (w1 == "COUNT") cmdCount(v, w2);
  else if (w1 == "MAX")   cmdMax(v, w2);
  else std::cout << "<INVALID COMMAND>\n";
}

int main(int argc, char* argv[]) {
  if (argc < 2) { std::cerr << "Error: filename not provided\n";return EXIT_FAILURE; }
  std::ifstream in(argv[1]);
  if (!in) { std::cerr << "Error: cannot open file\n";return EXIT_FAILURE; }

  std::vector<Polygon> polys;
  std::string line;
  while (std::getline(in, line)) {
    std::istringstream iss(line);
    Polygon pl;
    if (parsePolygon(iss, pl)) polys.push_back(std::move(pl));
  }

  while (std::getline(std::cin, line))
    processCmd(polys, line);

  return EXIT_SUCCESS;
}
