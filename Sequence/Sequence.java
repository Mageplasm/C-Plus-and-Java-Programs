
public class Sequence extends Element  {

	private Node head; //Head points to the beginning of the sequence
	private SequenceIterator iteratorNode; //
	private Sequence endTest; 
	private static int test = 0; 
	private SequenceIterator set;
	private boolean flag = false; 
	Element testInt;  

	private int listCount; //Number of elements in the Sequence
	
	//Create and empty Sequence
	public Sequence(){
		head = new Node(null);
		 set = new SequenceIterator();
		//iteratorNode = new SequenceIterator();
		//New Code:
		testInt = new Element();
		testInt = null;
		listCount = 0; 
	}
	
	//Set IteratorNode to point to the beginning of the Sequence
	public SequenceIterator begin()	{
		iteratorNode = new SequenceIterator();

		iteratorNode.set(this); 
		return this.iteratorNode; 
		
	}
	
	
	//Return a sequenceIterator that points to the end of the Sequence
	public SequenceIterator end()
	{
		SequenceIterator s = new SequenceIterator(); 
		Sequence end = new Sequence();
		
		end = this; 
		end.set.set(this);
		
		s = set; 

		int limit = (this.length()-1); 
		
		for (int pos  = 0; pos < this.length(); pos++)
		{	
			if(pos > 0)
				s.advance();
		}
		
		if(!flag)
		{
			s.getSeq().add(testInt, listCount);
			flag = true;
		}

		return s; 
	}


	
	//Add an element to the sequence at the specified index
	public void add(Element data, int index)
	{
		Node newNode = new Node(data); 
		Node currentNode = head;
		
		for (int i = 0; i < index && currentNode.getNext() != null; i++)
			currentNode = currentNode.getNext(); 
		
		newNode.setNext(currentNode.getNext()); 
		
		currentNode.setNext(newNode); 
		
		listCount++; 
	}
	
	//Return the element at the specified position in the Sequence
	public Element index(int pos){
		
		if (pos < 0)
            return null;
		
		Node currentNode = head.getNext(); 
		
		for (int i = 0; i < pos; i++)
		{
			if(currentNode.getNext() == null)
				return null; 
		
			currentNode = currentNode.getNext(); 
		}
		
		return currentNode.getData(); 
	}
	

	//Return the first Element in the Sequence
	public Element first(){
		Node currentNode = head; 
		
		if(this.length() == 0)	
			return null; 
		
		 return currentNode.getNext().getData();
	}
	
	//Make head point to the rest of the sequence
	public Sequence rest(){
	
		 Sequence newSeq = new Sequence(); 
		 newSeq.head = this.head.getNext(); 
		 
		 newSeq.listCount = this.listCount - 1; 
	
		 return newSeq; 
	}
	
	//Delete an element at the specified position
	public boolean delete (int index){
		
		if (index < 0 || index > length())
	            return false;
		   
		Node currentNode = head; 
		
		for (int i = 0; i < index && currentNode.getNext() != null; i++)
		{			
			if (currentNode.getNext() == null)
				return false; 
			
			currentNode = currentNode.getNext(); 
		}
		
		currentNode.setNext(currentNode.getNext().getNext());
        listCount--; // decrement the number of elements variable
        return true;
	}
	
	//Return the size of the sequence
	public int length(){
		int size = 0;
	 	Node currentNode = head.getNext();

	 	while (currentNode != null){
		 	currentNode = currentNode.getNext();
		 	size++;
	 	}
	 	//return size;
	 	 
	 	if(listCount == size){
	 		return listCount;
	 	}
	 	else{
	 		return size;
	 	}
	}
	
	
	//New Sequence will contain no inner sequences
	public Sequence flatten(){
		Node currentNode = head.getNext();
		Sequence newSequence = new Sequence();
		Sequence tempSeq; 
		int pos = 0; 
		int j = 0; 
		int limit;
		
		if(currentNode == null)
			return null; 
		
		for(int i = 0; i < listCount; i++)
		{
			if (currentNode.getData() instanceof Sequence)
			{
				tempSeq = currentNode.getData().flatten(); 
				limit = pos + tempSeq.listCount; 
				
				for(; pos < limit; pos++)
				{
					newSequence.add(tempSeq.index(j), pos); 
					j++;
				}
				
			}
			
			else
				newSequence.add(currentNode.getData(), pos);
			
			currentNode = currentNode.getNext(); 
			pos++; 
			j = 0; 
		}
		
		return newSequence; 
	}
	
	//Perform a deeop copy of Sequence object
	public Sequence copy(){
		Sequence nwSeq = new Sequence();
		Sequence updatedSequence = this; 
		Element e; 
		int position = 0; 
		int size = this.length(); 
		
		for (e = updatedSequence.first(); position < size;  ){ 
			if (e instanceof Sequence){
				nwSeq.add(((Sequence)e).copy(), position); 
			}
			
			else{
				if (e instanceof MyChar)
				{
					MyChar newChar = new MyChar(); 
					newChar.Set(((MyChar) e).Get()); 
					nwSeq.add(newChar, position); 
				}
				
				else if  (e instanceof MyInteger)
				{
					MyInteger newInt = new MyInteger(); 
					newInt.Set(((MyInteger) e).Get()); 
					nwSeq.add(newInt, position); 
				}
			}
			
			updatedSequence = updatedSequence.rest();	
			e = updatedSequence.first();
			position++; 
		}

		return nwSeq; 
	}
	
	
	//Print the Sequence
	public void Print(){
	 	Element data; 
        Node currentNode = head.getNext();
    	System.out.print("[ ");

        while (currentNode != null) {
        	data = currentNode.getData();
        	data.Print();
            currentNode = currentNode.getNext();
            
            if(currentNode != null)
            	System.out.print(" ");
        }

    	System.out.print(" ]"); 
	}
	 
	 //Return the head of the Sequence 
	 public Node returnH()
	 {
		 return head; 
	 }
	 
	 //Adjust the node's value at a certain position in the matrix
	 public void node_index(int row_num, MyInteger e){		
		if (row_num < 0)
            return ;
			
		Node currentNode = head.getNext(); 
		
		for (int i = 0; i < row_num; i++)
		{
			if(currentNode.getNext() == null)
				return ; 
		
			currentNode = currentNode.getNext(); 
		}
		
		currentNode.setData(e); 	
	}
}
