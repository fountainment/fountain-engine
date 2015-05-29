#include <fountain/fountaindef.h>
#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"

enum {
ID_SERVER_STATUS = ID_USER_PACKET_ENUM,
ID_CLIENT_ACTION = ID_USER_PACKET_ENUM + 1
};

#pragma pack(push, 1)
struct serverStatus
{
	unsigned char id;
	float yL, yR;
	float ballX, ballY;
	float scoreL, scoreR;

	serverStatus()
	{
		id = ID_SERVER_STATUS;
	}
};

struct clientAction
{
	unsigned char id;
	char speed;

	clientAction()
	{
		id = ID_CLIENT_ACTION;
	}
};
#pragma pack(pop)

class RakNetScene : public ftScene::Scene
{
	int state;
	ftUI::Button cButton, sButton;
	ftRect left, right, ball;
	ftRect edgeL, edgeR, edgeU, edgeD;
	RakNet::RakPeerInterface *client, *server;
	ftVec2 ballSpeed;
	int leftScore, rightScore;
	ftUI::Label LL, RR;
	char serverAddress[100];
public:
        void init();
        void update();
        void draw();
        void destroy();
};
