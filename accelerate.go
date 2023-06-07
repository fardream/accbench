package accbench

// #cgo CFLAGS: -mmacosx-version-min=13.3 -std=c17 -DACCELERATE_NEW_LAPACK
// #cgo LDFLAGS: -framework Accelerate -mmacosx-version-min=13.3
//
// #include <Accelerate/Accelerate.h>
import "C"

type LinAlgOps struct{}

const (
	CblasRowMajor = C.CblasRowMajor
	CblasColMajor = C.CblasColMajor
	CblasTrans    = C.CblasTrans
	CblasNoTrans  = C.CblasNoTrans
	CblasLower    = C.CblasLower
	CblasUpper    = C.CblasUpper
)

func (ops LinAlgOps) CblasDgemv(layout uint32, transpoe uint32, m, n int, alpha float64, a []float64, lda int, x []float64, incx int, beta float64, y []float64, incy int) {
	C.cblas_dgemv(
		layout,
		transpoe,
		C.int(m),
		C.int(n),
		C.double(alpha),
		(*C.double)(&a[0]),
		C.int(lda),
		(*C.double)(&x[0]),
		C.int(incx),
		C.double(beta),
		(*C.double)(&y[0]),
		C.int(incy),
	)
}

func (ops LinAlgOps) CblasDgemm(layout uint32, transa uint32, transb uint32, m int, n int, k int, alpha float64, a []float64, lda int, b []float64, ldb int, beta float64, c []float64, ldc int) {
	C.cblas_dgemm(
		layout,
		transa,
		transb,
		C.int(m),
		C.int(n),
		C.int(k),
		C.double(alpha),
		(*C.double)(&a[0]),
		C.int(lda),
		(*C.double)(&b[0]),
		C.int(ldb),
		C.double(beta),
		(*C.double)(&c[0]),
		C.int(ldc),
	)
}

func (ops LinAlgOps) Dpotrf(uplo uint8, n int, a []float64, lda int) int {
	var _r C.int
	_uplo := C.char(uplo)
	_n := C.int(n)
	_lad := C.int(lda)
	C.dpotrf_(&_uplo, &_n, (*C.double)(&a[0]), &_lad, &_r)
	return int(_r)
}

func (ops LinAlgOps) CblasDsymv(layout uint32, uplo uint32, n int, alpha float64, a []float64, lda int, x []float64, incx int, b float64, y []float64, incy int) {
	C.cblas_dsymv(
		layout,
		uplo,
		C.int(n),
		C.double(alpha),
		(*C.double)(&a[0]),
		C.int(lda),
		(*C.double)(&x[0]),
		C.int(incx),
		(C.double)(b),
		(*C.double)(&y[0]),
		C.int(incy),
	)
}
