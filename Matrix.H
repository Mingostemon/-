#ifndef _Matrix_H_IS_INCLUDED_
#define _Matrix_H_IS_INCLUDED_

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cmath>

#include "Vector.H"

class Matrix
{
	public :

		friend void luMatrix(const Matrix &A); 

		friend void gredientSolver(const Matrix A, const Vector b, Vector &x);

		friend void cgSolver(const Matrix A, const Vector b, Vector &x);
		friend Matrix operator * (const Matrix &A, const Matrix &B);

		friend Vector operator * (const Matrix &A, const Vector &u);

		friend std::ostream &operator << (std::ostream &, const Matrix &);

		friend const std::istream &operator >> (std::istream &, Matrix &);

	//------------------------------------------------------------------------------
		
	public :
		Matrix(void);
		virtual ~Matrix(void); //what means?

		Matrix(const Matrix &A);
		Matrix(int);

		const Matrix &operator = (const Matrix &A);

		int getRowNumber(void) const;

		Vector &operator [] (int i);
		const Vector &operator [](int i) const;
		
		Vector getColumnVector(int j) const;

		int getDimension(void) const;

		void resetDimension(int n);
		void setDimension(int n);
	
	//------------------------------------------------------------------------------
	private :
		int _n;
		Vector *_row;
};

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline void Matrix::setDimension(int n)
{
	if (n <= 0) {
		std::cout << "n = " << n << ", invalid matrix dimension." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (n == _n) {
		assert(0 != _row);//_row is never 0; 
		return;
	}

	if (0 != _row) {
		delete[] _row;
	}

	_n = n;
	_row = new Vector[_n];
	for (int i = 0; i < _n; i++) {
		_row[i].setDimension(_n);
	}
}
			
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline void Matrix::resetDimension(int n)
{
	setDimension(n);
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Vector &Matrix::operator [] (int i)
{
	return _row[i];
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline const Vector &Matrix::operator [] (int i) const
{
	return _row[i];
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline int Matrix::getDimension(void) const
{
	return _n;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Matrix::Matrix(void)
{
	_n = 0;
	_row = 0;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Matrix::~Matrix(void)
{
	if (0 != _row) {
		delete[] _row;
		_row = 0;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Matrix::Matrix(int n)
{
	if (n <= 0) {
		std::cout << "n = " << n << ", invalid matrix dimension." << std::endl;
		exit(EXIT_FAILURE);
	}

	_n = n;
	_row = new Vector[n];
	for (int i = 0; i < n; i++) {
		_row[i].setDimension(n);
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Matrix::Matrix(const Matrix &A)
{
	_n = A._n;
	_row = 0;
	if (A._row != 0) {
		if (A._n <= 0) {
			std::cout  << "Invalid matrix dimension found." << std::endl;
			exit(EXIT_FAILURE);
		}

		_row = new Vector[A._n];
		for (int i = 0; i < A._n; i++) {
			_row[i] = A._row[i];
		}
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline const Matrix &Matrix::operator = (const Matrix &A)
{
	if (this != &A) {
		if (_n != A._n) {
			delete [] _row;
			_row = 0;
		}

		_row = new Vector[A._n];
		for (int i = 0; i < A._n; i++) {
			_row[i].setDimension(A._n);
		}
		_n = A._n;
	}

	if (A._row != 0) {
		if (_n <=0) {
			std::cout  << "Invalid matrix dimension found." << std::endl;
			exit(EXIT_FAILURE);
		}

		if (_row == 0) {
			std::cout  << "The row pointer has not been initialized yet." << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < A._n; i++) {
			_row[i] = A._row[i];
		}
	}

	return *this;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Vector Matrix::getColumnVector(int j) const
{
	Vector a(_n);

	if ((j < 0) || (j >= _n)) {
			std::cout  << "Invalid matrix dimension found." << std::endl;
			exit(EXIT_FAILURE);
		}

	for (int i = 0; i < _n; i++) {
		a[i] = _row[i][j];
	}

	return a;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Vector operator * (const Matrix &A, const Vector &u)
{
	int n = u.getDimension();

	Vector v(n);
	for (int i = 0; i < n; i++) {
		double sum = 0.0;
		for (int j = 0; j < n; j++) {
			sum += A._row[i]._x[j] * u._x[j];
		}
		v[i] = sum;
	}

	return v;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline Matrix operator * (const Matrix &A, const Matrix &B)
{
	int na = A._n;
	int nb = B._n;

	if (na != nb) {
		std::cout  << "Unequal matrix dimensions found."
							 << "(na, nb) = (" << na << ", " << nb << ") " << std::endl;
		exit(EXIT_FAILURE);
	}

	int n = na;

	Matrix C(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			double sum = 0.0;
			for (int k = 0; k < n; k++) {
				sum += A._row[i]._x[k] * B._row[k]._x[j];
			}
			C._row[i]._x[j] = sum;
		}
	}

	return C;
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline void luMatrix(Matrix &A)
{
	int n = A.getDimension();
	std::cout  << "n = " << n << std::endl;
	for (int k = 0; k < n-1; k++) {
		if (fabs(A[k][k]) < EPSILON12) {
		  std::cout  << "k = "<< k << ", " << fabs(A[k][k])  <<std::endl;
			std::cout << "Null pivot element" << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = k+1; i < n; i++) {
			A[i][k] = A[i][k] / A[k][k];
		}
		for (int j = k+1; j < n; j++) {
			for (int i = k+1; i < n; i++) {
				A[i][j] = A[i][j] - A[i][k] * A[k][j];
			}
		}
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void gredientSolver(const Matrix A, const Vector b, Vector &x)
{
	int n = b.getDimension();
	Vector r(n), q(n);
	double relres = 0.0;
	double rho, alpha;
	int flag = 0;

	r = b - A*x;
	relres = r.computeL2Norm(n);
	if (relres < EPSILON12) {
		return;
	}
	
	for (int i = 0; i < iter; i++) {
		rho = r * r;
		q = A * r;
		alpha = rho / (r * q);
		x = x + alpha * r;
		r = r - alpha * q;
		relres = r.computeL2Norm(n) / b.computeL2Norm(n);
		if (fabs(relres) < EPSILON12) {
			flag = 1;
			return;
		}
	}

	if (flag == 0) {
		std::cout << "After " << iter << " iteration, we don't find the suitable solution yet." << std::endl;
	}
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
void cgSolver(const Matrix A, const Vector b, Vector &x)
{
	int n = b.getDimension();
	Vector r(n), q(n), p(n);
	double relres = 0.0;
	double rho, alpha, beta;
	int flag = 0;

	r = b - A*x;
	p = r;
	relres = r.computeL2Norm(n);
	if (relres < EPSILON12) {
		return;
	}
	
	for (int i = 0; i < iter; i++) {
		rho = p * r;
		q = A * p;
		alpha = rho / (p * q);
		x = x + alpha * p;
		r = r - alpha * q;
		beta = (q * r) / (p * q);
		p = r - beta * p;
		relres = r.computeL2Norm(n) / b.computeL2Norm(n);
		if (fabs(relres) < EPSILON12) {
			flag = 1;
			return;
		}
	}

	if (flag == 0) {
		std::cout << "After " << iter << " iteration, we don't find the suitable solution yet." << std::endl;
	}
}
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline std::ostream &operator << (std::ostream &os, const Matrix &A)
{
	os << "\n";
	os << "  ";
	for (int i = 0; i < A._n; i++) {
		if (i > 0) {
			os << ",\n  ";
		}
		os << A._row[i];
	}
	os << std::endl;
	return os;
}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
inline const std::istream &operator >> (std::istream &is, Matrix &A)
{
	int n = 0;
	is >> n;
	if (n <= 0) {
			std::cout  << "Invalid matrix dimension found." << std::endl;
			exit(EXIT_FAILURE);
	}

	A.setDimension(n);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			is >> A._row[i]._x[j];
		}
	}

	return is;
}

#endif

//==============================================================================

int main(int argc, char *argv[])
{
/*	Matrix A;
	std::cout << "Matrix() is: " << A << std::endl;

	Matrix B(5);
	std::cout <<"Matrix B(5) is: " << "\n" << B << std::endl;
	std::cout << "dimension of B is: " << B.getDimension() << ", " 
						<< std::endl;
	Matrix C;
	C.setDimension(5);
	std::cout << "dimension of C is: " << C.getDimension() << std::endl;

	Matrix E(3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			E[i][j] = 1.0/(i+j+1);
		}
	}

	std::cout << "E is : " << E <<std::endl;


	Matrix D;
	std::cin >> D;
	std::cout << " Matrix D is: " << D << std::endl;
	
	std::cout << "D[0] is: " << D[0] << std::endl;
	std::cout << "D[0][1] is: " << D[0][1] << std::endl;
*/

	Matrix A(3);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			A[i][j] = 1.0/(i+j+1);
		}
		A[i][i] = 4 * i + 2;
	}
	std::cout << A << std::endl;

	Vector b(3), x(3), y(3);
	for (int i = 0; i < 3; i++) {
		b[i] = i+1;
	}

	gredientSolver(A,b,x);
	std::cout << "Gredient Method of Ax = b is: " << x << std::endl; 
	std::cout << "Time used = " << (double)clock()/CLOCKS_PER_SEC << std::endl;

	cgSolver(A,b,y);
	std::cout << "Conjugate Gredient Metohd of Ay = b is: " << y << std::endl; 
	std::cout << "Time used = " << (double)clock()/CLOCKS_PER_SEC << std::endl;

	luMatrix(A);

	std::cout << "Lu frac of A is: " << A  << std::endl;



	return 0;
}


