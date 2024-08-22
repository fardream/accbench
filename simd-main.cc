#include <benchmark/benchmark.h>
#include <math.h>

#include <random>
#include <vector>

#if defined(USE_MKL)

#include <mkl.h>

#elif __X86_64__

#if defined(__cplusplus)
extern "C" {
#endif

inline void vdSqrt(const int n, const double a[], double r[]) {
  for (int i = 0; i < n; i++) {
    r[i] = sqrt(a[i]);
  }
}
inline void vdPowx(const int n, const double a[], const double b, double r[]) {
  for (int i = 0; i < n; i++) {
    r[i] = pow(a[i], b);
  }
}

inline void vdLog1p(const int n, const double a[], double r[]) {
  for (int i = 0; i < n; i++) {
    r[i] = log1p(a[i]);
  }
}

#if defined(__cplusplus)
}
#endif

#else

#include <amath.h>

#if defined(__cplusplus)
extern "C" {
#endif

#define DOUBLESTEP 2

inline void vdSqrt(const int n, const double a[], double r[]) {
  const int nstep = n / DOUBLESTEP;
  const int end = nstep * DOUBLESTEP;
  for (int i = 0; i < end; i += DOUBLESTEP) {
    float64x2_t input = vld1q_f64(a + i);
    float64x2_t output = vsqrtq_f64(input);
    vst1q_f64(r + i, output);
  }

  for (int i = end; i < n; i++) {
    float64x1_t input = vld1_f64(a + i);
    float64x1_t output = vsqrt_f64(input);
    vst1_f64(r + i, output);
  }
}

inline void vdPowx(const int n, const double a[], const double b, double r[]) {
  const int nstep = n / DOUBLESTEP;
  const int end = nstep * DOUBLESTEP;

  float64x2_t bptr = vmovq_n_f64(b);

  for (int i = 0; i < end; i += DOUBLESTEP) {
    float64x2_t input = vld1q_f64(a + i);
    float64x2_t output = _ZGVnN2vv_pow(input, bptr);
    vst1q_f64(r + i, output);
  }

  for (int i = end; i < n; i++) {
    r[i] = pow(a[i], b);
  }
}

inline void vdLog1p(const int n, const double a[], double r[]) {
  const int nstep = n / DOUBLESTEP;
  const int end = nstep * DOUBLESTEP;
  for (int i = 0; i < end; i += DOUBLESTEP) {
    float64x2_t input = vld1q_f64(a + i);
    float64x2_t output = _ZGVnN2v_log1p(input);
    vst1q_f64(r + i, output);
  }

  for (int i = end; i < n; i++) {
    r[i] = log1p(a[i]);
  }
}

#if defined(__cplusplus)
}
#endif

#endif

class RandomVectorBM : public benchmark::Fixture {
public:
  std::vector<std::vector<double>> data;

  void SetUp(::benchmark::State &state) {
    std::mt19937 gen(0);
    std::uniform_real_distribution<double> absdis(1.0, 15.0);

    for (size_t n = 9000; n < 10001; n++) {
      std::vector<double> x(n, 0);
      double a = absdis(gen);
      std::uniform_real_distribution<double> thisd(-a, a);
      for (size_t i = 0; i < n; i++) {
        x[i] = fabs(thisd(gen));
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
      vdPowx(v.size(), v.data(), 0.7, result.data());
    }
  }
}

BENCHMARK_REGISTER_F(RandomVectorBM, POWX);

BENCHMARK_DEFINE_F(RandomVectorBM, LOG1P)(benchmark::State &state) {
  for (auto _ : state) {
    for (const auto &v : this->data) {
      state.PauseTiming();
      std::vector<double> result(v.size(), 0);
      state.ResumeTiming();
      vdLog1p(v.size(), v.data(), result.data());
    }
  }
}

BENCHMARK_REGISTER_F(RandomVectorBM, LOG1P);

BENCHMARK_MAIN();
