
public class SequenceIterator  {
	
	private Sequence val; //Sequence node which iterates through the Sequence
	//Make Sequence null
	public SequenceIterator()
	{
		this.val = null;
	}
	
	//Advance the Sequence to point 1 position ahead
	public void advance()
	{
		this.val = val.rest(); 
	}
	
	//Return the element at a certain position
	public Element get()
	{
		return val.first(); 
	}
	
	//Return the Sequence head
	public Sequence getSeq()
	{
		return this.val; 
	}
	
	//See if two Sequence Nodes point to same position in the Sequence
	public boolean equal(SequenceIterator i)
	{
		Node n = i.getSeq().returnH().getNext(); 
		
		if(this.get() == null)
				return true;
		
		
		
		if(this.getSeq().returnH() == n)
			return true;
		
		return false; 
		
		
	}
	
	//Set the Sequence head
	public void set(Sequence first) {
		this.val = first; 

	}	
	
	


}
