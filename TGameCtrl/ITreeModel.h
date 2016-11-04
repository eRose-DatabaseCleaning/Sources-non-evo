#ifndef _ITREEMODEL_
#define _ITREEMODEL_

#include "TGameCtrl.h"
#include <crtdbg.h>

class CTObject;
class ITreeNode;

//------------------------------------------------------------------------------------------------------------------------
///	CTree���� Data Model�� ����ϰ��� �ϴ� Class�� �����ؾ��� Interface
///
/// @Author			������
/// @Date			2005/08/23 
//------------------------------------------------------------------------------------------------------------------------
class TGAMECTRL_API ITreeModel
{
public:

	virtual ITreeNode*	getChild(ITreeNode* , int& )=0 { _ASSERT( 0 && "ITreeModel::getChild" ); return NULL; };		///Returns the child of parent at index index in the parent's child array. 
	virtual void		getChildCount(ITreeNode*, int& )=0 { _ASSERT( 0 && "ITreeModel::getChildCount" ); };				///Returns the number of children of parent. 
	virtual int			getIndexOfChild(CTObject* , CTObject* )=0 { _ASSERT( 0 && "ITreeModel::getIndexOfChild"); return 0; };///Returns the index of child in parent. 
	virtual ITreeNode*	getRoot()=0 { _ASSERT( 0 && "ITreeModel::getRoot");return NULL; };									///Returns the root of the tree. 
	virtual bool		isLeaf(CTObject* ) = 0{ _ASSERT( 0 && "ITreeModel::isLeaf"); return true; };					///Returns true if node is a leaf. 
};
#endif