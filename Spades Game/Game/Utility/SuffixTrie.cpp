#include "SuffixTrie.h"

CSuffixTrie::CSuffixTrie()
{
	//Init the root node
	m_aRoot.pFailureNode=NULL;
	m_aRoot.aChar=0;
	m_aRoot.bFinal=false;
	m_aRoot.usDepth=0;
}

CSuffixTrie::CSuffixTrie(const CSuffixTrie& rTrie)
{
	//Clone to here
	rTrie.CloneTrie(*this);
}

CSuffixTrie::~CSuffixTrie()
{
	//Delete the tree
	Clear();
}

void CSuffixTrie::Clear()
{
	DeleteNode(&m_aRoot);
}

CSuffixTrie& CSuffixTrie::operator=(const CSuffixTrie& rTrie)
{
	//Sanity check
	if (this==&rTrie)
		return *this;

	//Close ourselves first
	rTrie.CloneTrie(*this);

	//Done
	return *this;
}

void CSuffixTrie::CloneTrie(CSuffixTrie& rTarget)const
{
	//First delete the other trie
	rTarget.Clear();

	//Do a clone
	Node* pClone;
	pClone=CloneNode(&m_aRoot);

	//Save it
	rTarget.m_aRoot=*pClone;

	//Renormalize it
	rTarget.BuildTreeIndex();

	//And delete that node
	delete pClone;
}

void CSuffixTrie::AddString(const SearchString& rString)
{
	//Add the string
	AddString(rString,
			  &m_aRoot);
}

void CSuffixTrie::AddString(const SearchString& rString,
						    Node* pNode)
{
	//Sanity check
	if (!pNode ||
		rString.empty())
		return;

	//The char we are looking for
	SearchChar aChar;
	aChar=rString[0];

	//Look for the next node
	SearchMap::iterator aIterator;
	aIterator=pNode->aMap.find(aChar);

	//Our node
	Node* pNewNode;

	//Do we have it?
	if (aIterator==pNode->aMap.end())
	{
		//Need to build this node
		pNewNode=new Node;

		//Reset it
		pNewNode->pFailureNode=NULL;
		pNewNode->aChar=aChar;
		pNewNode->usDepth=pNode->usDepth+1;
		pNewNode->bFinal=false;

		//Add it
		pNode->aMap.insert(SearchMap::value_type(aChar,pNewNode));
	}
	else
		//Take the node
		pNewNode=aIterator->second;

	//Is it the last char?
	if (rString.length()==1)
		//Set as last
		pNewNode->bFinal=true;
	else
		//Run next layer
		AddString(rString.substr(1,rString.length()-1),
				  pNewNode);
}

void CSuffixTrie::DeleteNode(Node* pNode)const
{
	//Make sure we have it
	if (!pNode)
		return;

	//Iterate all its children
	for (SearchMap::iterator aIterator=pNode->aMap.begin();
		 aIterator!=pNode->aMap.end();
		 ++aIterator)
	{
		//Send it to deletion
		DeleteNode(aIterator->second);

		//We can assume all the children have been deleted, then delete it
		delete aIterator->second;
	}
}

void CSuffixTrie::BuildTreeIndex()
{
	//Build index on the root
	BuildIndex("",
			   &m_aRoot);
}

CSuffixTrie::Node* CSuffixTrie::SearchNode(const SearchString& rString,
										   Node* pNode)
{
	//Sanity check
	if (!pNode ||
		rString.empty())
		return NULL;

	//The char we are looking for
	SearchChar aChar;
	aChar=rString[0];

	//Look for the next node
	SearchMap::iterator aIterator;
	aIterator=pNode->aMap.find(aChar);

	//Do we have it?
	if (aIterator!=pNode->aMap.end())
	{
		//Is it the last char?
		if (rString.length()==1)
			//We found our string
			return aIterator->second;
		else
			//Search again
			return SearchNode(rString.substr(1,rString.length()-1),
							  aIterator->second);
	}
	else
		//Not found
		return NULL;
}

const CSuffixTrie::Node* CSuffixTrie::SearchNode(const SearchString& rString,
												 const Node* pNode)const
{
	//Sanity check
	if (!pNode ||
		rString.empty())
		return NULL;

	//The char we are looking for
	SearchChar aChar;
	aChar=rString[0];

	//Look for the next node
	SearchMap::const_iterator aIterator;
	aIterator=pNode->aMap.find(aChar);

	//Do we have it?
	if (aIterator!=pNode->aMap.end())
	{
		//Is it the last char?
		if (rString.length()==1)
			//We found our string
			return aIterator->second;
		else
			//Search again
			return SearchNode(rString.substr(1,rString.length()-1),
							  aIterator->second);
	}
	else
		//Not found
		return NULL;
}

