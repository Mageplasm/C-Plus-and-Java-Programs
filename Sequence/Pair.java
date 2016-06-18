
public class Pair extends Element {
	
	private MyChar key; //Key of the Element
	private Element value; //Value of the Element
	
	//Initialize key and value pair
	public Pair (MyChar k, Element v)
	{
		this.key = k; 
		this.value = v; 
	}
	
	//Return the key of this Pair
	public MyChar getKey()
	{
		return key; 
	}
	
	//Print the Pair
	public void Print()
	{
		System.out.print("(");
		key.Print(); 
		System.out.print(" ");
		value.Print(); 
		System.out.print(")");
	}
}
