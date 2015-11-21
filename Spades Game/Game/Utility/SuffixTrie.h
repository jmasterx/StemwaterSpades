/*
 *  Copyright (c) 2000-2008 Barak Weichselbaum <barak@komodia.com>
 *  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Contact info:
 * -------------
 *
 * Site:					http://www.komodia.com
 * Main contact:			barak@komodia.com
 * For custom projects, 
 * consulting, or other
 * paid services:			sales@komodia.com
 */

#if !defined(AFX_SUFFIXTRIE_H__34D2D872_1C59_4D9F_BAF1_6AB80D7333B1__INCLUDED_)
#define AFX_SUFFIXTRIE_H__34D2D872_1C59_4D9F_BAF1_6AB80D7333B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <map>
#include <string>
#include <vector>
#include <set>

class CSuffixTrie 
{
public:
	//Our string type
	typedef std::string SearchString;

	//Data returned from our search
	typedef struct _DataFound
	{
		int				iFoundPosition;
		SearchString	sDataFound;
	} DataFound;

	//Our vector of data found
	typedef std::vector<DataFound> DataFoundVector;

	//All the strings vector
	typedef std::vector<SearchString> StringsVector;

	//All the strings set
	typedef std::set<SearchString> StringsSet;
public:
	//Get all the strings in the tree
	//Vector format
	StringsVector GetAllStringsVector()const;

	//Set format
	StringsSet GetAllStringsSet()const;

	//Clear the trie
	void Clear();

	//Build the tree index for Aho-Corasick
	//This is done when all the strings has been added
	void BuildTreeIndex();

	//Add a string (will destroy normalization, caller is reponsible for this part)
	void AddString(const SearchString& rString);

	//Get string (is the string there?)
	bool FindString(const SearchString& rString)const;

	//Delete a string (will destroy normalization, caller is reponsible for this part)
	void DeleteString(const SearchString& rString);

	//Do an actual find for the first match
	DataFound SearchAhoCorasik(const SearchString& rString)const;

	//Do an actual find for all the matches
	DataFoundVector SearchAhoCorasikMultiple(const SearchString& rString)const;

	//Assigmnet operator
	CSuffixTrie& operator=(const CSuffixTrie& rTrie);

	//Ctor and Dtor
	CSuffixTrie();
	CSuffixTrie(const CSuffixTrie& rTrie);
	virtual ~CSuffixTrie();
private:
	//Our char search type
	typedef char SearchChar;

	//Forward declare the node
	struct _Node;

	//Our map
	typedef std::map <SearchChar,_Node*> SearchMap;

	//Our node
	typedef struct _Node
	{
		SearchChar		aChar;	//Our character
		bool			bFinal; //Do we have a word here
		SearchMap		aMap;	//Our next nodes
		_Node*			pFailureNode;	//Where we go incase of failure
		unsigned short	usDepth;	//Depth of this level
	} Node;
private:
	//Add a string to a node
	void AddString(const SearchString& rString,
				   Node* pNode);

	//Search for a non final string (this is to build the index)
	//If not found then it will get the root node
	const Node* SearchNode(const SearchString& rString,
						   const Node* pNode)const;
	Node* SearchNode(const SearchString& rString,
					 Node* pNode);

	//Build the node index
	void BuildIndex(const SearchString& rString,
					Node* pNode);

	//Delete a node
	void DeleteNode(Node* pNode)const;

	//Clone a node
	Node* CloneNode(const Node* pNode)const;

	//Clone the entire trie
	void CloneTrie(CSuffixTrie& rTarget)const;

	//Insert a string into a vector
	void BuildStrings(StringsVector& rVector,
				      const SearchString& rString,
					  const Node* pNode)const;

	//Our root node
	Node m_aRoot;
};

#endif // !defined(AFX_SUFFIXTRIE_H__34D2D872_1C59_4D9F_BAF1_6AB80D7333B1__INCLUDED_)
