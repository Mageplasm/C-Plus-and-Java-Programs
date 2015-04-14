
public class Map {

	private MapIterator mapIterator; 
	private Node head; 
	private MapIterator set;
	private int length; 
	private boolean flag = false; 
	Pair testInt;  

	
	//Create an empty Map	
	public Map()
	{
		head = new Node(null); 
		set = new MapIterator();
		testInt = new Pair(null, null);
		testInt = null; 
		
		
	}
	
	//Set MapIterator to point to the beginning of the Map
	public MapIterator begin()
	{
		mapIterator = new MapIterator(); 
		mapIterator.set(this); 
		return this.mapIterator; 
		
	}
	
	//Set MapIterator to point to the end of the Map
	public MapIterator end()
	{
		MapIterator s = new MapIterator(); 
		Map end = new Map();
		
		end = this; 
		end.set.set(this);
		
		s = set; 

		//int limit = (this.length()-1); 
		
		for (int pos  = 0; pos < this.length(); pos++)
		{
			
			
			if(pos > 0)
			s.advance();
				
			
		}
		
		if(!flag)
		{
			s.mapGet().add(testInt);
			flag = true;
		}

		return s; 
	}

	
	//Return the first Pair in the Map
	public Pair first(){
		Node currentNode = head; 
		
		if(this.length() == 0)
		{
			
			return null; 
		}
		
		 return (Pair) currentNode.getNext().getData();
	}
	
	//Set Map head to point to the rest of the Map
	public Map rest(){
		
		 Map newSeq = new Map(); 
		 newSeq.head = this.head.getNext(); 
		 
		 newSeq.length = this.length - 1; 
		 
		/*head1 = this.head.getNext(); 
		return this;*/
	
		 return newSeq; 
		
	}

	//Add a pair to the Map	
	public void add(Pair inval)
	{
		Node newNode = new Node(inval); 
		Node currentNode = head;
		
		if(inval == null)
			return; 
		
		for(int i = 0; currentNode.getNext() != null ; i++)
			
		{
			
			if((inval.getKey().Get() > ((Pair)currentNode.getNext().getData()).getKey().Get() || inval.getKey().Get() ==((Pair)currentNode.getNext().getData()).getKey().Get()) && currentNode.getNext() != null)
				currentNode = currentNode.getNext(); 
		
			
			else
				break; 
			
		}
		
		newNode.setNext(currentNode.getNext()); 
		
		currentNode.setNext(newNode); 
		
		length++; 
		
		
		
	}

	//Return the number of elements in the Map	
	public int length(){
		int size = 0;
	 	Node currentNode = head.getNext();
	 	while (currentNode != null){
	 	currentNode = currentNode.getNext();
	 	size++;
	 	}
	 	 
	 	if(length == size){
	 	return length;
	 	}else{
	 	return size;
	 	}
	}

	//Find the element with the specific key 	
	public MapIterator find(MyChar key)
	{
		MapIterator start = new MapIterator(); 
		Pair p = new Pair(null, null); 
		MyChar set = new MyChar(); 
		
		
		for(start = this.begin(); !start.equal(this.end()); start.advance())
		{
			p = start.get(); 
			set = p.getKey(); 
			
			if(key.Get() == set.Get())
			break; 
		}
		
		return start; 
	}
	
	//Print the Map
	public void Print()
	{
		Pair data; 
		Node currentNode = head.getNext(); 
		System.out.print("[ "); 

		 while (currentNode != null) {
	        	data = (Pair)currentNode.getData();
	        	data.Print();
	            currentNode = currentNode.getNext();
	            
	            if(currentNode != null)
	            	System.out.print(" ");
	        }
		System.out.print(" ]"); 
		
		
	}
	
	//Return the head of the Map 
	public Node returnH()
	{
		return head; 
	}
	
	
}
