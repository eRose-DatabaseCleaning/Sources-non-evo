#ifndef __GLOBALDATA_H__
#define __GLOBALDATA_H__


#define VGDATA_FILEENTRY_MAX_COUNT	210000
#define VGDATA_VENTRY_MAX_COUNT		100
#define VGDATA_FILENAME_MAX_LENGTH	2048

class CGlobalData
{
public:
	
	static const int FILEENTRY_MIN_COUNT = 0;
	static const int FILEENTRY_MAX_COUNT = VGDATA_FILEENTRY_MAX_COUNT;

	static const int VENTRY_MAX_COUNT = VGDATA_VENTRY_MAX_COUNT;

	static const int FILENAME_MAX_LENGTH = VGDATA_FILENAME_MAX_LENGTH;
};

#endif
