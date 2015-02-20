// main.cpp

#include "compile.h"
#include "controller.h"
#include "mapcontroller.h"
#include "basicscreen.h"
#include "profilecontroller.h"
#include "interface.h"
#include "map.h"
#include "screen.h"
#include <vector>

#if defined (__clang__) || defined (_MSC_VER)
#include <codecvt>
#endif

int main()
{
	//setlocale(LC_ALL, "en_US.UTF-8");
	//std::locale loc (std::locale(), new std::codecvt_utf8<char32_t>);
	//std::locale::global(loc);
	//setlocale(LC_ALL, loc.name().c_str());

	//std::cout << loc.name() << std::endl;
#if defined (__GNUG__) || defined (__clang__)
	//setlocale(LC_ALL, "en_US.UTF-8");
#endif
	//setlocale(LC_ALL,std::locale(fin.getloc(),new std::codecvt_utf16<wchar_t, 0xffff, std::consume_header>));

	std::shared_ptr<MasterController> game_controller(new MasterController);

	std::shared_ptr<Screen> profile_page(new BasicScreen("ProfilePage", nullptr, ".//db//profilepage//", "pp"));
	//profile_page->load();

	std::shared_ptr<ProfileController> profile_controller(new ProfileController(game_controller->getProfileWranlger(), game_controller->getProfile(), game_controller->getInterface(), game_controller, profile_page));


	game_controller->push_controller(profile_controller);


	game_controller->controlLoop();
	return 0;
}
