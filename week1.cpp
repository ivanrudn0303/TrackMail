#include <iostream>
#include <windows.h>
#include <cstdint>

#define INFINITE_ROOTS 3
#define TWO_ROOTS 2
#define ONE_ROOT 1
#define NON_ROOTS 0
#define ROUNDING_ERROR -2
#define POINTER_ERROR -1
#define EPSILON DBL_EPSILON

//ax^2 + bx + c = 0
int SolveSquare(double, double, double, double*, double*);
//kx + b = 0
int SolveLinear(double, double, double*);

int main()
{
	double x1 = NAN, x2 = NAN, a = NAN, b = NAN, c = NAN;
	scanf_s("%lg %lg %lg", &a, &b, &c);
	int n = SolveSquare(a, b, c, &x1, &x2);
	switch (n)
	{
	case INFINITE_ROOTS:
		printf("Inf Roots\n");
		break;
	case TWO_ROOTS:
		printf("Two Roots: x1 = %lg, x2 = %lg\n", x1, x2);
		break;
	case ONE_ROOT:
		printf("One Root: x1 = %lg\n", x1);
		break;
	case NON_ROOTS:
		printf("Non Roots\n");
		break;
	default:
		printf("Error occured: %d\n", n);
		break;
	}
}


int SolveSquare(double A, double B, double C, double *pFirstRoot, double *pSecondRoot)
{
	if (A != 0.0)
	{
		if (pFirstRoot != nullptr && pSecondRoot != nullptr)
		{
			double D = B * B - 4 * A * C;
			if (D < 0.0)
				return NON_ROOTS;
			else if (D < EPSILON)
			{
				double Root = -B / 2.0 / A;
				if (std::abs(Root) < std::numeric_limits<double>::infinity())
				{
					*pFirstRoot = Root;
					return ONE_ROOT;
				}
				else
					return ROUNDING_ERROR;
			}
			else
			{
				if (std::abs(D) < std::numeric_limits<double>::infinity())
				{
					D = sqrt(D);
					double RootFirst = (-D - B) / 2.0 / A, RootSecond = (D - B) / 2.0 / A;
					if ((std::abs(RootFirst) < std::numeric_limits<double>::infinity()) && (std::abs(RootSecond) < std::numeric_limits<double>::infinity()))
					{
						*pFirstRoot = RootFirst;
						*pSecondRoot = RootSecond;
						return TWO_ROOTS;
					}
					else
						return ROUNDING_ERROR;
				}
				else
					return ROUNDING_ERROR;
			}
		}
		else
			return POINTER_ERROR;
	}
	else
		return SolveLinear(B, C, pFirstRoot);
}

int SolveLinear(double K, double B, double *pRoot)
{
	if (K != 0.0)
	{
		if (pRoot != nullptr)
		{
			double Root = -B / K;
			if (std::abs(Root) < std::numeric_limits<double>::infinity())
			{
				*pRoot = Root;
				return ONE_ROOT;
			}
			else
				return ROUNDING_ERROR;
		}
		else
			return POINTER_ERROR;
	}
	else
	{
		if (B == 0.0)
			return INFINITE_ROOTS;
		else
			return NON_ROOTS;
	}
}
