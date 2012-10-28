#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

class Matrix{
private:
	double** matrix;
	int rows, columns;
	int* solutions;
	
	
public:
	void create()
	{		
		cout<<"Enter the number of constraints: ";
		cin>>rows;
		rows += 1;
		cout<<"Enter the number of variables: ";
		cin>>columns;
		columns += 1;
		matrix = new double*[rows];
		
		for (int i = 0; i<rows;i++)
			matrix[i] = new double[columns]; 

		solutions = new int[columns-1];
		for (int j = 0; j<columns-1;j++)
			solutions[j]=-1;
	}

	void create(int r, int c)
	{
		rows=r;
		columns=c;

		matrix = new double*[rows];

		for (int i = 0; i<rows;i++)
			matrix[i] = new double[columns-1];			
		
		solutions = new int[columns];
		for (int j = 0; j<columns-1;j++)
			solutions[j]=-1;
		
			
	}

	void fill()
	{
		cout<< "Enter the "<< columns-1 << " values for the objective function: ";
		
		for (int j =0;j<columns-1;j++)
			{				
				cin>>matrix[rows-1][j];				
			}
			matrix[rows-1][columns-1]=0;						
			cout<<"\n";

		cout<<"u = ";
		
		for (int a=0; a<columns-2;a++)
		{
			cout<<matrix[rows-1][a]<<"x"<<a+1<<" + ";
		}

		if ( columns-2>=0)
			cout<<matrix[rows-1][columns-2]<<"x"<<columns-1<<"\n";

		for (int i = 0;i<rows-1;i++)
		{
			cout<< "Enter the "<< columns << " values for constraint set #"<<i+1<<": ";
			for (int j =0;j<columns;j++)
			{
				cin>>matrix[i][j];				
			}			
			//print the variables of the constraint
			//start at first entry
			cout<<matrix[i][0]<<"x1";
			//all entries except first and last
			for (int b =1;b<columns-1;b++)
			{
				cout<<" + "<<matrix[i][b]<<"x"<<b+1;
			}
			//last entry if the matrix is big enough
			if ( columns-2>=0)
				cout<<" <= "<<matrix[i][columns-1]<<"\n";
			
			matrix[i][columns-1]*=-1;
						
		}

			cout<<"\n";	
		
		
		
	}	

	bool test_completion()
	{
		//test for endpoint
		//bool neg_test_obj=false, neg_test_const=false;
		bool test = false;
		for (int a=0;a<columns-1;a++)
		{
			if(matrix[rows-1][a]>0)				
				test = true;			
		}

		if (test == true)
		{
			for (int i = 0;i<rows-1;i++)
			{
				for (int j = 0;j<columns-1;j++)
				{
					if (matrix[i][j]>0)
						return true;
				}
			}
		}		
		return false;

	}

	int* pivoting_point()
	{
		//find pivot point

		bool* objective_test = new bool[columns];
		int *pivot_pos = new int[2];
		pivot_pos[0]=0;
		pivot_pos[1]=0;
		
		double ratio = -1;

		//list positive columns
		for (int i=0;i<columns-1;i++)
		{
			if (matrix[rows-1][i]>0)
				objective_test[i]=1;				
		}
		for (int j=0;j<columns-1;j++)
		{
			if (objective_test[j]==1)
			{
				for (int k=0;k<rows-1;k++)
				{					
					if (matrix[k][j]>0 && abs(matrix[k][columns-1])/matrix[k][j]<=ratio || ratio == -1)
					{
						pivot_pos[0]=k;
						pivot_pos[1]=j;

						ratio = abs(matrix[pivot_pos[0]][columns-1]/matrix[pivot_pos[0]][pivot_pos[1]]);
					}
				}
			}
		}

		cout<<"\n pivot pointx "<<pivot_pos[0];
		cout<<"\n pivot pointy "<<pivot_pos[1];
		return pivot_pos;
		
	}	

	void pivot()
	{
		int* pivot_point = pivoting_point();
		if (solutions[pivot_point[1]]==-1)
			solutions[pivot_point[1]]=pivot_point[0];
		else
			solutions[pivot_point[1]]=-1;

		double value = matrix[pivot_point[0]][pivot_point[1]];
		//make row = 1

		for (int r = 0;r<columns;r++)
		{
			matrix[pivot_point[0]][r]/=value;
		}

		for (int u1=0;u1<rows;u1++)
		{
			if(u1!=pivot_point[0])
			{
				for (int u2=0;u2<columns;u2++)
				{
					if (u2!= pivot_point[1])
						matrix[u1][u2]-=matrix[pivot_point[0]][u2]*matrix[u1][pivot_point[1]];
										
				}
			}
		}

		for (int c = 0;c<rows;c++)
		{
			if (c!=pivot_point[0])
				matrix[c][pivot_point[1]]/=-value;
			else
				matrix[c][pivot_point[1]]/=value;				
		}
	}
	

