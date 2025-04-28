#include "utility/utils.h"
#include "manager/userManager.h"
#include "controller/mainController.h"
#include "model/image/assets.h"
#include "model/user/user.h"

#include <Magick++.h>

using json = nlohmann::json;

int main()
{
    Magick::InitializeMagick(nullptr);
	Assets::getInstance();
	UserManager::getInstance().loadExistingUsers();
    MainController::getInstance().start();

    return 0;
}