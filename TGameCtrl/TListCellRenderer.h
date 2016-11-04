#ifndef _CTListCellRenderer_
#define _CTListCellRenderer_
#include "ilistcellrenderer.h"
class CTStatic;

/**
* CTContainerItem�� Item���� ������ Container�� ������ JListBox�� ���� Renderer
*
* @Author		������
*
* @Data			2005/8/30	
*/
class TGAMECTRL_API CTListCellRenderer : public IListCellRenderer
{
	CTStatic*		m_pComponent;
public:
	CTListCellRenderer(void);
	~CTListCellRenderer(void);

	virtual CWinCtrl* GetListCellRendererComponent( const CTObject* pObject, bool selected, bool mouseover );
};
#endif