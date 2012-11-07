#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;

class Matrix{
private:
	//declare globals
	double** matrix;
	int rows, columns;
	int rep_test[2];
	int rep_test1[2];
	int* rvariables;
	int* cvariables;
	
	
public:
	void create()
	{		
		//get input for # of rows and columns
		cout<<"Enter the number of constraints: ";
		cin>>rows;
		rows += 1;
		cout<<"Enter the number of variables: ";
		cin>>columns;
		columns += 1;
		//setup 2d dynamic array
		matrix = new double*[rows];		
		for (int i = 0; i<rows;i++)
			matrix[i] = new double[columns]; 
		//setup variable on axis for flipping
		cvariables = new int[columns-1];
		for (int j = 0; j<columns-1;j++)
			cvariables[j]=-1*(j+1);
		rvariables = new int[rows-1];
		for (int i = 0; i<rows-1;i++)
			rvariables[i]=i;
		//initialize variables that keep track of repetition		
		rep_test[0] = 0;
		rep_test[1] = -1;
		rep_test1[0] = -2;
		rep_test1[1] = -3;
	}

	
	void fill()//fill matrix with values
	{
		cout<< "Enter the "<< columns-1 << " values for the objective function: ";
		//start with objective function, bottom row
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
		//make sure matrix has enough columns for at least a=c
		if ( columns-2>=0)
			cout<<matrix[rows-1][columns-2]<<"x"<<columns-1<<"\n";

		cout<<"Use the form ax1+bx2...<=c for contrant set input. \n";
		//do rest of matrix now
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
		if (rep_test[0]==rep_test1[0]&&rep_test[1]==rep_test1[1]) //test for repetition, initialized variables are not equal
			return false;		

		for (int b=0;b<rows-1;b++) //check side for positive varaibles since program can't pivot on that yet
			if (matrix[b][columns-1]>0)
				return false;

		for (int a=0;a<columns-1;a++) //check for no solution from negative objective or negatives above objective value
		{
			if(matrix[rows-1][a]>0)				
				for (int i = 0; i<rows-1;i++)					
					{
						if (matrix[i][a]>0)
						return true;
					}
		}

		//return false if no viable pivot points exist
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
		for (int i=0;i<columns-1;i++) //make up list of positive objective columns
		{
			if (matrix[rows-1][i]>0)
				objective_test[i]=1;				
		}
		for (int j=0;j<columns-1;j++)
		{
			if (objective_test[j]==1) //make sure to only check columns with positive objective
			{
				for (int k=0;k<rows-1;k++)
				{					
					if (matrix[k][j]!=0&&matrix[k][j]>0 && abs(matrix[k][columns-1])/matrix[k][j]<=ratio || ratio == -1) //pivot can't be 0, and must be larger or equal to last pivot point
					{
						pivot_pos[0]=k;
						pivot_pos[1]=j;

						ratio = abs(matrix[pivot_pos[0]][columns-1]/matrix[pivot_pos[0]][pivot_pos[1]]); // ratio must be positive
					}
				}
			}
		}

		cout<<"\nPivot point is at ("<<pivot_pos[0]+1<<","<<pivot_pos[1]+1<<") with a value of "<<matrix[pivot_pos[0]][pivot_pos[1]]<<"\n";
		
		return pivot_pos;
		
	}	

	void pivot()
	{
		//get pivote point from pivoting method
		int* pivot_point = pivoting_point();		
		int hold;
		//set up tracker for variables
		hold =	cvariables[pivot_point[1]];
		cvariables[pivot_point[1]]=rvariables[pivot_point[0]];
		rvariables[pivot_point[0]]=hold;
		//set up values to check for repetition
		rep_test[0]=rep_test1[0];
		rep_test[1]=rep_test1[1];
		rep_test1[0]=pivot_point[0];
		rep_test1[1]=pivot_point[1];

		double value = matrix[pivot_point[0]][pivot_point[1]]; //put in pivot value
		
		for (int r = 0;r<columns;r++) //set pivot element to 1
		{
			matrix[pivot_point[0]][r]/=value;
		}

		for (int u1=0;u1<rows;u1++) //pivot all elements except same row
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

		for (int c = 0;c<rows;c++) //pivot columns of pivot element
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
				cout<<setw(10)<<left<<setprecision(4)<<matrix[i][j];
			}
			cout<<"\n";			
		}
	}

	void answer() //display whether there is or isn't a solution
	{
		bool solution = 1;

		for (int a = 0; a<columns-1;a++) //check if objective is negative values
		{
			if (matrix[rows-1][a]>0)
				solution = 0;
		}

		if (solution == 1) //if solution exists
		{
			double* solutions = new double[columns-1];

			for (int b =0;b<columns-1;b++)
				solutions[b]=0;

			for(int a =0;a<rows-1;a++)
				if(rvariables[a]<0)
					solutions[-(rvariables[a]+1)]=-matrix[a][columns-1];	
								

			cout<<"The optimal solution is "<<matrix[rows-1][columns-1]<<" when";

			for (int i = 0;i<columns-1;i++)
			{	
				cout<<", x"<<i+1<<" = "<<solutions[i];				
			}			
			
		cout<<"\n";
		}
		else
			cout<<"There is no finite optimal solution.";
	}


	
};

int main()
{
	bool loop = 1;
	while (loop == 1) //loop while user does not input 0
	{
		Matrix matrix;
		matrix.create();
	
		matrix.fill();
		matrix.print();	

		while (matrix.test_completion()==true) //loop as long as test_completion says there is a pivot point or solution is found
		{		
			matrix.pivot();
			matrix.print();		
		}

		cout<<"\n";
		matrix.answer();
		cout<<"\n Enter 1 if you want to do another, or 0 to stop.";
		cin>>loop;
	}
	
	return 0;
}