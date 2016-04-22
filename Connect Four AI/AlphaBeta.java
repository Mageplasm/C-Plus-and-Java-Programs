import java.util.List;
import java.util.Random;
import static java.lang.Math.*;
import java.util.ArrayList;

public class AlphaBeta extends AIModule
{
	private int ourPlayer;

	@Override
	public void getNextMove(final GameStateModule state)
	{
		//Get our player, either Player 1 or 2
		ourPlayer = state.getActivePlayer();

		double min; 
		int action = 0; 
		int move;
		int[] moves;
		int maxDepth = 1;

		//Get the legal moves for this state
		moves = getLegalMoves(state);

		while(!terminate)
		{
			//Set the starting values
			double alpha = -Integer.MAX_VALUE; 
			double beta = Integer.MAX_VALUE; 
			double utility = -Integer.MAX_VALUE;
			
			//Make all legal moves and store the action with the highest utility	
			for (int i = 0; i < moves.length; i++)
			{
				move = moves[i];
				state.makeMove(move);

				//Store the utility of the next state
				min = minValue(state, alpha, beta, maxDepth, 1);
				
				evaluate(state);

				if (utility == max(utility, min)); 

				else
				{
					utility = min; 
					action = move;  
				}

				if (utility >= beta)
					break;

				alpha = max(alpha, utility); 
				
				state.unMakeMove();
			}
			if(!terminate)
			{
				//Take the move with the maximum utility
				chosenMove = action;
				maxDepth++;
			}
		}
		//System.out.println(utility);
	}

	private int[] getLegalMoves(final GameStateModule state)
	{
		int numLegalMoves = 0;
		int order[] = {3,2,4,1,5,0,6};
		//int order[] = {3,0,6,1,5,2,4}; 

		//Run once to get the number of legal moves
		for(int i = 0; i < state.getWidth(); ++i)
			if(state.canMakeMove(order[i]))
				numLegalMoves++;

		//Initialize move array to the appropriate length 
		int[] moves = new int[numLegalMoves]; 

		numLegalMoves = 0;

		//Run again to store the legal moves
		for(int i = 0; i < state.getWidth(); ++i)
			if(state.canMakeMove(order[i]))
				moves[numLegalMoves++] = order[i];

		return moves;
	}

	private double maxValue(final GameStateModule state, double alpha, double beta, int maxDepth, int depth)
	{
		double utility; 
		int move;
		int[] moves;
		double[] eval;
		
		//If we have reached a leaf node, find out who won
		//Note: The program will terminate before we have a chance to search all leaf nodes
		if(state.isGameOver())
		{
			if(state.getWinner() == 0)
				return 0; 

			else if(ourPlayer == state.getWinner())
				return 999999999; 

			else 
				return -999999999; 
		}

		utility = -Integer.MAX_VALUE;

		moves = getLegalMoves(state); 

		if(terminate)
			return 0;
		
		for (int i = 0; i < moves.length; i++)
		{
			move = moves[i];
			state.makeMove(move);
			
			if(depth >= maxDepth)
			{
				eval = evaluate(state);
				utility = max(utility, eval[ourPlayer-1]-eval[2-ourPlayer]);
			}
			else
				utility = max(utility, minValue(state, alpha, beta, maxDepth, depth+1));

			if (utility >= beta)
			{
				state.unMakeMove(); 
				return utility;
			} 

			alpha = max(alpha, utility);
			
			state.unMakeMove(); 
		}
		return utility;
	}

	private double minValue(final GameStateModule state, double alpha, double beta, int maxDepth, int depth)
	{
		double utility; 
		int move;
		int[] moves;
		double[] eval;
		
		//If we have reached a leaf node, find out who won
		//Note: The program will terminate before we have a chance to search all leaf nodes
		if(state.isGameOver())
		{
			if(state.getWinner() == 0)
				return 0; 

			else if(ourPlayer == state.getWinner())
				return 999999999; 

			else 
				return -999999999;
		}

		utility = Integer.MAX_VALUE;
		
		moves = getLegalMoves(state);
		
		if(terminate)
			return 0;

		for (int i = 0; i < moves.length; i++)
		{
			move = moves[i]; 
			state.makeMove(move);
			
			if(depth >= maxDepth)
			{
				eval = evaluate(state);
				utility = min(utility, eval[ourPlayer-1]-eval[2-ourPlayer]);
			}
			else
				utility = min(utility, maxValue(state, alpha, beta, maxDepth, depth+1));

			if (utility <= alpha)
			{
				state.unMakeMove(); 
				return utility; 
			}

			beta = min(beta, utility); 
			
			state.unMakeMove(); 
		}
		return utility; 
	}
	
