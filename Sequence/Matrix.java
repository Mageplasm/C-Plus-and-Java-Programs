
public class Matrix extends Sequence{
	
	private Sequence row;
	private Sequence matrix; 
	private MyInteger initial; 
	private int rows; 
	private int cols ; 
	private Matrix test; 
	private boolean truth = true; 

	//Set each value in the Matrix to 0 
	public Matrix(int rowsize, int colsize)
	{
		matrix = new Sequence();
		initial = new MyInteger(); 
		initial.Set(0);
		rows = rowsize; 
		cols = colsize; 
		
		
		for (int i = 0; i < rowsize; i++)
		{
			
			row = new Sequence();
			for (int j =0; j < colsize; j++)
			{
				row.add(initial, j); 
			}

			matrix.add(row, i);
		}
	}
	
	
	//Set the value at a certain position in the Matrix
	public void Set(int rowsize, int colsize, int value)
	{
	Sequence seq; 
		
		
		
		MyInteger newInt = new MyInteger(); 
		newInt.Set(value); 
		int limit = rowsize - 1; 
		for (int j = 0; j <= rowsize; j++)
		{
			 if(j == rowsize)
			 {
				 seq = (Sequence)matrix.index(j); 


				

			seq.node_index(colsize, newInt); 

			 }
		
		}
		 

		
	}
	
	//Get the value in the Matrix at a specific coordinate
	int Get(int rowsize, int colsize)
	{
		int val = 0; 
		Sequence seq; 
		
		seq = (Sequence)this.matrix.index(rowsize); 
		val = ((MyInteger)seq.index(colsize)).Get();
		
		return val; 
	}
	

	//Print the Matrix
	public void Print()
	{
		if(!truth)
			return; 
		
		Sequence new_row = new Sequence(); 
		for (int i = 0; i < rows; i++)
		{
			new_row = (Sequence)matrix.index(i); 
			new_row.Print();
			System.out.println("");
			
		}
	}
	
	//Find the sum of two matrices
	public Matrix Sum(Matrix m2)
	{
		Sequence matrix1 = new Sequence();
		Sequence seq; 

		Matrix sumMatrix = new Matrix(rows,cols); 
		Sequence s1, s2; 
		Sequence sub1, sub2; 
		int i1, i2, sum; 
		
		s1 = this.matrix; 
		s2 = m2.matrix; 
		for (int i = 0; i < rows; i++)
		{
			sub1 = (Sequence)s1.index(i); 
			sub2 = (Sequence)s2.index(i); 
			seq = (Sequence)sumMatrix.matrix.index(i); 

			
			for(int j = 0; j < cols; j++)
			{
				i1 = ((MyInteger)sub1.index(j)).Get();
				i2 = ((MyInteger)sub2.index(j)).Get();
				sum = i1 + i2; 
				
				initial = new MyInteger(); 
				initial.Set(sum);
				
				seq.node_index(j, initial); 
				
				

			}
		}
		
		return sumMatrix; 
	}
	
	//Find the product of two matrices
	public Matrix Product(Matrix m2)
	{
		
		
		Matrix sumMatrix = new Matrix(this.rows,m2.cols); 
		if(this.cols != m2.rows)
		{
			System.out.println("Matrix dimensions incompatible for Product");
			sumMatrix.truth = false; 
		}
		
		Sequence s1;
		Sequence s2; 
		int product = 0, i1, i2; 
		initial = new MyInteger(); 

		
		for (int i = 0; i < this.rows; i++)
		{
			s1 = (Sequence)this.matrix.index(i);

			for(int j =0; j < m2.cols; j++)
			{
				for(int k = 0; k < this.cols; k++)
				{
					i1 = ((MyInteger)s1.index(k)).Get();
					s2 = (Sequence)m2.matrix.index(k);					
					i2 = ((MyInteger)s2.index(j)).Get();
					product = product + (i1 * i2); 

					


					
				}
				sumMatrix.Set(i,j,product);
				product = 0; 
			}

		}
		
		return sumMatrix; 
		
	}
}
