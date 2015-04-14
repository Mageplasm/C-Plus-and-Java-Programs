
public class MyInteger extends Element {
	
	private int value; //Integer value stored in this MyInteger
	
	//Initialize this integer
	public MyInteger(){
		this.value = '0';
	}
	
	//Return this integer
	public int Get(){
		return this.value; 
	}
	
	//Set the value of this integer
	public void Set(int val){
		this.value = val; 
	}
	
	//Print out the integer
	public void Print(){
		System.out.print(this.value); 
	}
	
	
	
	
}
