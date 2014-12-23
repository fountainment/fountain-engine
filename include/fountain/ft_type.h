#ifndef _FT_TYPE_H_
#define _FT_TYPE_H_

namespace ftType {

	bool init();

	class fontMan {
	public:
		fontMan();
		bool loadFont();
		void drawString();
	};
};

#endif
