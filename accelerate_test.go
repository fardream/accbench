package accbench

import (
	"fmt"
	"testing"
)

func setup(n int) ([]float64, []float64) {
	r := make([]float64, n*n)
	m := make([]float64, n*n)
	v := make([]float64, n)

	for i := 0; i < n; i++ {
		for j := 0; j < i+1; j++ {
			if i == j {
				r[i*n+j] = (float64(i) + 1)
			} else {
				r[i*n+j] = 0.1 * float64(j)
			}
		}

		v[i] = float64(i)
	}

	LinAlgOps{}.CblasDgemm(CblasRowMajor, CblasNoTrans, CblasTrans, n, n, n, 1, r, n, r, n, 0, m, n)

	return m, v
}

func BenchmarkLinAlgOps(b *testing.B) {
	for _, i := range []int{10, 100, 1000} {
		i := i
		m, v := setup(i)
		b.Run(
			fmt.Sprintf("gemv-%d", i),
			func(b *testing.B) {
				for k := 0; k < b.N; k++ {
					r := make([]float64, i)
					LinAlgOps{}.CblasDgemv(CblasRowMajor, CblasNoTrans, i, i, 1, m, i, v, 1, 0, r, 1)
				}
			})

		b.Run(
			fmt.Sprintf("symv-%d", i),
			func(b *testing.B) {
				for k := 0; k < b.N; k++ {
					r := make([]float64, i)
					LinAlgOps{}.CblasDsymv(CblasRowMajor, CblasLower, i, 1, m, i, v, 1, 0, r, 1)
				}
			})

		b.Run(
			fmt.Sprintf("potrf-%d", i),
			func(b *testing.B) {
				for k := 0; k < b.N; k++ {
					r := make([]float64, i*i)
					copy(r, m)
					LinAlgOps{}.Dpotrf(uint8('U'), i, r, i)
				}
			})
	}
}
