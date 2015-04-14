
public class MyChar extends Element {
	
	private char value; //Character value for this MyChar
	
	//Initialize character
	public MyChar(){
		this.value = '0';
	}
	
	//Get the character
	public char Get(){
		return this.value; 
	}
	
	//Set the character value
	public void Set(char val){
		this.value = val; 
	}
	
	//Print the character
	public void Print(){
		System.out.print("'");
		System.out.print(this.value);
		System.out.print("'");
	}
	
	
	
	
}
