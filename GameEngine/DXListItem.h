#ifndef DX_LIST_ITEM_H
#define DX_LIST_ITEM_H

#include "dxlabel.h"

static const int s_nMaxListItemColumn = 10;

class CDXListItem : public CDXLabel
{
public:
    CDXListItem(void);
    virtual ~CDXListItem(void);

    virtual void Render();

private:

    int m_nColumnCount;
};

#endif