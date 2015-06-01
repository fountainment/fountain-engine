#include "scenes.h"
#include <cstring>

void RakNetScene::init()
{
	state = 0;
	cButton.setRectSize(200, 200);
	cButton.setPosition(-150, 0);
	cButton.setCaption("Client");
	sButton.setRectSize(200, 200);
	sButton.setPosition(150, 0);
	sButton.setCaption("Server");
	left.setSize(ftVec2(30, 180));
	right.setSize(ftVec2(30, 180));
	left.setCenter(ftVec2(-400 + 15, 0));
	right.setCenter(ftVec2(400 - 15, 0));
	ball.setSize(ftVec2(40, 40));
	ball.setCenter(ftVec2(0, 0));
	client = RakNet::RakPeerInterface::GetInstance();
	server = RakNet::RakPeerInterface::GetInstance();
	ballSpeed = ftVec2(1, 0);
	edgeU.setSize(ftVec2(800, 200));
	edgeU.setCenter(ftVec2(0, 400));
	edgeD.setSize(ftVec2(800, 200));
	edgeD.setCenter(ftVec2(0, -400));
	edgeL.setSize(ftVec2(200, 600));
	edgeL.setCenter(ftVec2(-500, 0));
	edgeR.setSize(ftVec2(200, 600));
	edgeR.setCenter(ftVec2(500, 0));
	leftScore = 0;
	rightScore = 0;
	LL.setPosition(-300, 200);
	RR.setPosition(300, 200);
	ftFile serverAd;
	serverAd.load("server.txt");
	strcpy(serverAddress, serverAd.getStr());
}

void RakNetScene::update()
{
	float speed;
	float rspeed;
	char sss[100];
	RakNet::BitStream bs;
	clientAction ca;
	serverStatus ss;
	sprintf(sss, "%d", leftScore);
	LL.setString(sss);
	sprintf(sss, "%d", rightScore);
	RR.setString(sss);
	switch (state)
	{
	case 0:
		cButton.update();
		sButton.update();
		if (cButton.getState() == FT_ButtonDown) {
			state = 1;
			RakNet::SocketDescriptor sd = RakNet::SocketDescriptor();
			client->Startup(1, &sd, 1);
			client->Connect(serverAddress, 3000, 0, 0);
		}
		if (sButton.getState() == FT_ButtonDown) {
			state = 2;
			RakNet::SocketDescriptor sd = RakNet::SocketDescriptor(3000, 0);
			server->Startup(1, &sd, 1);
			server->SetMaximumIncomingConnections(1);
		}
		break;
	case 1:
		for (RakNet::Packet *packet = client->Receive(); packet; client->DeallocatePacket(packet), packet = client->Receive()) {
                        switch (packet->data[0])
                        {
			case ID_SERVER_STATUS:
                                serverStatus *pss = (serverStatus*)packet->data;
                                left.setXY(ftVec2(left.getX(), pss->yL));
				right.setXY(ftVec2(right.getX(), pss->yR));
				ball.setXY(ftVec2(pss->ballX, pss->ballY));
				leftScore = pss->scoreL;
				rightScore = pss->scoreR;
				break;
                        }
		}
		speed = 0.0;
		if (fountain::sysKeyboard.getState(FT_Up)) {
                        speed += 5.0;
		}
		if (fountain::sysKeyboard.getState(FT_Down)) {
                        speed -= 5.0;
		}
		right.move(0, speed);
		ca.speed = speed;
		bs.Write((char *)&ca, sizeof ca);
		client->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		break;
	case 2:
		speed = 0.0;
		if (fountain::sysKeyboard.getState(FT_Up)) {
                        speed += 5.0;
		}
		if (fountain::sysKeyboard.getState(FT_Down)) {
                        speed -= 5.0;
		}
		left.move(0, speed);
		ball.move(ballSpeed.x, ballSpeed.y);

		ballSpeed *= 1.001;
		ss.yL =	left.getY();
		ss.yR = right.getY();
		ss.ballX = ball.getX();
		ss.ballY = ball.getY();
		ss.scoreL = leftScore;
		ss.scoreR = rightScore;
		bs.Write((char *)&ss, sizeof ss);
		server->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		for (RakNet::Packet *packet = server->Receive(); packet; server->DeallocatePacket(packet), packet = server->Receive()) {
                        switch (packet->data[0])
                        {
			case ID_CLIENT_ACTION:
                                clientAction *pca = (clientAction*)packet->data;
                                right.move(0, pca->speed);
                                rspeed = pca->speed;
				break;
                        }
		}
		if (ball.collideRect(left) && ballSpeed.x < 0) {
			ballSpeed.x *= -1;
			ballSpeed.y += speed * 0.3;
		}
		if (ball.collideRect(right) && ballSpeed.x > 0) {
			ballSpeed.x *= -1;
			ballSpeed.y += rspeed * 0.3;
		}
		if (ball.collideRect(edgeU) && ballSpeed.y > 0) {
			ballSpeed.y *= -1;
		}
		if (ball.collideRect(edgeD) && ballSpeed.y < 0) {
			ballSpeed.y *= -1;
		}
		if (ball.collideRect(edgeL)) {
			ball.setCenter(ftVec2(0, 0));
			ballSpeed = ftVec2(ftVec2(1, 0));
			rightScore++;
		}
		if (ball.collideRect(edgeR)) {
			ball.setCenter(ftVec2(0, 0));
			ballSpeed = ftVec2(ftVec2(-1, 0));
			leftScore++;
		}

		break;
	}
}

void RakNetScene::draw()
{
	mainCamera.update();
	if (state == 0) {
		cButton.draw();
		sButton.draw();
	} else {
		ftRender::ftColor3f(1.0, 1.0, 1.0);
		ftRender::drawRect(left);
		ftRender::drawRect(right);
		for (int i = 0; i < 10; i++) {
			ftRender::transformBegin(0, i * 80 - 300);
			ftRender::drawQuad(20, 50);
			ftRender::transformEnd();
		}
		ftRender::ftColor3f(1.0, 1.0, 0.5);
		ftRender::drawRect(ball);
		ftRender::ftColor3f(1.0, 1.0, 1.0);
		LL.draw();
		RR.draw();
	}
}

void RakNetScene::destroy()
{
	RakNet::RakPeerInterface::DestroyInstance(client);
	RakNet::RakPeerInterface::DestroyInstance(server);
}
