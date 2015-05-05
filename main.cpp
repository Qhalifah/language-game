/*Copywrite 2014 Shaun Bond, Jeffery Waldon, Charlie Carlson, Simon Smith, Joseph DeWilde, Aaron Andrews*/

/*This file is part of Dina'ki Adventures.

Dina'ki Adventures is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Dina'ki Adventures is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Dina'ki Adventures. If not, see <http://www.gnu.org/licenses/>.*/

// main.cpp

//#define  SFML_STATIC

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