void CSuffixTrie::BuildIndex(const SearchString& rString,
							 Node* pNode)
{
	//Sanity
	if (!pNode)
		return;

	//Do we need to process this node?
	if (pNode->usDepth>1)
	{
		//Clear the node first
		pNode->pFailureNode=NULL;

		//We need to start and look for suffix/prefix
		for (int iCount=1;
			 iCount<rString.length();
			 ++iCount)
		{
			//Build the sub string
			SearchString sString;
			sString=rString.substr(iCount,rString.length()-iCount);

			//And search
			Node* pFoundNode;
			pFoundNode=SearchNode(sString,
								  &m_aRoot);

			//Did we get it?
			if (pFoundNode)
			{
				//Save it
				pNode->pFailureNode=pFoundNode;

				//Exit from this loop
				break;
			}
		}	
	}

	//Build the next string
	SearchString sString(rString);

	//Iterate all its children
	for (SearchMap::iterator aIterator=pNode->aMap.begin();
		 aIterator!=pNode->aMap.end();
		 ++aIterator)
		//Build the index
		BuildIndex(rString+aIterator->first,
				   aIterator->second);
}

CSuffixTrie::DataFound CSuffixTrie::SearchAhoCorasik(const SearchString& rString)const
{
	//Our data found
	DataFound aData;
	aData.iFoundPosition=0;

	//The current string we match
	SearchString sMatchedString;

	//Our node position
	const Node* pNode;
	pNode=&m_aRoot;

	//Iterate the string
	for (int iCount=0;
		 iCount<rString.length();
		 ++iCount)
	{
		//Did we switch node already
		bool bSwitch;
		bSwitch=false;

		//Loop while we got something
		while (1)
		{
			//Look for the char
			SearchMap::const_iterator aIterator;
			aIterator=pNode->aMap.find(rString[iCount]);

			//Do we have it?
			if (aIterator==pNode->aMap.end())
				//No, check if we have failure node
				if (!pNode->pFailureNode)
				{
					//No failure node, start at root again
					pNode=&m_aRoot;

					//Reset search string
					sMatchedString="";

					//Did we do a switch?
					if (bSwitch)
						//We need to do this over
						--iCount;

					//Exit this loop
					break;
				}
				else
				{
					//What is the depth difference?
					unsigned short usDepth;
					usDepth=pNode->usDepth-pNode->pFailureNode->usDepth-1;

					//This is how many chars to remove
					sMatchedString=sMatchedString.substr(usDepth,sMatchedString.length()-usDepth);

					//Go to the failure node
					pNode=pNode->pFailureNode;

					//Set to switch
					bSwitch=true;
				}
			else
			{
				//Add the char
				sMatchedString+=rString[iCount];

				//Save the new node
				pNode=aIterator->second;

				//Exit the loop
				break;
			}
		}

		//Is this a final node?
		if (pNode->bFinal)
		{
			//We got our data
			aData.iFoundPosition=iCount-sMatchedString.length()+1;
			aData.sDataFound=sMatchedString;

			//Exit
			return aData;
		}
	}

	//Nothing found
	return aData;
}

CSuffixTrie::DataFoundVector CSuffixTrie::SearchAhoCorasikMultiple(const SearchString& rString)const
{
	//Our vector of data found
	DataFoundVector aVec;

	//The current string we match
	SearchString sMatchedString;

	//Our node position
	const Node* pNode;
	pNode=&m_aRoot;

	//Iterate the string
	for (int iCount=0;
		 iCount<rString.length();
		 ++iCount)
	{
		//Did we switch node already
		bool bSwitch;
		bSwitch=false;

		//Loop while we got something
		while (1)
		{
			//Look for the char
			SearchMap::const_iterator aIterator;
			aIterator=pNode->aMap.find(rString[iCount]);

			//Do we have it?
			if (aIterator==pNode->aMap.end())
				//No, check if we have failure node
				if (!pNode->pFailureNode)
				{
					//No failure node, start at root again
					pNode=&m_aRoot;

					//Reset search string
					sMatchedString="";

					//Did we do a switch?
					if (bSwitch)
						//We need to do this over
						--iCount;

					//Exit this loop
					break;
				}
				else
				{
					//What is the depth difference?
					unsigned short usDepth;
					usDepth=pNode->usDepth-pNode->pFailureNode->usDepth-1;

					if((int)sMatchedString.length() - usDepth  > 0)
					{
						//This is how many chars to remove
					sMatchedString=sMatchedString.substr(usDepth,sMatchedString.length()-usDepth);

					}

				
					//Go to the failure node
					pNode=pNode->pFailureNode;

					//Set to switch
					bSwitch=true;
			
				}
			else
			{
				//Add the char
				sMatchedString+=rString[iCount];

				//Save the new node
				pNode=aIterator->second;

				//Exit the loop
				break;
			}
		}

		//Is this a final node?
		if (pNode->bFinal)
		{
			//We got our data
			DataFound aData;
			aData.iFoundPosition=iCount-sMatchedString.length()+1;
			aData.sDataFound=sMatchedString;

			//Insert it
			aVec.push_back(aData);

			//Go back
			iCount-=sMatchedString.length()-1;

			//Reset the data
			sMatchedString="";
		}
	}

	//Done
	return aVec;
}

