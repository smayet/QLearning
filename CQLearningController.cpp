/**
         (                                      
   (     )\ )                                   
 ( )\   (()/(   (    ) (        (        (  (   
 )((_)   /(_)) ))\( /( )(   (   )\  (    )\))(  
((_)_   (_))  /((_)(_)|()\  )\ |(_) )\ )((_))\  
 / _ \  | |  (_))((_)_ ((_)_(_/((_)_(_/( (()(_) 
| (_) | | |__/ -_) _` | '_| ' \)) | ' \)) _` |  
 \__\_\ |____\___\__,_|_| |_||_||_|_||_|\__, |  
                                        |___/   

Refer to Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
for a detailed discussion on Q Learning
*/
#include "CQLearningController.h"


CQLearningController::CQLearningController(HWND hwndMain):
	CDiscController(hwndMain),
	_grid_size_x(CParams::WindowWidth / CParams::iGridCellDim + 1),
	_grid_size_y(CParams::WindowHeight / CParams::iGridCellDim + 1)
{
}
/**
 The update method should allocate a Q table for each sweeper (this can
 be allocated in one shot - use an offset to store the tables one after the other)

 You can also use a boost multiarray if you wish
*/
void CQLearningController::InitializeLearningAlgorithm(void)
{
	int n=m_numSweepers;
	
	// make an n by y by x multi_array
	// in other words, n x by y tables 
	// one for each minesweeper
	
	Qtables=new double**[n];
	for(int i=0;i<n;++i) {
		Qtables[i]=new double*[_grid_size_y];
		for(int j=0;j<_grid_size_y;++j) {
			Qtables[i][j]=new double[_grid_size_x];
		}
	}
}
/**
 The immediate reward function. This computes a reward upon achieving the goal state of
 collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and 
 of course for hitting supermines/rocks!
*/
double CQLearningController::R(uint x,uint y, uint sweeper_no){
	
	// state: position on grid 
	
	// x and y represent coordinates
	// check this sweeper at those coordinates i.e. m_vecSweeper[sweeper_no]
	// if the sweeper has collected all mines, reward 200 - check m_dMinesGathered
	// if the sweeper encounters a rock at these coordinates, reward -50
	// if the sweeeper encounters a supermine at these coordinates, reward -100
	
	// check all m_vecObjects for whether any of them getPosition() gives you the same as (x,y) 
	// you can getType() on a collision object 
	
	if(m_vecSweepers[sweeper_no].MinesGathered()==m_NumMines) 
		return 500;
	
	for(int i=0;i<m_vecObjects.size();++i) {
		if((m_vecObjects[i].getPosition().x==x)&&(m_vecObjects[i].getPosition().y==y)) {
			
			if(m_vecObjects[i].getType()==Supermine) 
				return -100;
				
			else if(m_vecObjects[i].getType()==Rock) 
				return -50;
				
		}
	}
	
	return 0;
}
/**
The update method. Main loop body of our Q Learning implementation
See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
*/
bool CQLearningController::Update(void)
{
	//m_vecSweepers is the array of minesweepers
	//everything you need will be m_[something] ;)
	uint cDead = std::count_if(m_vecSweepers.begin(),
							   m_vecSweepers.end(),
						       [](CDiscMinesweeper * s)->bool{
								return s->isDead();
							   });
	if (cDead == CParams::iNumSweepers){
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
	}

	//iterating over the minesweepers
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		
		// THE STATE WE ARE IN 
		
		//1:::Observe the current state:
		
		CDiscMinesweeper mine=m_vecSweepers[sw];
		SVector2D<int> facing=mine.m_vLookAt;
		
		//2:::Select action with highest historic return:
		
		// this is going to require checking where we can go
		// and looking at the reward function to see what we'll get 
		
		//TODO
		//now call the parents update, so all the sweepers fulfill their chosen action
	}
	
	CDiscController::Update(); //call the parent's class update. Do not delete this.
	
	//iterating over the minesweepers 
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		
		// THE NEXT STATE THAT WE VISIT, ACCORDING TO CODE ABOVE 
		
		//3:::Observe new state:
		//TODO
		//4:::Update _Q_s_a accordingly:	// UPDATE ACCORDING TO REWARD FUNCTION 
		//TODO
	}
	return true;
}

CQLearningController::~CQLearningController(void)
{
	for(int i=0;i<m_vecSweepers.size();++i) {
		for(int j=0;j<_grid_size_y;++j) {
			delete[] Qtables[i][j];
		}
		delete[] Qtables[i];
	}
	delete[] Qtables; 	
}