	public double[] evaluate(final GameStateModule game) ///////////// MAKE PRIVATE
	{
		double value[] = {0,0};
		double sum = 0;
		double wait = 0;
		double maxSum;
		double decay = 0.9;
		int power = 3;

		// Check for open horizontal plays
		for(int row=0; row<game.getHeight(); row++)
			for(int pl=1; pl<=2; pl++)
			{
				maxSum = 0;
				for(int col=0; col<game.getWidth()-3; col++)
				{
					wait = 0;
					sum = 0;
					for(int i=col; i<col+4; i++)
					{
						if(game.getAt(i,row) == pl)
							sum += 1;
						else if(game.getAt(i,row) == 3-pl)
						{
							sum = 0;
							break;
						}
						else
							wait += row-game.getHeightAt(col);
					}
					if(pow(sum,power)*pow(decay,wait) > maxSum)
					{
						value[max(pl-1,0)] = value[max(pl-1,0)] - maxSum + pow(sum,power)*pow(decay,wait);
						maxSum = pow(sum,power)*pow(decay,wait);
					}
				}
			}

		// Check for open vertical plays
		for(int col=0; col<game.getWidth(); col++)
			for(int pl=1; pl<=2; pl++)
			{
				maxSum = 0;
				// Move the window from left to right
				for(int row=max(0,game.getHeightAt(col)-3); row<min(game.getHeightAt(col),game.getHeight()-3); row++)
				{
					sum = 0;
					// Create a four tiles wide window
					for(int i=row; i<row+4; i++)
					{
						// If the tile belongs to the player we are currently checking for
						if(game.getAt(col,i) == pl)
							sum += 1;
						// Else if it belongs to the other player
						else if(game.getAt(col,i) == 3-pl)
						{
							sum = 0;
							break;
						}
					}
					// Only give points for the "best window" in the row
					if(sum > maxSum)
					{
						value[max(pl-1,0)] = value[max(pl-1,0)] - maxSum + sum;
						maxSum = sum;
					}
				}
			}

		// Check for open diagonal plays /
		for(int col=0; col<game.getWidth()-3; col++)
			for(int pl=1; pl<=2; pl++)
			{
				maxSum = 0;
				for(int row=0; row<game.getHeight()-3; row++)
				{
					wait = 0;
					sum = 0;
					for(int i=0; i<4; i++)
					{
						if(game.getAt(col+i,row+i) == pl)
							sum += 1;
						else if(game.getAt(col+i,row+i) == 3-pl)
						{
							sum = 0;
							break;
						}
						else
							wait += row-game.getHeightAt(col);
					}
					if(pow(sum,power)*pow(decay,wait) > maxSum)
					{
						value[max(pl-1,0)] = value[max(pl-1,0)] - maxSum + pow(sum,power)*pow(decay,wait);
						maxSum = pow(sum,power)*pow(decay,wait);
					}
				}
			}

		// Check for open diagonal plays \
		for(int col=3; col<game.getWidth(); col++)
			for(int pl=1; pl<=2; pl++)
			{
				maxSum = 0;
				for(int row=0; row<game.getHeight()-3; row++)
				{
					wait = 0;
					sum = 0;
					for(int i=0; i<4; i++)
					{
						if(game.getAt(col-i,row+i) == pl)
							sum += 1;
						else if(game.getAt(col-i,row+i) == 3-pl)
						{
							sum = 0;
							break;
						}
						else
							wait += row-game.getHeightAt(col);
					}
					if(pow(sum,power)*pow(decay,wait) > maxSum)
					{
						value[max(pl-1,0)] = value[max(pl-1,0)] - maxSum + pow(sum,power)*pow(decay,wait);
						maxSum = pow(sum,power)*pow(decay,wait);
					}
				}
			}

		return value;
	}
}