	double** get()
	{return matrix;}

	void print()
	{
		for (int i =0;i<rows;i++)
		{
			if (i == rows-1)
				for (int k = 0; k<columns;k++)
					cout<<setw(6)<<"___";
				cout<<"\n";

			for (int j = 0;j<columns;j++)
			{
				if (j == columns-1)
					cout<<"|";
				cout<<setw(10)<<left<<setprecision(3)<<matrix[i][j];
			}
			cout<<"\n";			
		}
	}

	void answer()
	{
		cout<<"The optimal solution is "<<matrix[rows-1][columns-1]<<" when";

		for (int i = 0;i<columns-1;i++)
		{	
			if (solutions[i]!=-1)
				cout<<", x"<<i+1<<" = "<< -matrix[solutions[i]][columns-1];
			else
				cout<<", x"<<i+1<<" = "<< 0;
		}
		cout<<"\n";
	}


	
};


//Code to test main code
class Test{
	private:		
		void print(string a) {cout<<a;}		
	public:
		bool run_tests()
		{
			//variable to return whether tests failed or passed
			bool valid = true;
			//matrix class for testing
			Matrix matrix1;
			//first test is to see if a matrix of proper size is created. It is filled with 0s outside the class.
			double test1[5][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
			matrix1.create(5,4);
			double** compare = matrix1.get();
			for (int a=0;a<5;a++)
				for (int b=0;b<4;b++)
					compare[a][b]=0;
			for (int c=0;c<5;c++)
				for (int d=0;d<4;d++)
					if (compare[c][d]!=test1[c][d])
						valid=false;						
			if (valid==true)
				print("Matrix creation works \n");
			if (valid == false)
			{
				print("Failed Matrix creation \n");
				valid = false;
			}

			//Test matrix filling
			Matrix matrix2;
			cout<<"Enter 2 for constrains, 3 for variables. Objective function is 0s. Constraints are a 2x2 identity matrix.\n";
			matrix2.create();
			matrix2.fill();
			matrix2.print();
			/*compare = matrix2.get();
			double test2[3][3] = {{1,0,0},{0,1,0},{0,0,0}};			
			for (int e=0;e<3;e++)
				for (int f=0;f<3;f++)
					if (compare[e][f]!=test2[e][f])
						valid=false;

			if (valid==true)
				print("Matrix filling works \n");
			if (valid == false)
			{
				print("Failed Matrix filling \n");
				valid = false;
			}
			*/

			Matrix matrix;

			cout<<"enter {{-2,3,-2},{3,2,-5},{2,3,0}}";
			matrix.create(3,3);
			matrix.fill();			
			int* point = matrix.pivoting_point();			
			compare = matrix.get();			
			cout<<"\n"<<point[0]<<"  "<<point[1];
			cout<<"The pivot value is: "<<compare[point[0]][point[1]]<<"\n";
			cout<<"Enter 1 if it's the same \n";
			cin>>valid;
			
			cout<<	"Testing the completion test \n";
			cout<<matrix.test_completion();

			cout<<"Testing pivot operation \n";

			matrix.print();
			matrix.pivot();
			matrix.print();

			cout<<matrix.test_completion()<<"\n";

			matrix.pivot();
			matrix.print();

			cout<<matrix.test_completion()<<"\n";

			return valid;
			
		}
};



int main()
{
	bool debug;		
	cout<<"Enter 1 if you would you like to debug the program before running it, enter anything else to go straight to the program: ";
	cin>>debug;
	if (debug == 1)
	{
		cout<<"Debugging \n";
		Test test;
		bool test_result = test.run_tests();
		if (test_result == false)
			cout<<"Tests failed \n";
		else
			cout<<"Tests passed \n";
	}
	else
		cout<<"Running Program \n";

	Matrix matrix;
	matrix.create();
	
	matrix.fill();
	matrix.print();	

	while (matrix.test_completion()==true)
	{		
		matrix.pivot();
		matrix.print();		
	}

	cout<<"\n";
	matrix.answer();
	
	return 0;
}