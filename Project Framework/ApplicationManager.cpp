#include "ApplicationManager.h"
#include "Actions\ActionAddSquare.h"
#include "Actions\ActionAddEllipse.h"
#include "Actions\ActionAddHexagon.h"
#include "Actions\ActionExit.h"
#include "Actions\ActionSave.h"
#include "Actions\Select.h"
#include "GUI\GUI.h"
#include <string.h>





//Constructor
ApplicationManager::ApplicationManager()
{
	//Create Input and output
	pGUI = new GUI;

	FigCount = 0;

	//Create an array of figure pointers and set them to NULL		
	for (int i = 0; i < MaxFigCount; i++)
		FigList[i] = NULL;
}

void ApplicationManager::Run()
{
	ActionType ActType;
	do
	{
		//1- Read user action
		ActType = pGUI->MapInputToActionType();

		//2- Create the corresponding Action
		Action* pAct = CreateAction(ActType);

		//3- Execute the action
		ExecuteAction(pAct);

		//4- Update the interface
		UpdateInterface();

	} 
	while (true);
	//while (ActType != EXIT);

}


//==================================================================================//
//								Actions Related Functions							//
//==================================================================================//
//Creates an action
Action* ApplicationManager::CreateAction(ActionType ActType)
{
	Action* newAct = NULL;
	string answer, answer2;
	//According to Action Type, create the corresponding action object
	switch (ActType)
	{
	case DRAW_SQUARE:
		newAct = new ActionAddSquare(this);
		break;

	case DRAW_ELPS:
		newAct = new ActionAddEllipse(this);

		break;

	case DRAW_HEX:
		newAct = new ActionAddHexagon(this);
		break;

	case SAVE:
		newAct = new ActionSave(this,FigCount,0);
		break;


	case SELECT:
		newAct = new Select(this);
		break;



	case EXIT: 
		pGUI->PrintMessage("Are you sure? if you want to save your grapth write y");
		answer = pGUI->GetSrting();
		if (answer == "Y" || answer == "y")
		{
			newAct = new ActionSave(this, FigCount,true);
		}
		else
		{
			pGUI->PrintMessage("See you soon!");
			exit(0);
		}
		break;

	case STATUS:	//a click on the status bar ==> no action
		return NULL;
		break;
	}
	return newAct;
}
//////////////////////////////////////////////////////////////////////
//Executes the created Action
void ApplicationManager::ExecuteAction(Action*& pAct)
{
	//Execute the created action
	if (pAct != NULL)
	{
		pAct->Execute();//Execute
		delete pAct;	//Action is not needed any more ==> delete it
		pAct = NULL;
	}
}
//==================================================================================//
//						Figures Management Functions								//
//==================================================================================//

//Add a figure to the list of figures
void ApplicationManager::AddFigure(CFigure* pFig)
{
	if (FigCount < MaxFigCount)
		FigList[FigCount++] = pFig;
}
////////////////////////////////////////////////////////////////////////////////////
CFigure* ApplicationManager::GetFigure(int x, int y) const
{
	//If a figure is found return a pointer to it.
	//if this point (x,y) does not belong to any figure return NULL
	for (int i = FigCount - 1; i >= 0; i--) {
		if (FigList[i] != NULL) {
			if (FigList[i]->PointInShape(x, y)) {
				return FigList[i];
			}

		}

	}

	///Add your code here to search for a figure given a point x,y	

	return NULL;
}
//==================================================================================//
//							Save And load Functions									//
//==================================================================================//

//to return the UI info in a string format
string ApplicationManager::colorString(color c) const
{
	if (c == BLACK) return "BLACK";
	else if (c == WHITE) return "WHITE";
	else if (c == BLUE) return "BLUE";
	else if (c == RED) return "RED";
	else if (c == YELLOW) return "YELLOW";
	else if (c == GREEN) return "GREEN";
	else if (c == LIGHTGOLDENRODYELLOW) return "LIGHTGOLDENRODYELLOW";
	else if (c == MAGENTA) return "MAGENTA";
	else if (c == TURQUOISE) return "TURQUOISE";
	else
		return "COLOR";
}


void ApplicationManager::SaveAll(ofstream& Out)   //Call the Save function for each Figure
{
	for (int i = 0; i < FigCount; ++i)
		FigList[i]->Save(Out);
}


//==================================================================================//
//							Interface Management Functions							//
//==================================================================================//

//Draw all figures on the user interface
void ApplicationManager::UpdateInterface() const
{
	for (int i = 0; i < FigCount; i++)
		FigList[i]->DrawMe(pGUI);		//Call Draw function (virtual member fn)
}
////////////////////////////////////////////////////////////////////////////////////
//Return a pointer to the interface
GUI* ApplicationManager::GetGUI() const
{
	return pGUI;
}
////////////////////////////////////////////////////////////////////////////////////
//Destructor


ApplicationManager::~ApplicationManager()
{
	for (int i = 0; i < FigCount; i++)
		delete FigList[i];
	delete pGUI;

}


//==================================================================================//
//							------EXIT WINDOW----------      						//
//==================================================================================//

int ApplicationManager::DisplayMessageBox()
{
	int msgboxID = MessageBox(
		NULL,
		"You didn't save the drawing\nAre you sure?",
		"Exit",
		MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONWARNING
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		pGUI->PrintMessage("The app should keep alive");
		break;
	case IDOK:

		pGUI->PrintMessage("The app should cancel");
		exit(0);
		break;
	}
	
	return msgboxID;
}