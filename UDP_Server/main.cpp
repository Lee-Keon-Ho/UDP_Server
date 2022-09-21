#include "LobbyServerApp.h"

int main(void)
{
	CLobbyServerApp app;

	if (app.Init())
	{
		app.Run();
	}
	app.Destroy();

	return 0;
}