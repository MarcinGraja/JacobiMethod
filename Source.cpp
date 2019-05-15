#include <iostream>
#include <fstream>
class matrix
{
	int rows;
	int columns;
	double **data;;
public:
	matrix() {}
	matrix(int size): matrix(size,size){}
	matrix(int rows, int columns)
	{
		this->rows = rows;
		this->columns = columns;
		data = new double*[rows];
		for (int i = 0; i < rows; i++)
		{
			data[i] = new double[columns];
			for (int j = 0; j < columns; j++) 
			{
				data[i][j] = 0;
			}
		}
	}
	bool loadData()
	{
		std::ifstream input;
		input.open("data.txt");
		if (input.good())
		{
			input >> rows;
			columns = rows + 1;
		}
		else
		{
			return false;

		}
		data = new double *[getRows()];
		for (int i = 0; i < getRows(); i++)
		{
			data[i] = new double[columns];
		}
		int i = 0, j = 0;

		while (input.good())
		{
			input >> data[i][j];
			if (j == getRows())
			{
				i++;
				j = 0;
			}
			else
			{
				j++;
			}
		}
		return true;
	}
	double **getData() const
	{
		return data;
	}
	int getRows() const
	{
		return rows;
	}
	int getColumns() const
	{
		return columns;
	}
	void print(const char s[]) const
	{
		std::cout << s << "\n";
		for (int i = 0; i < getRows(); i++)
		{
			for (int j = 0; j < getColumns(); j++)
			{
				std::cout << data[i][j] << "\t";
			}
			std::cout << "\n";
		}
		std::cout << "\n";
	}
	matrix** generateSubmatrices() const
	{
		matrix* lowerAndUpper = new matrix(getRows());
		matrix* inversedDiagonal = new matrix(getRows());
		for (int i = 0; i < getRows(); i++)
		{
			for (int j = 0; j < getRows(); j++)
			{
				lowerAndUpper->getData()[i][j] = getData()[i][j];
				if (i == j) lowerAndUpper->getData()[i][j] = 0;
			}
			inversedDiagonal->getData()[i][i] = 1/getData()[i][i];
		}
		return new matrix*[2]{ lowerAndUpper, inversedDiagonal };
	}
	matrix operator*(const matrix& second) const
	{
		if (this->getColumns() != second.getRows())
		{
			return NULL;
		}
		matrix out(this->getRows(), second.getColumns());
		for (int i = 0; i < this->getRows(); i++)
		{
			for (int j = 0; j < second.getColumns(); j++)
			{
				for (int k = 0; k < this->getRows(); k++)
				{
					out.getData()[i][j] += this->getData()[i][k] * second.getData()[k][j];
				}
			}
		}
		/*this->print("Multiplication:\nFirst:");
		second.print("Second:");
		out.print("out");
		std::cin.ignore();*/
		return out;
	}
	matrix operator+(const matrix& second) const
	{
		if (this->columns != second.columns || this->rows != second.rows) return NULL;
		matrix out(rows, columns);
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				out.data[i][j] = this->data[i][j] + second.data[i][j];
			}
		}
		return out;
	}
};
matrix jacobi(const matrix& inversedDiagonal, const matrix& lowerAndUpper, const matrix& rightSide, const matrix& startingPoint, int passes)//righSide refers to "free" numbers that aren't multiplied by x^n
{
	if (passes == 0) return startingPoint;
	--passes;
	matrix newStartingPoint = inversedDiagonal * (lowerAndUpper) * (startingPoint) + inversedDiagonal * (rightSide);
	/*inversedDiagonal.print("inversedDiagonal:");
	lowerAndUpper.print("lowerAndUpper:");
	rightSide.print("rightSide:");
	startingPoint.print("startingPoint:");*/
	newStartingPoint.print("newStartingPoint:");
	std::cout << "passes left:" << passes << "\n";
	std::cin.ignore();
	return jacobi(inversedDiagonal, lowerAndUpper, rightSide, newStartingPoint, passes);

}
int main()
{
	matrix inputMatrix;
	if (!inputMatrix.loadData())
	{
		std::cout << "failed to load data";
		return 1;
	}
	matrix** submatrices = inputMatrix.generateSubmatrices();
	matrix lowerAndUpper = *submatrices[0];
	matrix inversedDiagonal = *submatrices[1];
	matrix rightSide(inputMatrix.getRows(), 1);
	for (int i = 0; i < inputMatrix.getRows(); i++)
	{
		rightSide.getData()[i][0] = inputMatrix.getData()[i][inputMatrix.getColumns() - 1];
	}
	
	std::cout << "specify starting point\n";
	matrix startingPoint(inputMatrix.getRows(), 1);
	for (int i = 0; i < startingPoint.getRows(); i++)
	{
		std::cin >> startingPoint.getData()[i][0];
	}
	inputMatrix.print("inputMatrix:");
	inversedDiagonal.print("inversedDiagonal:");
	lowerAndUpper.print("lowerAndUpper:");
	rightSide.print("rightSide:");
	startingPoint.print("startingPoint:");
	std::cout << "\nnumber of passes:\n";
	int passes;
	std::cin >> passes;
	std::cin.ignore();
	jacobi(inversedDiagonal, lowerAndUpper, rightSide, startingPoint, passes).print("result:");
	std::cin.ignore();
	
}