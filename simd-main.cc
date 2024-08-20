#include <benchmark/benchmark.h>
#include <math.h>

#include <random>
#include <vector>

#if defined(__X86_64__) || defined(USE_MKL)

#include <mkl.h>

#else

void vdSqrt(const int n, const double a[], double r[]) {
  for (int i = 0; i < n; i++) {
    r[i] = sqrt(a[i]);
  }
}
void vdPowx(const int n, const double a[], const double b, double r[]) {
  for (int i = 0; i < n; i++) {
    r[i] = pow(a[i], b);
  }
}

#endif

class RandomVectorBM : public benchmark::Fixture {
public:
  std::vector<std::vector<double>> data;

  void Setup(::benchmark::State &state) {
    std::mt19937 gen(0);
    std::uniform_real_distribution<double> absdis(1.0, 15.0);

    for (size_t n = 900; n < 1001; n++) {
      std::vector<double> x(n, 0);
      double a = absdis(gen);
      std::uniform_real_distribution<double> thisd(-a, a);
      for (size_t i = 0; i < n; i++) {
        x[i] = thisd(gen);
      }
      this->data.push_back(x);
    }
  }
};

BENCHMARK_DEFINE_F(RandomVectorBM, SQRT)(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &v : this->data) {
      state.PauseTiming();
      std::vector<double> result(v.size(), 0);
      state.ResumeTiming();
      vdSqrt(v.size(), v.data(), result.data());
    }
  }
}

BENCHMARK_REGISTER_F(RandomVectorBM, SQRT);

BENCHMARK_DEFINE_F(RandomVectorBM, POWX)(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &v : this->data) {
      state.PauseTiming();
      std::vector<double> result(v.size(), 0);
      state.ResumeTiming();
      vdPowx(v.size(), v.data(), 0.5, result.data());
    }
  }
}

BENCHMARK_REGISTER_F(RandomVectorBM, POWX);

BENCHMARK_MAIN();
