#include <iostream>
#include "services/ComflixController.h"
#include "ui/Menu.h"

using namespace std;

int main() {

	ComflixController app(400 );

	// Users - ADD FIRST (before loading, so we don't lose persisted state)
	app.addUser(User(1, "Rimsha"));
	app.addUser(User(2, "Arham"));
	app.addUser(User(3, "Wajeeha"));
	app.addUser(User(4, "Mehwish"));

	// Load data - AFTER users added, so we restore persisted history/watchLater for existing users
	app.loadData();

	app.setCurrentUser(1);



	// Launch UI
	Menu menu(app);
	menu.showMainMenu();

	return 0;
}