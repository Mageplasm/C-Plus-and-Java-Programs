
public class MapIterator {

	private Map head; //Head of the Map 
	
	//Initialize head to null
	public MapIterator()
	{
		this.head = null; 
	}
	
	//Advance the iterator one position
	public void advance()
	{
		this.head = head.rest(); 
	}
	
	//Set which Map the head points to 
	public void set(Map first)
	{
		this.head = first; 
	}
	
	//Return the Pair at a certain position
	public Pair get()
	{
		return head.first(); 
	}
	
	//Return the Map head
	public Map mapGet()
	{
		return this.head; 
	}
	
	//See if two MapIterators point to the same position in the Map
	public boolean equal(MapIterator i)
	{
		Node n = i.mapGet().returnH().getNext(); 
		
		
		if(this.get() == null)
				return true;
		
		
		
		if(this.mapGet().returnH() == n)
			return true;
		
		return false; 
		
		
	}
	
	
}
