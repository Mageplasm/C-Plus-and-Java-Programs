
public class Node {
	private Node next; //Refers to the next node
	public Element data; //Refers to the data stored at this node
	
	//Initialize node
	public Node(Element dataValue){
		next = null; 
		data = dataValue; 
	}
	
	//Return data stored at this node
	public Element getData(){
		return data; 
	}
	
	//Set the data for this node
	public void setData(Element dataValue){
		data = dataValue; 
	}
	
	//Return the next node
	public Node getNext(){
		return next; 
	}

	//Initialize next node	
	public void setNext(Node nextValue){
		next = nextValue; 
	}
}