CSuffixTrie::Node* CSuffixTrie::CloneNode(const Node* pNode)const
{
	//Sanity check
	if (!pNode)
		return NULL;

	//Create the new node
	Node* pNewNode;
	pNewNode=new Node;

	//Copy the data
	pNewNode->aChar=pNode->aChar;
	pNewNode->bFinal=pNode->bFinal;
	pNewNode->pFailureNode=NULL;
	pNewNode->usDepth=pNode->usDepth;

	//Now clone the sub nodes
	for (SearchMap::const_iterator aIterator=pNode->aMap.begin();
		 aIterator!=pNode->aMap.end();
		 ++aIterator)
	{
		//Clone this sub node
		Node* pSubNode;
		pSubNode=CloneNode(aIterator->second);

		//Did we get it?
		if (pSubNode)
			//Insert it
			pNewNode->aMap.insert(SearchMap::value_type(aIterator->first,pSubNode));
	}

	//Done
	return pNewNode;
}

bool CSuffixTrie::FindString(const SearchString& rString)const
{
	return SearchNode(rString,
					  &m_aRoot)!=NULL;
}

CSuffixTrie::StringsVector CSuffixTrie::GetAllStringsVector()const
{
	//Our vector
	StringsVector aVector;

	//Start to build the trie
	BuildStrings(aVector,
				"",
				 &m_aRoot);

	//Done
	return aVector;
}

CSuffixTrie::StringsSet CSuffixTrie::GetAllStringsSet()const
{
	//We will convert the vector
	StringsVector aVector(GetAllStringsVector());

	//Our set
	StringsSet aSet;

	//Iterate it
	for (int iCount=0;
		 iCount<aVector.size();
		 ++iCount)
		//Insert to the set
		aSet.insert(aVector[iCount]);

	//Done
	return aSet;
}

void CSuffixTrie::BuildStrings(StringsVector& rVector,
							   const SearchString& rString,
							   const Node* pNode)const
{
	//Sanity check
	if (!pNode)
		return;

	//Is this a final node?
	if (pNode->bFinal)
		//Add to the vector
		rVector.push_back(rString);

	//Iterate all its children
	for (SearchMap::const_iterator aIterator=pNode->aMap.begin();
		 aIterator!=pNode->aMap.end();
		 ++aIterator)
		//Send it to next level
		BuildStrings(rVector,
					 rString+aIterator->first,
					 aIterator->second);
}

void CSuffixTrie::DeleteString(const SearchString& rString)
{
	//Our prev node
	Node* pPrevNode;
	pPrevNode=NULL;

	//Start to find the nodes
	for (int iCount=0;
		 iCount<rString.length();
		 ++iCount)
	{
		//Find the node
		Node* pNode;
		pNode=SearchNode(rString.substr(iCount,rString.length()-iCount),
						 &m_aRoot);

		//Do we have a previous node?
		if (pPrevNode &&
			pNode)
		{
			//We need to delete it
			pNode->aMap.erase(pPrevNode->aChar);
			
			//And delete the node
			delete pPrevNode;
			pPrevNode=NULL;
		}

		//Did we get it?
		if (pNode)
			//What stage are we?
			if (!iCount)
				//Does it have children?
				if (pNode->aMap.empty())
					//We can delete it
					pPrevNode=pNode;
				else
				{
					//Can't be final
					pNode->bFinal=false;

					//Exit
					return;
				}
			//Do we have children
			else if (pNode->aMap.empty())
				//We can delete it
				pPrevNode=pNode;
			else
				//Exit
				return;
	}
}
				